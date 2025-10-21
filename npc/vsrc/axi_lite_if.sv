interface axi_lite_if;

    // Read address channel (AR)
    logic        arvalid;
    logic        arready;
    logic [31:0] araddr;

    // Read data channel (R)
    logic        rvalid;
    logic        rready;
    logic [31:0] rdata;
    logic        rresp;

    // Write address channel (AW)
    logic        awvalid;
    logic        awready;
    logic [31:0] awaddr;

    // Write data channel (W)
    logic        wvalid;
    logic        wready;
    logic [31:0] wdata;
    logic [ 7:0] wmask;

    // Write response channel (B)
    logic        bvalid;
    logic        bready;
    logic        bresp;

    // Master 端口方向（CPU 或 XBAR 上游）
    modport master (
        output arvalid, araddr, rready,
        output awvalid, awaddr,
        output wvalid,  wdata,  wmask,
        output bready,

        input  arready, rvalid, rdata, rresp,
        input  awready, wready, bvalid, bresp
    );

    // Slave 端口方向（UART/MEM等外设或 XBAR 下游）
    modport slave (
        input  arvalid, araddr, rready,
        input  awvalid, awaddr,
        input  wvalid,  wdata,  wmask,
        input  bready,

        output arready, rvalid, rdata, rresp,
        output awready, wready, bvalid, bresp
    );

endinterface
