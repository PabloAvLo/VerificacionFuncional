#ifndef SC_TB_TEST_H
#define SC_TB_TEST_H

#include "systemc.h"
#include "sc_define.c"

SC_MODULE (interface) {

    // Input Signals
    // WB bus
    sc_signal<bool>               wb_clk_i;
    sc_signal<bool>               wb_ack_o;
    sc_signal< sc_uint<dw> >      wb_dat_o;
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
    sc_signal< sc_uint<dw> >      wb_dat_i;
    sc_signal< sc_uint<dw/8> >    wb_sel_i;
    sc_signal<bool>               wb_cyc_i;
    sc_signal< sc_uint<3>         wb_cti_i;
    // Interface to SDRAM
    sc_signal<bool>               sdram_resetn;
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

  SC_CTOR(interface) {

  }

};

/*SC_MODULE (scoreboard) {

  sc_fifo<sc_uint<8> > fifo;

  SC_CTOR(scoreboard) {
    sc_fifo<sc_uint<8> > fifo (100); //FIXME this should be dynamic allocation.
  }
};*/

SC_MODULE (driver) {

  interface *intf_int;
  //scoreboard *scb_int;

  SC_HAS_PROCESS(driver);
  driver(sc_module_name driver, /*scoreboard *scb_ext, */interface *intf_ext) {
    //Interface
    intf_int = intf_ext;
    //Scoreboard
    //scb_int = scb_ext;
  }

  void reset();
  void write();
  void read();

};

SC_MODULE (monitor) {

  interface *intf_int;
  //scoreboard *scb_int;

  sc_uint<8> data_out_exp;
  sc_uint<8> data_out_read;

  SC_HAS_PROCESS(monitor);
  monitor(sc_module_name monitor, /*scoreboard *scb_ext, */interface *intf_ext) {
    //Interface
    intf_int=intf_ext
    //Scoreboard
    scb_int = scb_ext;
    SC_THREAD(mnt_out);
      //sensitive << intf_int->rd_en.pos();
  }

  void mnt_out();

};

SC_MODULE (environment) {

  driver *drv;
  monitor *mnt;
  //scoreboard *scb;

  SC_HAS_PROCESS(environment);
  environment(sc_module_name environment, interface *intf_ext) {

    //Scoreboard
    //scb = new scoreboard("scb");
    //Driver
    drv = new driver("drv",scb,intf_ext);
    //Monitor
    mnt = new monitor("mnt",scb,intf_ext);

  }
};

SC_MODULE (base_test) {

  interface *intf_int;
  environment *env;

  void test ();

  SC_HAS_PROCESS(base_test);
  base_test(sc_module_name base_test, interface *intf_ext) {
    intf_int = intf_ext;
    //environment
    env = new environment("env",intf_ext);
    SC_CTHREAD(test,intf_ext->wb_clk_i.pos());

  }
};

SC_MODULE (sc_tb) {

  base_test *test1;
  interface *intf;

  SC_CTOR(sc_tb) {
    intf = new interface("intf");
    test1 = new base_test("test1",intf);

  }
};

#endif
