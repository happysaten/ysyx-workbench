// MEM(Memory) 负责统一内存的读写访问
module MEM (
    input logic             clk,
    input logic             reset,
          axi_lite_if.slave s     // 使用interface替代所有独立的AXI信号
);
    // 读通道状态机
    typedef enum logic [1:0] {
        IDLE_RD,
        WAIT_RD,
        RESP_RD
    } rd_state_t;
    rd_state_t rd_state, rd_next_state;

    // 写通道状态机
    typedef enum logic [1:0] {
        IDLE_WR,
        WAIT_WDATA,
        WAIT_WR,
        RESP_WR
    } wr_state_t;
    wr_state_t wr_state, wr_next_state;

    // 读通道状态更新
    always @(posedge clk) begin
        if (reset) rd_state <= IDLE_RD;
        else rd_state <= rd_next_state;
    end

    // 写通道状态更新
    always @(posedge clk) begin
        if (reset) wr_state <= IDLE_WR;
        else wr_state <= wr_next_state;
    end

    logic rd_resp_ready, wr_resp_ready;
    logic random_bit;

    // 随机延迟生成器
    lfsr8 #(
        .TAPS(8'b01010110)
    ) u_rd_resp_lfsr (
        .clk  (clk),
        .reset(reset),
        .en   (1'b1),
        .out  (random_bit)
    );

    assign rd_resp_ready = random_bit;
    assign wr_resp_ready = random_bit;

    // 读通道状态机
    always_comb begin
        unique case (rd_state)
            IDLE_RD: begin
                rd_next_state = (s.arvalid && s.arready) ? (rd_resp_ready ? RESP_RD : WAIT_RD) : IDLE_RD;
            end
            WAIT_RD: rd_next_state = rd_resp_ready ? RESP_RD : WAIT_RD;
            RESP_RD: rd_next_state = (s.rvalid && s.rready) ? IDLE_RD : RESP_RD;
            default: rd_next_state = IDLE_RD;
        endcase
    end

    // 写通道状态机
    always_comb begin
        unique case (wr_state)
            IDLE_WR: begin
                if (s.awvalid && s.awready && s.wvalid && s.wready) begin
                    // 地址和数据同时到达
                    wr_next_state = wr_resp_ready ? RESP_WR : WAIT_WR;
                end else if (s.awvalid && s.awready) begin
                    // 只有地址到达
                    wr_next_state = WAIT_WDATA;
                end else begin
                    wr_next_state = IDLE_WR;
                end
            end
            WAIT_WDATA: begin
                // 等待数据到达
                wr_next_state = (s.wvalid && s.wready) ?
                                (wr_resp_ready ? RESP_WR : WAIT_WR) : WAIT_WDATA;
            end
            WAIT_WR: wr_next_state = wr_resp_ready ? RESP_WR : WAIT_WR;
            RESP_WR: wr_next_state = (s.bvalid && s.bready) ? IDLE_WR : RESP_WR;
            default: wr_next_state = IDLE_WR;
        endcase
    end

    // 读通道握手信号
    assign s.arready = (rd_state == IDLE_RD);
    assign s.rvalid  = (rd_state == RESP_RD);

    // 写通道握手信号
    assign s.awready = (wr_state == IDLE_WR);
    assign s.wready  = (wr_state == IDLE_WR) || (wr_state == WAIT_WDATA);
    assign s.bvalid  = (wr_state == RESP_WR);

    import "DPI-C" function int pmem_read_npc(input int raddr);
    import "DPI-C" function void pmem_write_npc(
        input int  waddr,
        input int  wdata,
        input byte wmask
    );

    // 读事务处理
    always @(posedge clk) begin
        if (s.arvalid && s.arready) s.rdata <= pmem_read_npc(s.araddr);
    end

    logic [31:0] wr_addr_reg;
    logic [31:0] wr_data_reg;
    logic [ 7:0] wr_mask_reg;
    logic wr_addr_received, wr_data_received;

    // 保存写地址
    always @(posedge clk) begin
        if (reset) begin
            wr_addr_reg <= 32'h0;
            wr_addr_received <= 1'b0;
        end else if (s.awvalid && s.awready) begin
            wr_addr_reg <= s.awaddr;
            wr_addr_received <= 1'b1;
        end else if (wr_state == IDLE_WR) begin
            wr_addr_received <= 1'b0;
        end
    end

    // 保存写数据
    always @(posedge clk) begin
        if (reset) begin
            wr_data_reg <= 32'h0;
            wr_mask_reg <= 8'h0;
            wr_data_received <= 1'b0;
        end else if (s.wvalid && s.wready) begin
            wr_data_reg <= s.wdata;
            wr_mask_reg <= s.wmask;
            wr_data_received <= 1'b1;
        end else if (wr_state == IDLE_WR) begin
            wr_data_received <= 1'b0;
        end
    end

    // 写事务处理
    logic [31:0] final_waddr;
    logic [31:0] final_wdata;
    logic [ 7:0] final_wmask;

    assign final_waddr = wr_addr_received ? wr_addr_reg : s.awaddr;
    assign final_wdata = wr_data_received ? wr_data_reg : s.wdata;
    assign final_wmask = wr_data_received ? wr_mask_reg : s.wmask;

    always @(posedge clk) begin
        if ((wr_state == WAIT_WDATA && s.wvalid && s.wready) ||
            (wr_state == IDLE_WR && s.awvalid && s.awready && s.wvalid && s.wready)) begin
            pmem_write_npc(final_waddr, final_wdata, final_wmask);
        end
    end

    assign s.rresp = 2'b00;
    assign s.bresp = 2'b00;

endmodule

module lfsr8 #(
    parameter logic [7:0] TAPS = 8'b10111000 // 默认抽头：位7,5,4,3，对应x^8 + x^6 + x^5 + x^4 + 1
) (
    input clk,
    input reset,
    input en,
    output logic out
);

    logic [7:0] lfsr;
    logic feedback;
    assign feedback = ^(lfsr & TAPS);  // 参数化反馈计算

    // 8-bit maximal-length LFSR polynomial: x^8 + x^6 + x^5 + x^4 + 1
    always_ff @(posedge clk) begin
        if (reset) lfsr <= 8'h1;  // 初始值不能为0
        else if (en) lfsr <= {lfsr[6:0], feedback};
    end

    assign out = (lfsr[1:0] == 2'b10);

endmodule
