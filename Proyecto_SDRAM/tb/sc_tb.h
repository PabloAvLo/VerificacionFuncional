#ifndef SC_TB_TEST_H
#define SC_TB_TEST_H

#include "systemc.h"
#include "scv.h"
#include "sc_define.c"

// **************** INTERFACE  **************** //
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

// ************ FUNCTIONAL COVERAGE *********** //
SC_MODULE (functional_cov) {

  interface *intf_int;
  int bank0;
  int bank1;
  int bank2;
  int bank3;
  int rst_cnt;

  SC_HAS_PROCESS(functional_cov);
  functional_cov(sc_module_name cov, interface *intf_ext) {

    bank0 = 0;
    bank1 = 0;
    bank2 = 0;
    bank3 = 0;
    rst_cnt = 0;

    //Interface
    intf_int = intf_ext;

    SC_THREAD(bank_cov)
      sensitive<<intf_ext->wb_cyc_i.pos();

    SC_THREAD(rst_cov)
      sensitive<<intf_ext->wb_rst_i.pos();
  }

  void bank_cov();
  void print_cov();
  void rst_cov();
};


// **************** SCOREBOARD *************** //
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
};

// ***************** CONSTRAINS ***************** //
// Data Constrains
class data_rnd_constraint : public scv_constraint_base {
public:
  scv_smart_ptr< sc_uint<32> >  data;

  SCV_CONSTRAINT_CTOR(data_rnd_constraint) {
    // Soft Constraint
    SCV_SOFT_CONSTRAINT ( data() <= MAX_DATA );
    SCV_SOFT_CONSTRAINT ( data() >= MIN_DATA );
  }
};

// Burst length constrains
class bl_rnd_constraint : public scv_constraint_base {
public:
  scv_smart_ptr< sc_uint<4> >   bl; //burst lenght

  SCV_CONSTRAINT_CTOR(bl_rnd_constraint) {
    // Hard Constraint: bl -> {1,2,4,8}
    // bl->keep_only(MIN_BL, MAX_BL);
    // bl->keep_out(LOW_LIM_BL, HIGH_LIM_BL);
    // SCV_CONSTRAINT ( bl() != NO_VALID_BL );
    SCV_CONSTRAINT ( bl() >= MIN_BL );
    SCV_CONSTRAINT ( bl() <= MAX_BL );
    SCV_CONSTRAINT ( bl() != NO_VALID_BL1 );
    SCV_CONSTRAINT ( bl() != NO_VALID_BL2 );
    SCV_CONSTRAINT ( bl() != NO_VALID_BL3 );
    SCV_CONSTRAINT ( bl() != NO_VALID_BL4 );
  }
};

// Address constrains
class addr_rnd_constraint : public scv_constraint_base {
public:
  scv_smart_ptr< sc_uint<32> >  addr;

  SCV_CONSTRAINT_CTOR(addr_rnd_constraint) {
    // Soft Constraint
    SCV_SOFT_CONSTRAINT ( addr() <= MAX_ADDR );
    SCV_SOFT_CONSTRAINT ( addr() >= MIN_ADDR );
  }
};

// Time constrains
class wait_rnd_constraint : public scv_constraint_base {
public:
  scv_smart_ptr< sc_uint<4> >  cyc;

  SCV_CONSTRAINT_CTOR(wait_rnd_constraint) {
    // Soft Constraint
    SCV_SOFT_CONSTRAINT ( cyc() <= MAX_CYC );
    SCV_SOFT_CONSTRAINT ( cyc() >= MIN_CYC );
  }
};

// ***************** SIGNAL GENERATOR ***************** //
SC_MODULE (signal_generator) {

  SC_HAS_PROCESS(signal_generator);
  signal_generator(sc_module_name signal_generator) {
  }

  // Sets global seed
  void init(unsigned long long seed) {
      scv_random::set_global_seed(seed);
  }

  // Generates random data
  sc_uint<32> data_rnd_gen() {
      data_rnd_constraint data_rnd ("data_rnd_constraint");
      data_rnd.next();
      return data_rnd.data.read();
  }

  // Generates random burst length
  sc_uint<4> bl_rnd_gen() {
      bl_rnd_constraint bl_rnd ("bl_rnd_constraint");
      bl_rnd.next();
      return bl_rnd.bl.read();
  }

  // Generates random address
  sc_uint<32> addr_rnd_gen() {
      addr_rnd_constraint addr_rnd ("addr_rnd_constraint");
      addr_rnd.next();
      return addr_rnd.addr.read();
  }

  // Generates random cycles to wait
  sc_uint<4> wait_rnd_gen() {
      wait_rnd_constraint wait_rnd ("wait_rnd_constraint");
      wait_rnd.next();
      return wait_rnd.cyc.read();
  }
};


// **************** MONITOR **************** //
SC_MODULE (monitor) {

  interface *intf_int;

  sc_uint<8> data_out_exp;
  sc_uint<8> data_out_read;
  bool initFlag;
  bool readFlag;
  bool writeFlag;
  int AutoRef_counter;
  int readCounter;
  int writeCounter;


  SC_HAS_PROCESS(monitor);
  monitor(sc_module_name monitor, interface *intf_ext) {
    //Interface
    intf_int = intf_ext;
    AutoRef_counter = 0;
    readCounter     = 0;
    writeCounter    = 0;
    initFlag        = 0;
    readFlag        = 0;
    writeFlag       = 0;

    SC_METHOD(mnt_out);
    sensitive << intf_int->sdr_init_done.pos();
    sensitive << intf_int->wb_clk_i.pos();
    sensitive << intf_int->done.pos();
  }
  void mnt_out();
};

// **************** CHECKER **************** //
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

  void verify(int type,sc_dt::sc_uint<32> mnt_value);

};

// **************** DRIVER **************** //
SC_MODULE (driver) {

  interface *intf_int;
  scoreboard *scb_int;
  signal_generator *sig_gen;
  checker *check;

  SC_HAS_PROCESS(driver);
  driver(sc_module_name driver, scoreboard *scb_ext, interface *intf_ext) {

    //Interface
    intf_int = intf_ext;
    //Scoreboard
    scb_int = scb_ext;
  }

  void config();
  void init(unsigned long long);
  void reset();
  void write(sc_uint<32> address, sc_uint<8> burstLenght, sc_uint<32> data);
  void rnd_write();
  void read(sc_uint<32> address, sc_uint<8> burstLenght);
  void seq_read();
};

// **************** ENVIROMENT **************** //
SC_MODULE (environment) {

  driver *drv;
  monitor *mnt;
  scoreboard *scb;
  interface *intf_int;
  checker *check;
  functional_cov *cov;

  SC_HAS_PROCESS(environment);
  environment(sc_module_name environment, interface *intf_ext) {

    intf_int = intf_ext;
    // Scoreboard
    scb = new scoreboard("scb");
    // Monitor
    mnt = new monitor("mnt",intf_ext);
    // Checker
    //check = new checker("check",scb,mnt,intf_ext);
    // Driver
    drv = new driver("drv",scb,intf_ext);
    // Functional Coverage
    cov = new functional_cov("cov",intf_ext);

  }
};

// **************** BASE TEST **************** //
SC_MODULE (base_test) {

  interface *intf_int;
  environment *env;
  string name;

  void test ();

  SC_HAS_PROCESS(base_test);
  base_test(sc_module_name base_test, interface *intf_ext) {
    intf_int = intf_ext;
    //environment
    name = "BASE TEST";
    env = new environment("env",intf_ext);
    SC_CTHREAD(test,intf_ext->wb_clk_i.pos());

  }
};

// ************* BASIC FUNCTIONALITY *************** //
SC_MODULE (basic_func) {

  interface *intf_int;
  environment *env;
  string name;

  void test ();

  SC_HAS_PROCESS(basic_func);
  basic_func(sc_module_name basic_func, interface *intf_ext) {
    intf_int = intf_ext;
    //environment
    name = "BASIC FUNCTIONALITY";
    env = new environment("env",intf_ext);
    SC_CTHREAD(test,intf_ext->wb_clk_i.pos());

  }
};

// ************* READ AFTER RESET *************** //
SC_MODULE (rd_after_rst) {

  interface *intf_int;
  environment *env;
  string name;

  void test ();

  SC_HAS_PROCESS(rd_after_rst);
  rd_after_rst(sc_module_name rd_after_rst, interface *intf_ext) {
    intf_int = intf_ext;
    //environment
    name = "READ AFTER RESET";
    env = new environment("env",intf_ext);
    SC_CTHREAD(test,intf_ext->wb_clk_i.pos());

  }
};

// ************* OVERWRITE DATA *************** //
SC_MODULE (overwrite) {

  interface *intf_int;
  environment *env;
  string name;

  void test ();

  SC_HAS_PROCESS(overwrite);
  overwrite(sc_module_name overwrite, interface *intf_ext) {
    intf_int = intf_ext;
    //environment
    name = "OVERWRITE DATA";
    env = new environment("env",intf_ext);
    SC_CTHREAD(test,intf_ext->wb_clk_i.pos());

  }
};

// ************* CROSSOVER *************** //
SC_MODULE (cross_over) {

  interface *intf_int;
  environment *env;
  string name;

  void test ();

  SC_HAS_PROCESS(cross_over);
  cross_over(sc_module_name cross_over, interface *intf_ext) {
    intf_int = intf_ext;
    //environment
    name = "CROSSOVER";
    env = new environment("env",intf_ext);
    SC_CTHREAD(test,intf_ext->wb_clk_i.pos());

  }
};

// ********** RANDOM WRITE/READ *********** //
SC_MODULE (rnd_wr_rd) {

  interface *intf_int;
  environment *env;
  string name;

  void test ();

  SC_HAS_PROCESS(rnd_wr_rd);
  rnd_wr_rd(sc_module_name rnd_wr_rd, interface *intf_ext) {
    intf_int = intf_ext;
    //environment
    name = "RANDOM WRITE/READ";
    env = new environment("env",intf_ext);
    SC_CTHREAD(test,intf_ext->wb_clk_i.pos());

  }
};

// ******** USAGE OF 4 BANKS ********* //
SC_MODULE (usg_4_banks) {

  interface *intf_int;
  environment *env;
  string name;

  void test ();

  SC_HAS_PROCESS(usg_4_banks);
  usg_4_banks(sc_module_name usg_4_banks, interface *intf_ext) {
    intf_int = intf_ext;
    //environment
    name = "USAGE OF 4 BANKS";
    env = new environment("env",intf_ext);
    SC_CTHREAD(test,intf_ext->wb_clk_i.pos());

  }
};

// *********** SYSTEM C TEST BENCH ************ //
SC_MODULE (sc_tb) {

  base_test     *test1;
  basic_func    *test2;
  rd_after_rst  *test3;
  overwrite     *test4;
  cross_over    *test5;
  rnd_wr_rd     *test6;
  usg_4_banks   *test7;
  interface     *intf;

  SC_CTOR(sc_tb) {
    intf  = new interface("intf");
    // test1 = new base_test("test1",intf);
    test2 = new basic_func("test2",intf);
    // test3 = new rd_after_rst("test3",intf);
    // test4 = new overwrite("test4",intf);
    // test5 = new cross_over("test5",intf);
    // test6 = new rnd_wr_rd("test6",intf);
    // test7 = new usg_4_banks("test7",intf);
  }
};

#endif
