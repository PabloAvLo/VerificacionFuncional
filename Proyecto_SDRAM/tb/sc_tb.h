#ifndef SC_TB_TEST_H
#define SC_TB_TEST_H

#include "systemc.h"
#include "scv.h"
#include "sc_define.c"
#include <queue>
#include <string>

SC_MODULE (interface) {

    // Input Signals
    // WB bus
    sc_inout<bool>               wb_clk_i;
    sc_inout<bool>               wb_ack_o;
    sc_inout< sc_uint<DW> >      wb_dat_o;
    // Interface to SDRAM
    sc_inout<bool>               sdram_clk;
    sc_inout<bool>               sdr_cs_n;   // SDRAM Chip Select
    sc_inout<bool>               sdr_cke;    // SDRAM CKE
    sc_inout<bool>               sdr_ras_n;  // SDRAM ras
    sc_inout<bool>               sdr_cas_n;  // SDRAM cas
    sc_inout<bool>               sdr_we_n;   // SDRAM write enable
    sc_inout< sc_uint<SDR_BW> >  sdr_dqm;    // SDRAM Data Mask
    sc_inout< sc_uint<2> >       sdr_ba;     // SDRAM Bank Enable
    sc_inout< sc_uint<13> >      sdr_addr;   // SDRAM Address
    sc_inout< sc_uint<SDR_DW> >  sdr_dq;     // SDRA Data Output
    // Parameters
    sc_inout<bool>               sdr_init_done; // SDRAM initialized

    // Output Signals
    sc_out<bool>               done; //Terminate sim
    // Config signals
    /* 2'b00 - 32 Bit SDR, 2'b01 - 16 Bit SDR
       2'b1x - 8 Bit  */
    sc_out< sc_uint<2> >       cfg_sdr_width;
    /* 2'b00 - 8 Bit column address, 2'b01 - 9 Bit
       2'b10 - 10 bit, 2'b11 - 11Bits */
    sc_out< sc_uint<2> >       cfg_colbits;
    // WB bus
    sc_out<bool>               wb_rst_i;
    sc_out<bool>               wb_stb_i;
    sc_out< sc_uint<APP_AW> >  wb_addr_i;
    sc_out<bool>               wb_we_i;
    sc_out< sc_uint<DW> >      wb_dat_i;
    sc_out< sc_uint<DW/8> >    wb_sel_i;
    sc_out<bool>               wb_cyc_i;
    sc_out< sc_uint<3> >       wb_cti_i;
    // Interface to SDRAM
    sc_out<bool>               sdram_resetn;
    //sc_out< sc_uint<4> >       sdram_wr_en_n;
    // Parameters
    sc_out< sc_uint<2> >       cfg_req_depth;  // Maximum Request accepted by SDRAM controller
    sc_out<bool>               cfg_sdr_en;   // Enable SDRAM controller
    sc_out< sc_uint<13> >      cfg_sdr_mode_reg;
    sc_out< sc_uint<4> >       cfg_sdr_tras_d; // Active to precharge delay
    sc_out< sc_uint<4> >       cfg_sdr_trp_d;  // Precharge to active delay
    sc_out< sc_uint<4> >       cfg_sdr_trcd_d; // Active to R/W delay
    sc_out< sc_uint<3> >       cfg_sdr_cas;  // SDRAM CAS Latency
    sc_out< sc_uint<4> >       cfg_sdr_trcar_d;  // Auto-refresh period
    sc_out< sc_uint<4> >       cfg_sdr_twr_d; // Write recovery delay
    sc_out< sc_uint<SDR_RFSH_TIMER_W> >    cfg_sdr_rfsh;
    sc_out< sc_uint<SDR_RFSH_ROW_CNT_W> >  cfg_sdr_rfmax;

    int errCnt;

// Constructor
  SC_CTOR(interface){}
};


// ************************** SCOREBOARD
SC_MODULE (scoreboard) {

  sc_fifo<sc_uint<32> > addr_fifo;
  sc_fifo<sc_uint<8> > buLen_fifo;
  sc_fifo<sc_uint<32> > data_fifo;
  int n_cases;

  SC_CTOR(scoreboard) {
    sc_fifo<sc_uint<32> > addr_fifo (100); //n_cases);
    sc_fifo<sc_uint<8> > buLen_fifo (100); //n_cases);
    sc_fifo<sc_uint<32> > data_fifo (100); //n_cases);
  }

  // Can search address, data and burstLenghts
  int search(int value_to_search);
};


// ************************** DRIVER
SC_MODULE (driver) {

  interface *intf_int;
  scoreboard *scb_int;

  SC_HAS_PROCESS(driver);
  driver(sc_module_name driver, scoreboard *scb_ext, interface *intf_ext) {
    //Interface
    intf_int = intf_ext;
    //Scoreboard
    scb_int = scb_ext;
  }

  void config();
  void init();
  void reset();
  void write(sc_uint<32> address, sc_uint<8> burstLenght, sc_uint<32> data);
  void read(sc_uint<32> address, sc_uint<8> burstLenght);
};

//************************** CONSTRAINS
class data_rnd_constraint : public scv_constraint_base {
public:
  scv_smart_ptr< sc_uint<32> >  data;

  SCV_CONSTRAINT_CTOR(data_rnd_constraint) {
    // Soft Constraint
    SCV_SOFT_CONSTRAINT ( data() <= 32 ); // Max
    SCV_SOFT_CONSTRAINT ( data() >= 1 );   // Min
  }
};

class bl_rnd_constraint : public scv_constraint_base {
public:
  scv_smart_ptr< sc_uint<4> >   bl; //burst lenght

  SCV_CONSTRAINT_CTOR(bl_rnd_constraint) {
    // Hard Constraint: bl -> {1,2,4,8}
    SCV_CONSTRAINT ( bl() > 0 );
    SCV_CONSTRAINT ( bl() <= 8 );
    SCV_CONSTRAINT ( bl() != 3 );
    SCV_CONSTRAINT ( bl() != 5 );
    SCV_CONSTRAINT ( bl() != 6 );
    SCV_CONSTRAINT ( bl() != 7 );
  }
};

class addr_rnd_constraint : public scv_constraint_base {
public:
  scv_smart_ptr< sc_uint<32> >  addr;

  SCV_CONSTRAINT_CTOR(addr_rnd_constraint) {
    // Soft Constraint
    SCV_SOFT_CONSTRAINT ( addr() <= 400 ); // Max
    SCV_SOFT_CONSTRAINT ( addr() >= 200 );   // Min
  }
};

class wait_rnd_constraint : public scv_constraint_base {
public:
  scv_smart_ptr< sc_uint<4> >  wait;

  SCV_CONSTRAINT_CTOR(wait_rnd_constraint) {
    // Soft Constraint
    SCV_SOFT_CONSTRAINT ( wait() <= 12 ); // Max
    SCV_SOFT_CONSTRAINT ( wait() >= 4  ); // Min
  }
};

// ************************** SIGNAL GENERATOR
SC_MODULE (signal_generator) {

  interface *intf_int;

  SC_HAS_PROCESS(signal_generator);
  signal_generator(sc_module_name signal_generator, interface *intf_ext) {

    //Interface
    intf_int = intf_ext;
  }

  void init() {
      scv_random::set_global_seed(scv_random::pick_random_seed()); //FIXME: needs to come from test seed
  }

  sc_uint<32> data_rnd_gen() {
      data_rnd_constraint data_rnd ("data_rnd_constraint");
      data_rnd.next();
      return data_rnd.data.read();
  }

  sc_uint<4> bl_rnd_gen() {
      bl_rnd_constraint bl_rnd ("bl_rnd_constraint");
      bl_rnd.next();
      return bl_rnd.bl.read();
  }

  sc_uint<32> addr_rnd_gen() {
      addr_rnd_constraint addr_rnd ("addr_rnd_constraint");
      addr_rnd.next();
      return addr_rnd.addr.read();
  }

  sc_uint<4> wait_rnd_gen() {
      wait_rnd_constraint wait_rnd ("wait_rnd_constraint");
      wait_rnd.next();
      return wait_rnd.wait.read();
  }
};

// ************************** MONITOR
SC_MODULE (monitor) {

  interface *intf_int;

  sc_uint<8> data_out_exp;
  sc_uint<8> data_out_read;

  SC_HAS_PROCESS(monitor);
  monitor(sc_module_name monitor, interface *intf_ext) {
    //Interface
    intf_int=intf_ext;
    SC_THREAD(mnt_out);
    //sensitive << intf_int->rd_en.pos();
  }

  void mnt_out();

};

// ************************** CHECKER
SC_MODULE (checker) {

  interface *intf_int;
  scoreboard *scb_int;
  monitor *mnt_int;

  SC_HAS_PROCESS(checker);
  checker(sc_module_name checker, scoreboard *scb_ext, monitor *mnt_ext, interface *intf_ext) {
    //Interface
    intf_int=intf_ext;
    //Scoreboard
    scb_int = scb_ext;
    //Monitor
    mnt_int = mnt_ext;
  }

  void verify(int mnt_value, string pass_msg);

};


// ************************** ENVIROMENT
SC_MODULE (environment) {

  driver *drv;
  monitor *mnt;
  scoreboard *scb;
  signal_generator *sig_gen;
  interface *intf_int;
  checker *check;

  SC_HAS_PROCESS(environment);
  environment(sc_module_name environment, interface *intf_ext) {

    intf_int = intf_ext;
    //Scoreboard
    scb = new scoreboard("scb");
    //Driver
    drv = new driver("drv",scb,intf_ext);
    //Monitor
    mnt = new monitor("mnt",intf_ext);
    //Checker
    check = new checker("check",scb,mnt,intf_ext);
    // Signal Generator
    sig_gen = new signal_generator("sig_gen",intf_ext);

  }
};

// ************************** BASE TEST
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

// ************************** SYSTEM C TEST BENCH
SC_MODULE (sc_tb) {

  base_test *test1;
  interface *intf;

  SC_CTOR(sc_tb) {
    intf = new interface("intf");
    test1 = new base_test("test1",intf);

  }
};

#endif
