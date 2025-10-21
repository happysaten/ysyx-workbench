// MEM(Memory) 负责统一内存的读写访问
module MEM (
    input logic             clk,
    input logic             reset,
          axi_lite_if.slave mem     // 使用interface替代所有独立的AXI信号
);
    // 读通道状态机
    typedef enum logic [1:0] {
        RD_IDLE,
        RD_WAIT,
        RD_RESP
    } rd_state_t;
    rd_state_t rd_state, rd_next_state;

    // 写通道状态机
    typedef enum logic [2:0] {
        WR_IDLE,
        WR_ADDR,
        WR_DATA,
        WR_WAIT,
        WR_RESP
    } wr_state_t;
    wr_state_t wr_state, wr_next_state;

    // 读通道状态更新
    always @(posedge clk) begin
        if (reset) rd_state <= RD_IDLE;
        else rd_state <= rd_next_state;
    end

    // 写通道状态更新
    always @(posedge clk) begin
        if (reset) wr_state <= WR_IDLE;
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
            RD_IDLE: begin
                rd_next_state = (mem.arvalid && mem.arready) ? (rd_resp_ready ? RD_RESP : RD_WAIT) : RD_IDLE;
            end
            RD_WAIT: rd_next_state = rd_resp_ready ? RD_RESP : RD_WAIT;
            RD_RESP: rd_next_state = (mem.rvalid && mem.rready) ? RD_IDLE : RD_RESP;
            default: rd_next_state = RD_IDLE;
        endcase
    end

    // 写通道状态机
    always_comb begin
        unique case (wr_state)
            WR_IDLE: begin
                if (mem.awvalid && mem.awready && mem.wvalid && mem.wready) begin
                    // 地址和数据同时到达
                    wr_next_state = wr_resp_ready ? WR_RESP : WR_WAIT;
                end else if (mem.awvalid && mem.awready) begin
                    // 只有地址到达
                    wr_next_state = WR_DATA;
                end else if (mem.wvalid && mem.wready) begin
                    // 只有数据到达
                    wr_next_state = WR_ADDR;
                end else begin
                    wr_next_state = WR_IDLE;
                end
            end
            WR_ADDR: begin
                // 等待地址到达
                wr_next_state = (mem.awvalid && mem.awready) ?
                                (wr_resp_ready ? WR_RESP : WR_WAIT) : WR_ADDR;
            end
            WR_DATA: begin
                // 等待数据到达
                wr_next_state = (mem.wvalid && mem.wready) ?
                                (wr_resp_ready ? WR_RESP : WR_WAIT) : WR_DATA;
            end
            WR_WAIT: wr_next_state = wr_resp_ready ? WR_RESP : WR_WAIT;
            WR_RESP: wr_next_state = (mem.bvalid && mem.bready) ? WR_IDLE : WR_RESP;
            default: wr_next_state = WR_IDLE;
        endcase
    end

    // 读通道握手信号
    assign mem.arready = (rd_state == RD_IDLE);
    assign mem.rvalid  = (rd_state == RD_RESP);

    // 写通道握手信号
    assign mem.awready = (wr_state == WR_IDLE) || (wr_state == WR_ADDR);
    assign mem.wready  = (wr_state == WR_IDLE) || (wr_state == WR_DATA);
    assign mem.bvalid  = (wr_state == WR_RESP);

    import "DPI-C" function int pmem_read_npc(input int raddr);
    import "DPI-C" function void pmem_write_npc(
        input int  waddr,
        input int  wdata,
        input byte wmask
    );

    // 读事务处理
    always @(posedge clk) begin
        if (mem.arvalid && mem.arready) mem.rdata <= pmem_read_npc(mem.araddr);
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
        end else if (mem.awvalid && mem.awready) begin
            wr_addr_reg <= mem.awaddr;
            wr_addr_received <= 1'b1;
        end else if (wr_state == WR_IDLE) begin
            wr_addr_received <= 1'b0;
        end
    end

    // 保存写数据
    always @(posedge clk) begin
        if (reset) begin
            wr_data_reg <= 32'h0;
            wr_mask_reg <= 8'h0;
            wr_data_received <= 1'b0;
        end else if (mem.wvalid && mem.wready) begin
            wr_data_reg <= mem.wdata;
            wr_mask_reg <= mem.wmask;
            wr_data_received <= 1'b1;
        end else if (wr_state == WR_IDLE) begin
            wr_data_received <= 1'b0;
        end
    end

    // 写事务处理
    logic [31:0] final_waddr;
    logic [31:0] final_wdata;
    logic [ 7:0] final_wmask;

    assign final_waddr = wr_addr_received ? wr_addr_reg : mem.awaddr;
    assign final_wdata = wr_data_received ? wr_data_reg : mem.wdata;
    assign final_wmask = wr_data_received ? wr_mask_reg : mem.wmask;

    always @(posedge clk) begin
        if ((wr_state == WR_DATA && mem.wvalid && mem.wready) ||
            (wr_state == WR_ADDR && mem.awvalid && mem.awready) ||
            (wr_state == WR_IDLE && mem.awvalid && mem.awready && mem.wvalid && mem.wready)) begin
            pmem_write_npc(final_waddr, final_wdata, final_wmask);
        end
    end

    assign mem.rresp = 0;
    assign mem.bresp = 0;

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
