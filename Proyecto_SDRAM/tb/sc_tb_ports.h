#ifndef SC_TB_PORTS_H
#define SC_TB_PORTS_H

// Define Complex Type of Input and Out for DUT
struct tagInput {
  // WB bus
  unsigned long wb_clk_i;
  unsigned long wb_ack_o;
  unsigned long wb_dat_o;

  // Interface to SDRAM
  unsigned long sdram_clk;
  unsigned long sdr_cs_n;
  unsigned long sdr_cke;
  unsigned long sdr_ras_n;
  unsigned long sdr_cas_n;
  unsigned long sdr_we_n;
  unsigned long sdr_dqm;
  unsigned long sdr_ba;
  unsigned long sdr_addr;
  unsigned long sdr_dq;
};

struct tagOutput {
  int done;

  // Config signals
  unsigned long cfg_sdr_width;
  unsigned long cfg_colbits;

  // WB bus
  unsigned long wb_rst_i;
  unsigned long wb_stb_i;
  unsigned long wb_addr_i;
  unsigned long wb_we_i;
  unsigned long wb_dat_i;
  unsigned long wb_sel_i;
  unsigned long wb_cyc_i;
  unsigned long wb_cti_i;

  // Interface to SDRAM
  unsigned long sdram_resetn;

  // Parameters
  unsigned long cfg_req_depth;
  unsigned long cfg_sdr_en;
  unsigned long cfg_sdr_mode_reg;
  unsigned long cfg_sdr_tras_d;
  unsigned long cfg_sdr_trp_d;
  unsigned long cfg_sdr_trcd_d;
  unsigned long cfg_sdr_cas;
  unsigned long cfg_sdr_trcar_d;
  unsigned long cfg_sdr_twr_d;
  unsigned long cfg_sdr_rfsh;
  unsigned long cfg_sdr_rfmax;
};

typedef struct tagInput    INVECTOR;
typedef struct tagOutput  OUTVECTOR;

#endif
