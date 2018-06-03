`timescale 1ns / 1ns
`include  "tb_define.v"

// Testbench top level
module tb();

reg               sys_clk; // System Clock
// Config Signals
reg [1:0]         cfg_sdr_width;
reg [1:0]         cfg_colbits;
// WB bus
reg               wb_rst_i;
reg               wb_stb_i;
reg [APP_AW-1:0]  wb_addr_i;
reg               wb_we_i;
reg [dw-1:0]      wb_dat_i;
reg [dw/8-1:0]    wb_sel_i;
reg               wb_cyc_i;
reg [2:0]         wb_cti_i;
// Interface to SDRAM
reg               sdram_resetn;
reg               sdram_clk;
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
reg [SDR_RFSH_TIMER_W-1:0]    cfg_sdr_rfsh;
reg [SDR_RFSH_ROW_CNT_W-1:0]  cfg_sdr_rfmax;

wire              wb_ack_o;
wire [dw-1:0]     wb_dat_o;
  // Interface to SDRAM
wire              sdram_clk;
wire              sdr_cs_n;
wire              sdr_cke;
wire              sdr_ras_n;
wire              sdr_cas_n;
wire              sdr_we_n;
wire [SDR_BW-1:0] sdr_dqm;
wire [1:0]        sdr_ba;
wire [12:0]       sdr_addr;
wire [SDR_DW-1:0] sdr_dq;


initial begin
 $dumpfile("verilog.vcd");
 $dumpvars(0);
 $display("[DUT]: Starting...");
 $sc_tb;// Testbench Connection
 sys_clk   = 0;
 sdram_clk = 0;
end

// Clock generator
always #(P_SYS/2) sys_clk   = ~sys_clk;
always #(P_SDR/2) sdram_clk = ~sdram_clk;

// DUT connection
sdrc_top dut (

    cfg_sdr_width       ,
    cfg_colbits         ,

    // WB bus
    wb_rst_i            ,
    wb_clk_i            ,
    wb_stb_i            ,
    wb_ack_o            ,
    wb_addr_i           ,
    wb_we_i             ,
    wb_dat_i            ,
    wb_sel_i            ,
    wb_dat_o            ,
    wb_cyc_i            ,
    wb_cti_i            ,

		/* Interface to SDRAMs */
    sdram_clk           ,
    sdram_resetn        ,
    sdr_cs_n            ,
    sdr_cke             ,
    sdr_ras_n           ,
    sdr_cas_n           ,
    sdr_we_n            ,
    sdr_dqm             ,
    sdr_ba              ,
    sdr_addr            ,
    sdr_dq              ,

		/* Parameters */
    sdr_init_done       ,
    cfg_req_depth       ,   //how many req. buffer should hold
    cfg_sdr_en          ,
    cfg_sdr_mode_reg    ,
    cfg_sdr_tras_d      ,
    cfg_sdr_trp_d       ,
    cfg_sdr_trcd_d      ,
    cfg_sdr_cas         ,
    cfg_sdr_trcar_d     ,
    cfg_sdr_twr_d       ,
    cfg_sdr_rfsh        ,
	  cfg_sdr_rfmax
  );

endmodule
