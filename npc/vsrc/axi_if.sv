interface axi4_lite_if;

    // Read address channel (AR)
    logic        arvalid;
    logic        arready;
    logic [31:0] araddr;

    // Read data channel (R)
    logic        rvalid;
    logic        rready;
    logic [31:0] rdata;
    logic [ 1:0] rresp;

    // Write address channel (AW)
    logic        awvalid;
    logic        awready;
    logic [31:0] awaddr;

    // Write data channel (W)
    logic        wvalid;
    logic        wready;
    logic [31:0] wdata;
    logic [ 3:0] wstrb;

    // Write response channel (B)
    logic        bvalid;
    logic        bready;
    logic [ 1:0] bresp;

    // Master 端口方向（CPU 或 XBAR 上游）
    modport master(
        output arvalid, araddr, rready,
        output awvalid, awaddr,
        output wvalid, wdata, wstrb,
        output bready,

        input arready, rvalid, rdata, rresp,
        input awready, wready, bvalid, bresp
    );

    // Slave 端口方向（UART/MEM等外设或 XBAR 下游）
    modport slave(
        input arvalid, araddr, rready,
        input awvalid, awaddr,
        input wvalid, wdata, wstrb,
        input bready,

        output arready, rvalid, rdata, rresp,
        output awready, wready, bvalid, bresp
    );

endinterface

interface axi4_if;

    // Read Address Channel (AR)
    logic        arvalid;
    logic        arready;
    logic [31:0] araddr;
    logic [ 3:0] arid;
    logic [ 7:0] arlen;
    logic [ 2:0] arsize;
    logic [ 1:0] arburst;

    // Read Data Channel (R)
    logic        rvalid;
    logic        rready;
    logic [31:0] rdata;
    logic [ 1:0] rresp;
    logic        rlast;
    logic [ 3:0] rid;

    // Write Address Channel (AW)
    logic        awvalid;
    logic        awready;
    logic [31:0] awaddr;
    logic [ 3:0] awid;
    logic [ 7:0] awlen;
    logic [ 2:0] awsize;
    logic [ 1:0] awburst;

    // Write Data Channel (W)
    logic        wvalid;
    logic        wready;
    logic [31:0] wdata;
    logic [ 3:0] wstrb;
    logic        wlast;

    // Write Response Channel (B)
    logic        bvalid;
    logic        bready;
    logic [ 1:0] bresp;
    logic [ 3:0] bid;

    // Master 端口方向（发起方，如 CPU 或 XBAR 上游）
    // 通道顺序：AR, R, AW, W, B
    modport master(
        // AR channel outputs from master
        output arvalid, araddr, arid, arlen, arsize, arburst,
        // R channel input control from master
        output rready,

        // AW channel outputs from master
        output awvalid, awaddr, awid, awlen, awsize, awburst,
        // W channel outputs from master
        output wvalid, wdata, wstrb, wlast,
        // B channel control from master
        output bready,

        // inputs to master (from slave)
        input arready,
        input rvalid, rdata, rresp, rlast, rid,
        input awready, wready,
        input bvalid, bresp, bid
    );

    // Slave 端口方向（响应方，如 外设 或 XBAR 下游）
    modport slave(
        // AR channel inputs to slave
        input arvalid, araddr, arid, arlen, arsize, arburst,
        // R channel control from slave
        input rready,

        // AW channel inputs to slave
        input awvalid, awaddr, awid, awlen, awsize, awburst,
        // W channel inputs to slave
        input wvalid, wdata, wstrb, wlast,
        // B channel control from slave
        input bready,

        // outputs from slave (to master)
        output arready,
        output rvalid, rdata, rresp, rlast, rid,
        output awready, wready,
        output bvalid, bresp, bid
    );

    // 说明：io_master_* / io_slave_* 命名可在顶层 port 处按题目表格映射到本接口信号
endinterface
