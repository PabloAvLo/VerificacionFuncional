#include "systemc.h"
#include "sc_tb.h"

#include "sc_tb_ports.h"
#include "sc_tb_exports.h"
#include "strings.h"

// Instantiate Top-Level DUT
sc_tb  u_sc_tb("u_sc_tb");

// Declare Signals
sc_signal<bool>    clk;
sc_signal<bool>    rst;
sc_signal<bool>    w;
sc_signal<bool>    z;
sc_signal<int>     done;

// Top-Level testbench
void init_sc() {
  // Port mapping
  u_sc_tb.clk(clk);
  u_sc_tb.rst(rst);
  u_sc_tb.w(w);
  u_sc_tb.z(z);
  u_sc_tb.done(done);
  // VCD Tracing:
  sc_trace_file* tf;
  tf = sc_create_vcd_trace_file("sc_tb");
  tf->set_time_unit(1, SC_NS);

  //   Signals to be traced
  sc_trace(tf, clk, "clk");
  sc_trace(tf, rst, "rst");
  sc_trace(tf, w, "w");
  sc_trace(tf, z, "z");

  // Initialize SC
  sc_start();

  cout<<"@"<<sc_time_stamp()<<" Started SystemC Scheduler"<<endl;
}

void sample_hdl(void *Invector) {
  INVECTOR *pInvector = (INVECTOR *)Invector;
  clk.write(pInvector->clk);
  z.write(pInvector->z);
}

void drive_hdl(void *Outvector) {
  OUTVECTOR *pOutvector = (OUTVECTOR *)Outvector;
  pOutvector->rst  =  rst.read()? 1 : 0;
  pOutvector->w    =  w.read()?   1 : 0;
  pOutvector->done =  done;
}

//void advance_sim(unsigned long simtime) { commented out by gjcastro
void advance_sim(sc_time simtime) {
  sc_start(simtime);
}

//void exec_sc(void *invector, void *outvector, unsigned long simtime) {
void exec_sc(void *invector, void *outvector, sc_time simtime) {
  sample_hdl(invector);    // Input-Stimuli
  advance_sim(simtime);   // Advance Simulator
  drive_hdl(outvector);  // Output Vectors
}

void exit_sc() {
  cout<<"@"<<sc_time_stamp()<<" Stopping SystemC Scheduler"<<endl;
  sc_stop();
}
