#include <stdio.h>
#include <stdlib.h>
#include "vpi_user.h"

#include "sc_tb_ports.h"
#include "sc_tb_exports.h"

// CallBack Proto
int sc_tb_calltf(char *user_data);
int sc_tb_interface(p_cb_data cb_data);

int sc_tb_calltf(char *user_data) {
  s_vpi_value  value_s;
  s_vpi_time  time_s;
  s_cb_data  cb_data_s;
  vpiHandle wb_clk_i;

  time_s.type         = vpiSuppressTime;
  value_s.format      = vpiIntVal;

  // Set-Up Value Change callback option
  cb_data_s.user_data = NULL;
  cb_data_s.reason    = cbValueChange;
  cb_data_s.cb_rtn    = sc_tb_interface;
  cb_data_s.time      = &time_s;
  cb_data_s.value     = &value_s;

  //Callback signal
  wb_clk_i = vpi_handle_by_name("tb.sys_clk", NULL);
  cb_data_s.obj  = wb_clk_i;
  vpi_register_cb(&cb_data_s);

  init_sc();  // Initialize SystemC Model

  return(0);

}

//Value change simulation callback routine
int sc_tb_interface(p_cb_data cb_data)
{
  s_vpi_value  value_s;
  s_vpi_time time;

  // IO ports systemC testbench
  static INVECTOR   invector;
  static OUTVECTOR  outvector;

  // Input Signals
  // WB bus
  vpiHandle wb_clk_i          = vpi_handle_by_name("tb.wb_clk_i", NULL);
  vpiHandle wb_ack_o          = vpi_handle_by_name("tb.wb_ack_o", NULL);
  vpiHandle wb_dat_o          = vpi_handle_by_name("tb.wb_dat_o", NULL);
  // Interface to SDRAM
  vpiHandle sdram_clk         = vpi_handle_by_name("tb.sdram_clk", NULL);
  vpiHandle sdr_cs_n          = vpi_handle_by_name("tb.sdr_cs_n", NULL);
  vpiHandle sdr_cke           = vpi_handle_by_name("tb.sdr_cke", NULL);
  vpiHandle sdr_ras_n         = vpi_handle_by_name("tb.sdr_ras_n", NULL);
  vpiHandle sdr_cas_n         = vpi_handle_by_name("tb.sdr_cas_n", NULL);
  vpiHandle sdr_we_n          = vpi_handle_by_name("tb.sdr_we_n", NULL);
  vpiHandle sdr_dqm           = vpi_handle_by_name("tb.sdr_dqm", NULL);
  vpiHandle sdr_ba            = vpi_handle_by_name("tb.sdr_ba", NULL);
  vpiHandle sdr_addr          = vpi_handle_by_name("tb.sdr_addr", NULL);
  vpiHandle sdr_dq            = vpi_handle_by_name("tb.sdr_dq", NULL);

  // Output Signals
  vpiHandle cfg_sdr_width     = vpi_handle_by_name("tb.cfg_sdr_width", NULL);
  vpiHandle cfg_colbits       = vpi_handle_by_name("tb.cfg_colbits", NULL);
  // WB bus
  vpiHandle wb_rst_i          = vpi_handle_by_name("tb.wb_rst_i", NULL);
  vpiHandle wb_stb_i          = vpi_handle_by_name("tb.wb_stb_i", NULL);
  vpiHandle wb_addr_i         = vpi_handle_by_name("tb.wb_addr_i", NULL);
  vpiHandle wb_we_i           = vpi_handle_by_name("tb.wb_we_i", NULL);
  vpiHandle wb_dat_i          = vpi_handle_by_name("tb.wb_dat_i", NULL);
  vpiHandle wb_sel_i          = vpi_handle_by_name("tb.wb_sel_i", NULL);
  vpiHandle wb_cyc_i          = vpi_handle_by_name("tb.wb_cyc_i", NULL);
  vpiHandle wb_cti_i          = vpi_handle_by_name("tb.wb_cti_i", NULL);
  // Interface to SDRAM
  vpiHandle sdram_resetn      = vpi_handle_by_name("tb.sdram_resetn", NULL);
  // Parameters
  vpiHandle cfg_req_depth     = vpi_handle_by_name("tb.cfg_req_depth", NULL);
  vpiHandle cfg_sdr_en        = vpi_handle_by_name("tb.cfg_sdr_en", NULL);
  vpiHandle cfg_sdr_mode_reg  = vpi_handle_by_name("tb.cfg_sdr_mode_reg", NULL);
  vpiHandle cfg_sdr_tras_d    = vpi_handle_by_name("tb.cfg_sdr_tras_d", NULL);
  vpiHandle cfg_sdr_trp_d     = vpi_handle_by_name("tb.cfg_sdr_trp_d", NULL);
  vpiHandle cfg_sdr_trcd_d    = vpi_handle_by_name("tb.cfg_sdr_trcd_d", NULL);
  vpiHandle cfg_sdr_cas       = vpi_handle_by_name("tb.cfg_sdr_cas", NULL);
  vpiHandle cfg_sdr_trcar_d   = vpi_handle_by_name("tb.cfg_sdr_trcar_d", NULL);
  vpiHandle cfg_sdr_twr_d     = vpi_handle_by_name("tb.cfg_sdr_twr_d", NULL);
  vpiHandle cfg_sdr_rfsh      = vpi_handle_by_name("tb.cfg_sdr_rfsh", NULL);
  vpiHandle cfg_sdr_rfmax     = vpi_handle_by_name("tb.cfg_sdr_rfmax", NULL);

  // Read current value from Verilog
  value_s.format = vpiIntVal;

  vpi_get_value(wb_clk_i, &value_s);
  invector.wb_clk_i = value_s.value.integer;

  vpi_get_value(wb_ack_o, &value_s);
  invector.wb_ack_o = value_s.value.integer;

  vpi_get_value(wb_dat_o, &value_s);
  invector.wb_dat_o = value_s.value.integer;

  vpi_get_value(sdram_clk, &value_s);
  invector.sdram_clk = value_s.value.integer;

  vpi_get_value(sdr_cs_n, &value_s);
  invector.sdr_cs_n = value_s.value.integer;

  vpi_get_value(sdr_cke, &value_s);
  invector.sdr_cke = value_s.value.integer;

  vpi_get_value(sdr_ras_n, &value_s);
  invector.sdr_ras_n = value_s.value.integer;

  vpi_get_value(sdr_cas_n, &value_s);
  invector.sdr_cas_n = value_s.value.integer;

  vpi_get_value(sdr_we_n, &value_s);
  invector.sdr_we_n = value_s.value.integer;

  vpi_get_value(sdr_dqm, &value_s);
  invector.sdr_dqm = value_s.value.integer;

  vpi_get_value(sdr_ba, &value_s);
  invector.sdr_ba = value_s.value.integer;

  vpi_get_value(sdr_addr, &value_s);
  invector.sdr_addr = value_s.value.integer;

  vpi_get_value(sdr_dq, &value_s);
  invector.sdr_dq = value_s.value.integer;

  sc_time sc_time_tmp (1, SC_NS);
  exec_sc(&invector, &outvector, sc_time_tmp);

  value_s.value.integer = outvector.cfg_sdr_width;
  vpi_put_value(cfg_sdr_width, &value_s, 0, vpiNoDelay);

  value_s.value.integer = outvector.cfg_colbits;
  vpi_put_value(cfg_colbits, &value_s, 0, vpiNoDelay);

  value_s.value.integer = outvector.wb_rst_i;
  vpi_put_value(wb_rst_i, &value_s, 0, vpiNoDelay);

  value_s.value.integer = outvector.wb_stb_i;
  vpi_put_value(wb_stb_i, &value_s, 0, vpiNoDelay);

  value_s.value.integer = outvector.wb_addr_i;
  vpi_put_value(wb_addr_i, &value_s, 0, vpiNoDelay);

  value_s.value.integer = outvector.wb_we_i;
  vpi_put_value(wb_we_i, &value_s, 0, vpiNoDelay);

  value_s.value.integer = outvector.wb_dat_i;
  vpi_put_value(wb_dat_i, &value_s, 0, vpiNoDelay);

  value_s.value.integer = outvector.wb_sel_i;
  vpi_put_value(wb_sel_i, &value_s, 0, vpiNoDelay);

  value_s.value.integer = outvector.wb_cyc_i;
  vpi_put_value(wb_cyc_i, &value_s, 0, vpiNoDelay);

  value_s.value.integer = outvector.wb_cti_i;
  vpi_put_value(wb_cti_i, &value_s, 0, vpiNoDelay);

  value_s.value.integer = outvector.sdram_resetn;
  vpi_put_value(sdram_resetn, &value_s, 0, vpiNoDelay);

  value_s.value.integer = outvector.cfg_req_depth;
  vpi_put_value(cfg_req_depth, &value_s, 0, vpiNoDelay);

  value_s.value.integer = outvector.cfg_sdr_en;
  vpi_put_value(cfg_sdr_en, &value_s, 0, vpiNoDelay);

  value_s.value.integer = outvector.cfg_sdr_mode_reg;
  vpi_put_value(cfg_sdr_mode_reg, &value_s, 0, vpiNoDelay);

  value_s.value.integer = outvector.cfg_sdr_tras_d;
  vpi_put_value(cfg_sdr_tras_d, &value_s, 0, vpiNoDelay);

  value_s.value.integer = outvector.cfg_sdr_trp_d;
  vpi_put_value(cfg_sdr_trp_d, &value_s, 0, vpiNoDelay);

  value_s.value.integer = outvector.cfg_sdr_trcd_d;
  vpi_put_value(cfg_sdr_trcd_d, &value_s, 0, vpiNoDelay);

  value_s.value.integer = outvector.cfg_sdr_cas;
  vpi_put_value(cfg_sdr_cas, &value_s, 0, vpiNoDelay);

  value_s.value.integer = outvector.cfg_sdr_trcar_d;
  vpi_put_value(cfg_sdr_trcar_d, &value_s, 0, vpiNoDelay);

  value_s.value.integer = outvector.cfg_sdr_twr_d;
  vpi_put_value(cfg_sdr_twr_d, &value_s, 0, vpiNoDelay);

  value_s.value.integer = outvector.cfg_sdr_rfsh;
  vpi_put_value(cfg_sdr_rfsh, &value_s, 0, vpiNoDelay);

  value_s.value.integer = outvector.cfg_sdr_rfmax;
  vpi_put_value(cfg_sdr_rfmax, &value_s, 0, vpiNoDelay);

  if (outvector.done) {

     exit_sc();

     vpi_control(vpiFinish);
  }
  return(0);
}

void sc_tb()
{
	s_vpi_systf_data tf;

	tf.type		=	vpiSysTask;
	tf.tfname	=	"$sc_tb";
	tf.calltf	=	sc_tb_calltf;
	tf.compiletf	=	NULL;
	tf.sizetf	=	0;
	tf.user_data	=	0;

	vpi_register_systf(&tf);
}

void (*vlog_startup_routines[])() = {
	sc_tb,
	0
};
