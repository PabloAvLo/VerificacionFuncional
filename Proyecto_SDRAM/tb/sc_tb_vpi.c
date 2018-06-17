#include "sc_define.c"
#include "sc_tb_ports.h"
#include "sc_tb_exports.h"
#include "strings.h"

// Instantiate Top-Level DUT
sc_tb    u_sc_tb("u_sc_tb");

  // Input Signals
  // WB bus
  sc_signal<bool>               wb_clk_i;
  sc_signal<bool>               wb_ack_o;
  sc_signal< sc_uint<DW> >      wb_dat_o;
  // Interface to SDRAM
  sc_signal<bool>               sdram_clk;
  sc_signal<bool>               sdr_cs_n;   // SDRAM Chip Select
  sc_signal<bool>               sdr_cke;    // SDRAM CKE
  sc_signal<bool>               sdr_ras_n;  // SDRAM ras
  sc_signal<bool>               sdr_cas_n;  // SDRAM cas
  sc_signal<bool>               sdr_we_n;   // SDRAM write enable
  sc_signal< sc_uint<SDR_BW> >  sdr_dqm;    // SDRAM Data Mask
  sc_signal< sc_uint<2> >       sdr_ba;     // SDRAM Bank Enable
  sc_signal< sc_uint<13> >      sdr_addr;   // SDRAM Address
  sc_signal< sc_uint<SDR_DW> >  sdr_dq;     // SDRA Data Output
  // Parameters
  // Indicate SDRAM initialization is done
  sc_signal<bool>               sdr_init_done;

  // Output Signals
  sc_signal<bool>               done; //Terminate sim
  // Config signals
  /* 2'b00 - 32 Bit SDR, 2'b01 - 16 Bit SDR
     2'b1x - 8 Bit  */
  sc_signal< sc_uint<2> >       cfg_sdr_width;
  /* 2'b00 - 8 Bit column address, 2'b01 - 9 Bit
     2'b10 - 10 bit, 2'b11 - 11Bits */
  sc_signal< sc_uint<2> >       cfg_colbits;
  // WB bus
  sc_signal<bool>               wb_rst_i;
  sc_signal<bool>               wb_stb_i;
  sc_signal< sc_uint<APP_AW> >  wb_addr_i;
  sc_signal<bool>               wb_we_i;
  sc_signal< sc_uint<DW> >      wb_dat_i;
  sc_signal< sc_uint<DW/8> >    wb_sel_i;
  sc_signal<bool>               wb_cyc_i;
  sc_signal< sc_uint<3> >       wb_cti_i;
  // Interface to SDRAM
  sc_signal<bool>               sdram_resetn;
  //sc_signal< sc_uint<4> >       sdram_wr_en_n;
  // Parameters
  sc_signal< sc_uint<2> >       cfg_req_depth;  // Maximum Request accepted by SDRAM controller
  sc_signal<bool>               cfg_sdr_en;   // Enable SDRAM controller
  sc_signal< sc_uint<13> >      cfg_sdr_mode_reg;
  sc_signal< sc_uint<4> >       cfg_sdr_tras_d; // Active to precharge delay
  sc_signal< sc_uint<4> >       cfg_sdr_trp_d;  // Precharge to active delay
  sc_signal< sc_uint<4> >       cfg_sdr_trcd_d; // Active to R/W delay
  sc_signal< sc_uint<3> >       cfg_sdr_cas;  // SDRAM CAS Latency
  sc_signal< sc_uint<4> >       cfg_sdr_trcar_d;  // Auto-refresh period
  sc_signal< sc_uint<4> >       cfg_sdr_twr_d; // Write recovery delay
  sc_signal< sc_uint<SDR_RFSH_TIMER_W> >    cfg_sdr_rfsh;
  sc_signal< sc_uint<SDR_RFSH_ROW_CNT_W> >  cfg_sdr_rfmax;


// Top-Level testbench
void init_sc() {

  // Input signals
  // WB bus
  u_sc_tb.intf->wb_clk_i(wb_clk_i);
  u_sc_tb.intf->wb_ack_o(wb_ack_o);
  u_sc_tb.intf->wb_dat_o(wb_dat_o);
  // Interface to SDRAM
  u_sc_tb.intf->sdram_clk(sdram_clk);
  u_sc_tb.intf->sdr_cs_n(sdr_cs_n);
  u_sc_tb.intf->sdr_cke(sdr_cke);
  u_sc_tb.intf->sdr_ras_n(sdr_ras_n);
  u_sc_tb.intf->sdr_cas_n(sdr_cas_n);
  u_sc_tb.intf->sdr_we_n(sdr_we_n);
  u_sc_tb.intf->sdr_dqm(sdr_dqm);
  u_sc_tb.intf->sdr_ba(sdr_ba);
  u_sc_tb.intf->sdr_addr(sdr_addr);
  u_sc_tb.intf->sdr_dq(sdr_dq);
  // Parameters
  u_sc_tb.intf->sdr_init_done(sdr_init_done);

  //Output Signals
  u_sc_tb.intf->done(done);
  // Config signals
  u_sc_tb.intf->cfg_sdr_width(cfg_sdr_width);
  u_sc_tb.intf->cfg_colbits(cfg_colbits);
  // WB bus
  u_sc_tb.intf->wb_rst_i(wb_rst_i);
  u_sc_tb.intf->wb_stb_i(wb_stb_i);
  u_sc_tb.intf->wb_addr_i(wb_addr_i);
  u_sc_tb.intf->wb_we_i(wb_we_i);
  u_sc_tb.intf->wb_dat_i(wb_dat_i);
  u_sc_tb.intf->wb_sel_i(wb_sel_i);
  u_sc_tb.intf->wb_cyc_i(wb_cyc_i);
  u_sc_tb.intf->wb_cti_i(wb_cti_i);
  // Interface to SDRAM
  u_sc_tb.intf->sdram_resetn(sdram_resetn);
  //u_sc_tb.intf->sdram_wr_en_n(sdram_wr_en_n);
  // Parameters
  u_sc_tb.intf->cfg_req_depth(cfg_req_depth);
  u_sc_tb.intf->cfg_sdr_en(cfg_sdr_en);
  u_sc_tb.intf->cfg_sdr_mode_reg(cfg_sdr_mode_reg);
  u_sc_tb.intf->cfg_sdr_tras_d(cfg_sdr_tras_d);
  u_sc_tb.intf->cfg_sdr_trp_d(cfg_sdr_trp_d);
  u_sc_tb.intf->cfg_sdr_trcd_d(cfg_sdr_trcd_d);
  u_sc_tb.intf->cfg_sdr_cas(cfg_sdr_cas);
  u_sc_tb.intf->cfg_sdr_trcar_d(cfg_sdr_trcar_d);
  u_sc_tb.intf->cfg_sdr_twr_d(cfg_sdr_twr_d);
  u_sc_tb.intf->cfg_sdr_rfsh(cfg_sdr_rfsh);
  u_sc_tb.intf->cfg_sdr_rfmax(cfg_sdr_rfmax);

  // Initialize
  sc_start();
  cout<<"@"<<sc_time_stamp()<<" Started SystemC Scheduler"<<endl;
}

void sample_hdl(void *Invector) {
  INVECTOR *pInvector = (INVECTOR *)Invector;
  // WB bus
  wb_clk_i.write(pInvector->wb_clk_i);
  wb_ack_o.write(pInvector->wb_ack_o);
  wb_dat_o.write(pInvector->wb_dat_o);
  // Interface to SDRAM
  sdram_clk.write(pInvector->sdram_clk);
  sdr_cs_n.write(pInvector->sdr_cs_n);
  sdr_cke.write(pInvector->sdr_cke);
  sdr_ras_n.write(pInvector->sdr_ras_n);
  sdr_cas_n.write(pInvector->sdr_cas_n);
  sdr_we_n.write(pInvector->sdr_we_n);
  sdr_dqm.write(pInvector->sdr_dqm);
  sdr_ba.write(pInvector->sdr_ba);
  sdr_addr.write(pInvector->sdr_addr);
  sdr_dq.write(pInvector->sdr_dq);
  // Parameters
  sdr_init_done.write(pInvector->sdr_init_done);
}

void drive_hdl(void *Outvector) {
  OUTVECTOR *pOutvector = (OUTVECTOR *)Outvector;
  pOutvector->done              = done.read();
  // Config signals
  pOutvector->cfg_sdr_width     = cfg_sdr_width.read();
  pOutvector->cfg_colbits       = cfg_colbits.read();
  // WB bus
  pOutvector->wb_rst_i          = wb_rst_i.read();
  pOutvector->wb_stb_i          = wb_stb_i.read();
  pOutvector->wb_addr_i         = wb_addr_i.read();
  pOutvector->wb_we_i           = wb_we_i.read();
  pOutvector->wb_dat_i          = wb_dat_i.read();
  pOutvector->wb_sel_i          = wb_sel_i.read();
  pOutvector->wb_cyc_i          = wb_cyc_i.read();
  pOutvector->wb_cti_i          = wb_cti_i.read();
  // Interface to SDRAM
  pOutvector->sdram_resetn      = sdram_resetn.read();
  //pOutvector->sdram_wr_en_n     = sdram_wr_en_n.read();
  // Parameters
  pOutvector->cfg_req_depth     = cfg_req_depth.read();
  pOutvector->cfg_sdr_en        = cfg_sdr_en.read();
  pOutvector->cfg_sdr_mode_reg  = cfg_sdr_mode_reg.read();
  pOutvector->cfg_sdr_tras_d    = cfg_sdr_tras_d.read();
  pOutvector->cfg_sdr_trp_d     = cfg_sdr_trp_d.read();
  pOutvector->cfg_sdr_trcd_d    = cfg_sdr_trcd_d.read();
  pOutvector->cfg_sdr_cas       = cfg_sdr_cas.read();
  pOutvector->cfg_sdr_trcar_d   = cfg_sdr_trcar_d.read();
  pOutvector->cfg_sdr_twr_d     = cfg_sdr_twr_d.read();
  pOutvector->cfg_sdr_rfsh      = cfg_sdr_rfsh.read();
  pOutvector->cfg_sdr_rfmax     = cfg_sdr_rfmax.read();
}

void advance_sim(sc_time simtime) {
  sc_start(simtime);
}

void exec_sc(void *invector, void *outvector, sc_time simtime) {
  sample_hdl(invector);    // Input-Stimuli
  advance_sim(simtime);   // Advance Simulator
  drive_hdl(outvector);  // Output Vectors
}

void exit_sc() {
  cout<<"@"<<sc_time_stamp()<<" Stopping SystemC Scheduler"<<endl;
  sc_stop();
}
