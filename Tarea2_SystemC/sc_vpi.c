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
  vpiHandle clk;

  time_s.type         = vpiSuppressTime;
  value_s.format      = vpiIntVal;

  // Set-Up Value Change callback option
  cb_data_s.user_data = NULL;
  cb_data_s.reason    = cbValueChange;
  cb_data_s.cb_rtn    = sc_tb_interface;
  cb_data_s.time      = &time_s;
  cb_data_s.value     = &value_s;

  //Callback signal
  clk = vpi_handle_by_name("tb.clk", NULL);
  cb_data_s.obj  = clk;
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

  vpiHandle rst   = vpi_handle_by_name("tb.rst", NULL);
  vpiHandle clk   = vpi_handle_by_name("tb.clk", NULL);
  vpiHandle w = vpi_handle_by_name("tb.w", NULL);
  vpiHandle z = vpi_handle_by_name("tb.z", NULL);

  // Read current value from Verilog
  value_s.format = vpiIntVal;

  vpi_get_value(clk, &value_s);
  invector.clk = value_s.value.integer;

  sc_time sc_time_tmp (1, SC_NS);
  exec_sc(&invector, &outvector, sc_time_tmp);

  vpi_get_value(z, &value_s);
  invector.z = value_s.value.integer;

  value_s.value.integer = outvector.rst;
  vpi_put_value(rst, &value_s, 0, vpiNoDelay);

  value_s.value.integer = outvector.w;
  vpi_put_value(w, &value_s, 0, vpiNoDelay);

  if (outvector.done) {

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
