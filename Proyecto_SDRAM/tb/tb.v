`timescale 1ns / 1ns
`include  "tb_define.v"

// Testbench top level
module tb();

reg               wb_clk_i; // System Clock
// Config Signals
reg [1:0]         cfg_sdr_width;
reg [1:0]         cfg_colbits;
// WB bus
reg               wb_rst_i;
reg               wb_stb_i;
reg [25:0]        wb_addr_i;
reg               wb_we_i;
reg [`dw-1:0]     wb_dat_i;
reg [`dw/8-1:0]   wb_sel_i;
reg               wb_cyc_i;
reg [2:0]         wb_cti_i;
// Interface to SDRAM
reg               sdram_resetn;
reg               sdram_clk;
//reg [3:0]         sdram_wr_en_n;
// Parameters
reg [1:0]         cfg_req_depth;
reg               cfg_sdr_en;
reg [12:0]        cfg_sdr_mode_reg;
reg [3:0]         cfg_sdr_tras_d;
reg [3:0]         cfg_sdr_trp_d;
reg [3:0]         cfg_sdr_trcd_d;
reg [2:0]         cfg_sdr_cas;
reg [3:0]         cfg_sdr_trcar_d;
reg [3:0]         cfg_sdr_twr_d;
reg [`SDR_RFSH_TIMER_W-1:0]    cfg_sdr_rfsh;
reg [`SDR_RFSH_ROW_CNT_W-1:0]  cfg_sdr_rfmax;

// WB Bus
wire                wb_ack_o;
wire [`dw-1:0]      wb_dat_o;
// Interface to SDRAM
wire                sdr_cs_n;
wire                sdr_cke;
wire                sdr_ras_n;
wire                sdr_cas_n;
wire                sdr_we_n;
wire [`SDR_BW-1:0]  sdr_dqm;
wire [1:0]          sdr_ba;
wire [12:0]         sdr_addr;
wire [`SDR_DW-1:0]  sdr_dq;
// Parameters
wire                sdr_init_done;




initial begin
 $dumpfile("verilog.vcd");
 $dumpvars(0);
 $display("[DUT]: Starting...");
 $sc_tb;  // Testbench Connection
 wb_clk_i   = 0;
 sdram_clk = 0;
end

// Clock generator
always #(`P_SYS/2) wb_clk_i  = ~wb_clk_i;
always #(`P_SDR/2) sdram_clk = ~sdram_clk;

// DUT connection
sdrc_top #(.SDR_DW(`SDR_DW),.SDR_BW(`SDR_BW),.dw(`dw),.APP_AW(`APP_AW))
  dut (

    .cfg_sdr_width( cfg_sdr_width    )  ,
    .cfg_colbits(   cfg_colbits      )  ,

    // WB bus
    .wb_rst_i(      wb_rst_i         )  ,
    .wb_clk_i(      wb_clk_i         )  ,
    .wb_stb_i(      wb_stb_i         )  ,
    .wb_ack_o(      wb_ack_o         )  ,
    .wb_addr_i(     wb_addr_i        )  ,
    .wb_we_i(       wb_we_i          )  ,
    .wb_dat_i(      wb_dat_i         )  ,
    .wb_sel_i(      wb_sel_i         )  ,
    .wb_dat_o(      wb_dat_o         )  ,
    .wb_cyc_i(      wb_cyc_i         )  ,
    .wb_cti_i(      wb_cti_i         )  ,

		/* Interface to SDRAMs */
    .sdram_clk(     sdram_clk        )  ,
    .sdram_resetn(  sdram_resetn     )  ,
    //sdram_wr_en_n    )  ,
    .sdr_cs_n(      sdr_cs_n         )  ,
    .sdr_cke(       sdr_cke          )  ,
    .sdr_ras_n(     sdr_ras_n        )  ,
    .sdr_cas_n(     sdr_cas_n        )  ,
    .sdr_we_n(      sdr_we_n         )  ,
    .sdr_dqm(       sdr_dqm          )  ,
    .sdr_ba(        sdr_ba           )  ,
    .sdr_addr(      sdr_addr         )  ,
    .sdr_dq(        sdr_dq           )  ,

    .sdr_init_done( sdr_init_done    )  ,
		/* Parameters */
    .cfg_req_depth( cfg_req_depth    )  ,   //how many req. buffer should hold
    .cfg_sdr_en(    cfg_sdr_en       )  ,
    .cfg_sdr_mode_reg(  cfg_sdr_mode_reg )  ,
    .cfg_sdr_tras_d(    cfg_sdr_tras_d   )  ,
    .cfg_sdr_trp_d(     cfg_sdr_trp_d    )  ,
    .cfg_sdr_trcd_d(    cfg_sdr_trcd_d   )  ,
    .cfg_sdr_cas(       cfg_sdr_cas      )  ,
    .cfg_sdr_trcar_d(   cfg_sdr_trcar_d  )  ,
    .cfg_sdr_twr_d(     cfg_sdr_twr_d    )  ,
    .cfg_sdr_rfsh(      cfg_sdr_rfsh     )  ,
	  .cfg_sdr_rfmax(     cfg_sdr_rfmax    )
  );


  // BFM
  wire #(2.0) sdram_clk_d   = sdram_clk;

  mt48lc8m8a2 #(.data_bits(8)) u_sdram8 (
            .Dq                 (sdr_dq             ),
            .Addr               (sdr_addr[11:0]     ),
            .Ba                 (sdr_ba             ),
            .Clk                (sdram_clk_d        ),
            .Cke                (sdr_cke            ),
            .Cs_n               (sdr_cs_n           ),
            .Ras_n              (sdr_ras_n          ),
            .Cas_n              (sdr_cas_n          ),
            .We_n               (sdr_we_n           ),
            .Dqm                (sdr_dqm            )
       );

endmodule
