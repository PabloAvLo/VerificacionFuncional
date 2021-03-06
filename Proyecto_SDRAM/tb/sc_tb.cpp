#include "sc_tb.h"

// ************** DRIVER CONFIG ************** //
void driver::config() {
    intf_int->cfg_sdr_width    = 0b10;
    intf_int->cfg_colbits      = 0b00;
    intf_int->cfg_req_depth    = 0x3;
    intf_int->cfg_sdr_en       = 0b1;
    intf_int->cfg_sdr_mode_reg = 0x033;
    intf_int->cfg_sdr_tras_d   = 0x4;
    intf_int->cfg_sdr_trp_d    = 0x2;
    intf_int->cfg_sdr_trcd_d   = 0x2;
    intf_int->cfg_sdr_cas      = 0x3;
    intf_int->cfg_sdr_trcar_d  = 0x7;
    intf_int->cfg_sdr_twr_d    = 0x1;
    intf_int->cfg_sdr_rfsh     = 0x100;
    intf_int->cfg_sdr_rfmax    = 0x6;
    wait(1);
}

// ************** DRIVER TOGGLE ************** //
void driver::toggle() {
    intf_int->cfg_sdr_width    = 0b0;
    intf_int->cfg_colbits      = 0b00;
    intf_int->cfg_req_depth    = 0;
    intf_int->cfg_sdr_en       = 0;
    intf_int->cfg_sdr_mode_reg = 0x0;
    intf_int->cfg_sdr_tras_d   = 0x0;
    intf_int->cfg_sdr_trp_d    = 0;
    intf_int->cfg_sdr_trcd_d   = 0;
    intf_int->cfg_sdr_cas      = 0;
    intf_int->cfg_sdr_trcar_d  = 0;
    intf_int->cfg_sdr_twr_d    = 0;
    intf_int->cfg_sdr_rfsh     = 0x00;
    intf_int->cfg_sdr_rfmax    = 0;
    wait(2);
    intf_int->cfg_sdr_width    = 0b11;
    intf_int->cfg_colbits      = 0b11;
    intf_int->cfg_req_depth    = 0b11;
    intf_int->cfg_sdr_en       = 1;
    intf_int->cfg_sdr_mode_reg = 0x1fff;
    intf_int->cfg_sdr_tras_d   = 0xf;
    intf_int->cfg_sdr_trp_d    = 0xf;
    intf_int->cfg_sdr_trcd_d   = 0xf;
    intf_int->cfg_sdr_cas      = 0b11;
    intf_int->cfg_sdr_trcar_d  = 0xf;
    intf_int->cfg_sdr_twr_d    = 0xf;
    intf_int->cfg_sdr_rfsh     = 0xfff;
    intf_int->cfg_sdr_rfmax    = 0b11;
    wait(2);
}

// ************** DRIVER INIT **************** //
void driver::init(unsigned long long seed) {
    driver::config();
    sig_gen->init(seed);

    //Clean some signals
    intf_int->wb_addr_i      = 0;
    intf_int->wb_dat_i       = 0;
    intf_int->wb_sel_i       = 0x0;
    intf_int->wb_we_i        = false;
    intf_int->wb_stb_i       = 0;
    intf_int->wb_cyc_i       = 0;
    intf_int->wb_rst_i       = 0;
    intf_int->sdram_resetn   = 1;
    intf_int->errCnt         = 0;

    // Reset
    reset();
}

// ************** DRIVER RESET **************** //
void driver::reset() {
  wait(10);

  // Applying reset
  intf_int->wb_rst_i       = 1;
  intf_int->sdram_resetn   = 0;
  wait(1000);

  // Releasing reset
  intf_int->wb_rst_i       = 0;
  intf_int->sdram_resetn   = 1;
  wait(100);

  while(intf_int->sdr_init_done == 0) {
      wait(1);
  }
  wait(100);
}

// **************** DRIVER WRITE  ************** //
void driver::write(sc_uint<32> address, sc_uint<8> burstLenght, sc_uint<32> data) {

  scb_int->addr_fifo.write(address);
  scb_int->buLen_fifo.write(burstLenght);
  scb_int->data_fifo.write(data);

  wait(5, SC_NS);
  cout<<"Write Address: "<< address <<", Burst Size: "<< burstLenght <<endl;
  for(int i=0; i < burstLenght; i++){
    intf_int->wb_stb_i        = true;
    intf_int->wb_cyc_i        = true;
    intf_int->wb_we_i         = true;
    intf_int->wb_sel_i        = 0b1111;
    intf_int->wb_addr_i       = (address >> 2) + i; // Address[31:2]+i;
    intf_int->wb_dat_i        = data;

    wait(1);
    while(intf_int->wb_ack_o == 0b0) {
        wait(1);
    }
    wait(5, SC_NS);

    cout<<"@"<<sc_time_stamp()<<" Burst-No: "<< i <<", Write Address: "<<
    intf_int->wb_addr_i <<", Write Data: "<< intf_int->wb_dat_i << endl;
  }

  wait(10);

  intf_int->wb_stb_i        = false;
  intf_int->wb_cyc_i        = false;
  intf_int->wb_we_i         = false; //'hx;
  intf_int->wb_sel_i        = 0; //'hx;
  intf_int->wb_addr_i       = 0x3FFFFFFF; //'hx;
  intf_int->wb_dat_i        = 0xFF; //'hx;
}

// *********** DRIVER RANDOM WRITE  *********** //
void driver::rnd_write() {

  sc_uint<32>   address = sig_gen->addr_rnd_gen();
  sc_uint<8>    burstLenght = sig_gen->bl_rnd_gen();

  scb_int->addr_fifo.write(address);
  scb_int->buLen_fifo.write(burstLenght);

  wait(5, SC_NS);
  cout<<"Write Address: "<< address <<", Burst Size: "<< burstLenght <<endl;
  for(int i=0; i < burstLenght; i++){
    intf_int->wb_stb_i        = true;
    intf_int->wb_cyc_i        = true;
    intf_int->wb_we_i         = true;
    intf_int->wb_sel_i        = 0b1111;
    intf_int->wb_addr_i       = (address >> 2) + i; // Address[31:2]+i;
    intf_int->wb_dat_i        = sig_gen->data_rnd_gen();

    wait(1);
    while(intf_int->wb_ack_o == 0b0) {
        wait(1);
    }
    wait(5, SC_NS);
    scb_int->data_fifo.write(intf_int->wb_dat_i);

    cout<<"@"<<sc_time_stamp()<<" Burst-No: "<< i <<", Write Address: "<<
    intf_int->wb_addr_i <<", Write Data: "<< intf_int->wb_dat_i << endl;
  }

  wait(10);

  intf_int->wb_stb_i        = false;
  intf_int->wb_cyc_i        = false;
  intf_int->wb_we_i         = false; //'hx;
  intf_int->wb_sel_i        = 0; //'hx;
  intf_int->wb_addr_i       = 0x3FFFFFFF; //'hx;
  intf_int->wb_dat_i        = 0xFF; //'hx;
}

// ******************* DRIVER READ *************** //
void driver::read(sc_uint<32> address, sc_uint<8> burstLenght) {

  wait(5, SC_NS);
  cout<<"Read Address: "<< address <<", Burst Size: "<< burstLenght <<endl;

  for(int j=0; j < burstLenght; j++) {
    intf_int->wb_stb_i        = true;
    intf_int->wb_cyc_i        = true;
    intf_int->wb_we_i         = false;
    intf_int->wb_addr_i       = (address >> 2) + j; // Address[31:2]+j;

    wait(1);
    while(intf_int->wb_ack_o == 0b0) {
        wait(1);
    }

    // It is neccesary to do this 2 times due to systemC sync problems
    wait(1);
    while(intf_int->wb_ack_o == 0b0) {
        wait(1);
    }

    cout<<"@"<<sc_time_stamp()<<"Burst-No: "<< j <<", Read Address: "<<
    intf_int->wb_addr_i <<", Read Data: "<< intf_int->wb_dat_o << endl;
    //check->verify(0, address);
    //check->verify(1, intf_int->wb_dat_o);
    wait(5, SC_NS);
  }

  intf_int->wb_stb_i        = false;
  intf_int->wb_cyc_i        = false;
  intf_int->wb_we_i         = true; //'hx;
  intf_int->wb_addr_i       = 0x3FFFFFFF; //'hx;
}

// *********** DRIVER SEQUENTIAL READ ************ //
void driver::seq_read() {

  sc_uint<32> address = scb_int->addr_fifo.read();
  sc_uint<8>  burstLenght = scb_int->buLen_fifo.read();

  wait(5, SC_NS);
  cout<<"Read Address: "<< address <<", Burst Size: "<< burstLenght <<endl;

  for(int j=0; j < burstLenght; j++) {
    intf_int->wb_stb_i        = true;
    intf_int->wb_cyc_i        = true;
    intf_int->wb_we_i         = false;
    intf_int->wb_addr_i       = (address >> 2) + j; // Address[31:2]+j;

    wait(1);
    while(intf_int->wb_ack_o == 0b0) {
        wait(1);
    }

    // It is neccesary to do this 2 times due to systemC sync problems
    wait(1);
    while(intf_int->wb_ack_o == 0b0) {
        wait(1);
    }

    cout<<"@"<<sc_time_stamp()<<" Burst-No: "<< j <<", Read Address: "<<
    intf_int->wb_addr_i <<", Read Data: "<< intf_int->wb_dat_o << endl;
    //check->verify(1,intf_int->wb_dat_o);
    wait(5, SC_NS);
  }

  intf_int->wb_stb_i        = false;
  intf_int->wb_cyc_i        = false;
  intf_int->wb_we_i         = true; //'hx;
  intf_int->wb_addr_i       = 0x3FFFFFFF; //'hx;
}

// **************** MONITOR **************** //
void monitor::mnt_out(){

  // wb_dat_i, wb_dat_o, wb_addr_i, wb_ack_o,
  // sdr_addr, sdr_dq, app_req*, sdr_dout* (sdr_dq), sdr_den_n*,

  if(intf_int->sdr_init_done && !initFlag){
    cout<<"@"<<sc_time_stamp()<<" MONITOR: READY FLAG" << intf_int->sdr_init_done << endl;
    initFlag = 1;
  }
  else if(!(intf_int->sdr_init_done) && initFlag){
    initFlag = 0;
  }

  if (!intf_int->sdr_ras_n && !intf_int->sdr_cas_n && intf_int->sdr_we_n){
    cout<<"@"<<sc_time_stamp()<<" MONITOR: AUTOREFRESH DONE"<< endl;
    AutoRef_counter +=1;
  }
  if (intf_int->sdr_ras_n && !intf_int->sdr_cas_n && intf_int->sdr_we_n && !readFlag){
    cout<<"@"<<sc_time_stamp()<<" MONITOR: SDRAM READ DONE"<< endl;
    readCounter +=1;
    readFlag = 1;
  }
  else if(!(intf_int->sdr_we_n) && readFlag){
    readFlag = 0;
  }

  if (intf_int->sdr_ras_n && !intf_int->sdr_cas_n && !intf_int->sdr_we_n  && !writeFlag){
    cout<<"@"<<sc_time_stamp()<<" MONITOR: SDRAM WRITE DONE"<< endl;
    writeCounter +=1;
    writeFlag = 1;
  }
  else if(intf_int->sdr_we_n && writeFlag){
    writeFlag = 0;
  }

  if(intf_int->done){
    cout<<"Event: AutoRefresh            = "<<AutoRef_counter<<endl;
    cout<<"Event: Read                   = "<<readCounter<<endl;
    cout<<"Event: Write                  = "<<writeCounter<<endl;
    cout<< "//////////////////////////////////////////////////////////" << endl << endl;
  }
}

// **************** CHECKER **************** //
/*
type 0 : Address
type 1 : Data
*/
void checker::verify(int type,sc_dt::sc_uint<32> mnt_value){
  sc_dt::sc_uint<32> scb_value;
  if(type == 0){
    scb_value = scb_int->addr_fifo.read();
  }
  else if(type ==1){
    scb_value = scb_int->data_fifo.read();
  }

  if(mnt_value == scb_value){
    cout<< "PASS: Monitor value "<< mnt_value << "matches with expected scoreboard value "<<scb_value <<endl;
  }
  else{
    cout<< "FAIL: Monitor value "<< mnt_value << "does not match with expected scoreboard value "<<scb_value <<endl;
    intf_int->errCnt += 1;
  }
}

// ************ BANK COVERAGE ************ //
void functional_cov::bank_cov(){
  while(true){
    wait(1);
    if (((sc_uint<26>)intf_int->wb_addr_i & (sc_uint<26>)0x0000300)>>8 == 0) {
      bank0++;
      cout<<"@"<<sc_time_stamp()<<" Read/Write in bank 1" << endl;
    }
    else if (((sc_uint<26>)intf_int->wb_addr_i & (sc_uint<26>)0x0000300)>>8 == 1) {
      bank1++;
      cout<<"@"<<sc_time_stamp()<<" Read/Write in bank 2" << endl;
    }
    else if (((sc_uint<26>)intf_int->wb_addr_i & (sc_uint<26>)0x0000300)>>8 == 2) {
      bank2++;
      cout<<"@"<<sc_time_stamp()<<" Read/Write in bank 3" << endl;
    }
    else if (((sc_uint<26>)intf_int->wb_addr_i & (sc_uint<26>)0x0000300)>>8 == 3) {
      bank3++;
      cout<<"@"<<sc_time_stamp()<<" Read/Write in bank 4" << endl;
    }
  }
}

// ************ RESET COVERAGE ************ //
void functional_cov::rst_cov(){
  while(true){
    wait(1);
    rst_cnt++;
  }
}

// ********* PRINT COVERAGE INFO ********* //
void functional_cov::print_cov(){

  cout <<endl<< "////////////// FUNCTIONAL COVERAGE RESULTS //////////////" << endl;
  cout<<"Event: reads/writes in bank 0 = " <<  bank0 << endl;
  cout<<"Event: reads/writes in bank 1 = " <<  bank1 << endl;
  cout<<"Event: reads/writes in bank 2 = " <<  bank2 << endl;
  cout<<"Event: reads/writes in bank 3 = " <<  bank3 << endl;
  cout<<"Event: reset count            = " <<  rst_cnt << endl;
}


// ************* BASE TEST ************ //
// Inits sdram and applied reset
  void base_test::test() {
    intf_int->done = 0;

    sc_uint<32> addr = 0x0;
    sc_uint<4>  bl   = 0x0;
    sc_uint<32> data = 0x0;
    sc_uint<4>  cyc  = 0x0;

    // Initialization
    unsigned long long seed =  scv_random::pick_random_seed();
    cout << "=======================================" << endl;
    cout<<"TEST: "<< name << endl;
    cout<<"SEED: "<< cout.precision(30) << seed << endl;
    cout << "=======================================" << endl;
    env->drv->init(seed);
    cyc  = env->drv->sig_gen->wait_rnd_gen();
    wait(cyc);

    // Request for simulation termination
    cout << "=======================================" << endl;
    cout << " SIMULATION END" << endl;
    cout << "=======================================" << endl;
    cyc  = env->drv->sig_gen->wait_rnd_gen();
    wait(cyc);
    env->cov->print_cov();
    intf_int->done = 1;
    // Just wait for few cycles
  }

// ************* BASIC FUNCTIONALITY ************ //
// Makes one write and one read
  void basic_func::test() {
    intf_int->done = 0;

    sc_uint<4>  cyc  = 0x0;

    // Initialization
    env->scb->n_cases = 1; // Max number of r/w to do
    unsigned long long seed =  scv_random::pick_random_seed();
    cout << "=======================================" << endl;
    cout<<"TEST: "<< name << endl;
    cout<<"SEED: "<< cout.precision(30) << seed << endl;
    cout << "=======================================" << endl;
    env->drv->init(seed);
    cyc  = env->drv->sig_gen->wait_rnd_gen();
    wait(cyc);

    // Write
    env->drv->rnd_write();
    //env->drv->write(0x40000, 1,  4294967295);
    cyc = env->drv->sig_gen->wait_rnd_gen();
    wait(cyc);

    // Read
    env->drv->seq_read();
    cyc  = env->drv->sig_gen->wait_rnd_gen();
    wait(cyc);

    sc_core::sc_time timeA = sc_time_stamp();
    wait(1);
    sc_core::sc_time timeB = sc_time_stamp();
    sc_core::sc_time time_diff = (timeB - timeA);
    // Request for simulation termination
    cout << "=======================================" << endl;
    //cout << " Clock Period = " << time_diff<<endl;
    cout << " SIMULATION END" << endl;
    cout << "=======================================" << endl;
    cyc  = env->drv->sig_gen->wait_rnd_gen();
    wait(cyc);
    env->cov->print_cov();
    intf_int->done = 1;
    // Just wait for few cycles
  }


// ********** READ AFTER RESET ************ //
// Reads data after reset memory
  void rd_after_rst::test() {
    intf_int->done = 0;

    sc_uint<32> addr = 0x0;
    sc_uint<4>  bl   = 0x0;
    sc_uint<32> data = 0x0;
    sc_uint<4>  cyc  = 0x0;

    // Initialization
    env->scb->n_cases = 1; // Max number of r/w to do
    unsigned long long seed =  scv_random::pick_random_seed();
    cout << "=======================================" << endl;
    cout<<"TEST: "<< name << endl;
    cout<<"SEED: "<< cout.precision(30) << seed << endl;
    cout << "=======================================" << endl;
    env->drv->init(seed);
    cyc  = env->drv->sig_gen->wait_rnd_gen();
    wait(cyc);

    // Write
    env->drv->write(0x40000, 1,  8);
    cyc = env->drv->sig_gen->wait_rnd_gen();
    wait(cyc);

    // Read
    env->drv->read(0x40000, 1);
    cyc  = env->drv->sig_gen->wait_rnd_gen();
    wait(cyc);

    // Reset
    env->drv->reset();
    cyc  = env->drv->sig_gen->wait_rnd_gen();
    wait(cyc);

    // Read
    env->drv->read(0x40000, 1);
    cyc  = env->drv->sig_gen->wait_rnd_gen();
    wait(cyc);

    // Request for simulation termination
    cout << "=======================================" << endl;
    cout << " SIMULATION END" << endl;
    cout << "=======================================" << endl;
    cyc  = env->drv->sig_gen->wait_rnd_gen();
    wait(cyc);
    env->cov->print_cov();
    intf_int->done = 1;
    // Just wait for few cycles
  }

// ********** OVERWRITE DATA ************ //
// Reads data after reset memory
  void overwrite::test() {
    intf_int->done = 0;

    sc_uint<32> addr = 0x0;
    sc_uint<4>  bl   = 0x0;
    sc_uint<32> data = 0x0;
    sc_uint<4>  cyc  = 0x0;

    // Initialization
    env->scb->n_cases = 1; // Max number of r/w to do
    unsigned long long seed =  scv_random::pick_random_seed();
    cout << "=======================================" << endl;
    cout<<"TEST: "<< name << endl;
    cout<<"SEED: "<< cout.precision(30) << seed << endl;
    cout << "=======================================" << endl;
    env->drv->init(seed);
    cyc  = env->drv->sig_gen->wait_rnd_gen();
    wait(cyc);

    // Write
    env->drv->write(0x40000, 1,  8);
    cyc = env->drv->sig_gen->wait_rnd_gen();
    wait(cyc);

    // Read
    env->drv->read(0x40000, 1);
    cyc  = env->drv->sig_gen->wait_rnd_gen();
    wait(cyc);

    // Overwrite data
    env->drv->write(0x40000, 1,  26);
    cyc = env->drv->sig_gen->wait_rnd_gen();
    wait(cyc);

    // Read
    env->drv->read(0x40000, 1);
    cyc  = env->drv->sig_gen->wait_rnd_gen();
    wait(cyc);

    // Request for simulation termination
    cout << "=======================================" << endl;
    cout << " SIMULATION END" << endl;
    cout << "=======================================" << endl;
    cyc  = env->drv->sig_gen->wait_rnd_gen();
    wait(cyc);
    env->cov->print_cov();
    intf_int->done = 1;
    // Just wait for few cycles
  }

// ********** PAGE CROSS OVER ************ //
// Creates a page cross over
  void cross_over::test() {
    intf_int->done = 0;

    sc_uint<32> addr = 0x0;
    sc_uint<4>  bl   = 0x0;
    sc_uint<32> data = 0x0;
    sc_uint<4>  cyc  = 0x0;

    // Initialization
    env->scb->n_cases = 24; // Max number of r/w to do
    unsigned long long seed =  scv_random::pick_random_seed();
    cout << "=======================================" << endl;
    cout<<"TEST: "<< name << endl;
    cout<<"SEED: "<< cout.precision(30) << seed << endl;
    cout << "=======================================" << endl;
    env->drv->init(seed);
    cyc  = env->drv->sig_gen->wait_rnd_gen();
    wait(cyc);

    // Writes data...
    env->drv->write(0x00000FF0, 0x8, env->drv->sig_gen->data_rnd_gen());
    env->drv->write(0x00010FF4, 0x8, env->drv->sig_gen->data_rnd_gen());
    env->drv->write(0x00020FF8, 0x8, env->drv->sig_gen->data_rnd_gen());
    env->drv->write(0x00030FFC, 0x8, env->drv->sig_gen->data_rnd_gen());
    env->drv->write(0x00040FE0, 0x8, env->drv->sig_gen->data_rnd_gen());
    env->drv->write(0x00050FE4, 0x8, env->drv->sig_gen->data_rnd_gen());
    env->drv->write(0x00060FE8, 0x8, env->drv->sig_gen->data_rnd_gen());
    env->drv->write(0x00070FEC, 0x8, env->drv->sig_gen->data_rnd_gen());
    env->drv->write(0x00080FD0, 0x8, env->drv->sig_gen->data_rnd_gen());
    env->drv->write(0x00090FD4, 0x8, env->drv->sig_gen->data_rnd_gen());
    env->drv->write(0x000A0FD8, 0x8, env->drv->sig_gen->data_rnd_gen());
    env->drv->write(0x000B0FDC, 0x8, env->drv->sig_gen->data_rnd_gen());
    env->drv->write(0x000C0FC0, 0x8, env->drv->sig_gen->data_rnd_gen());
    env->drv->write(0x000D0FC4, 0x8, env->drv->sig_gen->data_rnd_gen());
    env->drv->write(0x000E0FC8, 0x8, env->drv->sig_gen->data_rnd_gen());
    env->drv->write(0x000F0FCC, 0x8, env->drv->sig_gen->data_rnd_gen());
    //env->drv->write(0x00100FB0, 0x8, env->drv->sig_gen->data_rnd_gen());
    // env->drv->write(0x00110FB4, 0x8, env->drv->sig_gen->data_rnd_gen());
    // env->drv->write(0x00120FB8, 0x8, env->drv->sig_gen->data_rnd_gen());
    // env->drv->write(0x00130FBC, 0x8, env->drv->sig_gen->data_rnd_gen());
    // env->drv->write(0x00140FA0, 0x8, env->drv->sig_gen->data_rnd_gen());
    // env->drv->write(0x00150FA4, 0x8, env->drv->sig_gen->data_rnd_gen());
    // env->drv->write(0x00160FA8, 0x8, env->drv->sig_gen->data_rnd_gen());
    // env->drv->write(0x00170FAC, 0x8, env->drv->sig_gen->data_rnd_gen());
    cyc  = env->drv->sig_gen->wait_rnd_gen();
    wait(cyc);

    // Reads data
    env->drv->read(0x00000FF0, 0x8);
    env->drv->read(0x00010FF4, 0x8);
    env->drv->read(0x00020FF8, 0x8);
    env->drv->read(0x00030FFC, 0x8);
    env->drv->read(0x00040FE0, 0x8);
    env->drv->read(0x00050FE4, 0x8);
    env->drv->read(0x00060FE8, 0x8);
    env->drv->read(0x00070FEC, 0x8);
    env->drv->read(0x00080FD0, 0x8);
    env->drv->read(0x00090FD4, 0x8);
    env->drv->read(0x000A0FD8, 0x8);
    env->drv->read(0x000B0FDC, 0x8);
    env->drv->read(0x000C0FC0, 0x8);
    env->drv->read(0x000D0FC4, 0x8);
    env->drv->read(0x000E0FC8, 0x8);
    env->drv->read(0x000F0FCC, 0x8);
    env->drv->read(0x00100FB0, 0x8);
    env->drv->read(0x00110FB4, 0x8);
    env->drv->read(0x00120FB8, 0x8);
    env->drv->read(0x00130FBC, 0x8);
    env->drv->read(0x00140FA0, 0x8);
    env->drv->read(0x00150FA4, 0x8);
    env->drv->read(0x00160FA8, 0x8);
    env->drv->read(0x00170FAC, 0x8);
    cyc  = env->drv->sig_gen->wait_rnd_gen();
    wait(cyc);

    // Request for simulation termination
    cout << "=======================================" << endl;
    cout << " SIMULATION END" << endl;
    cout << "=======================================" << endl;
    cyc  = env->drv->sig_gen->wait_rnd_gen();
    wait(cyc);
    env->cov->print_cov();
    intf_int->done = 1;
    // Just wait for few cycles
  }

// ********** USAGE OF 4 BANKS ************ //
// Stimulates all SDRAM banks
  void usg_4_banks::test() {
    intf_int->done = 0;

    sc_uint<32> addr = 0x0;
    sc_uint<4>  bl   = 0x0;
    sc_uint<32> data = 0x0;
    sc_uint<4>  cyc  = 0x0;

    // Initialization
    env->scb->n_cases = 4; // Max number of r/w to do
    unsigned long long seed =  scv_random::pick_random_seed();
    cout << "=======================================" << endl;
    cout<<"TEST: "<< name << endl;
    cout<<"SEED: "<< cout.precision(30) << seed << endl;
    cout << "=======================================" << endl;
    env->drv->init(seed);
    cyc  = env->drv->sig_gen->wait_rnd_gen();
    wait(cyc);

    // Writes data in 4 banks
    env->drv->write(0x000000, 0x4, env->drv->sig_gen->data_rnd_gen()); // bank 0
    env->drv->write(0x001400, 0x4, env->drv->sig_gen->data_rnd_gen()); // bank 1
    env->drv->write(0x002800, 0x4, env->drv->sig_gen->data_rnd_gen()); // bank 2
    env->drv->write(0x003C00, 0x4, env->drv->sig_gen->data_rnd_gen()); // bank 3
    cyc  = env->drv->sig_gen->wait_rnd_gen();
    wait(cyc);

    // Reads data
    env->drv->read(0x000000, 0x4);
    env->drv->read(0x001400, 0x4);
    env->drv->read(0x002800, 0x4);
    env->drv->read(0x003C00, 0x4);
    cyc  = env->drv->sig_gen->wait_rnd_gen();
    wait(cyc);

    // Request for simulation termination
    cout << "=======================================" << endl;
    cout << " SIMULATION END" << endl;
    cout << "=======================================" << endl;
    cyc  = env->drv->sig_gen->wait_rnd_gen();
    wait(cyc);
    env->cov->print_cov();
    intf_int->done = 1;
    // Just wait for few cycles
  }

  // ************* TOGGLE SIGNALS ************ //
  // Toggle some signals for transition coverage
    void toggle_sig::test() {
      intf_int->done = 0;

      sc_uint<4>  cyc  = 0x0;

      // Initialization
      env->scb->n_cases = 1; // Max number of r/w to do
      unsigned long long seed =  scv_random::pick_random_seed();
      cout << "=======================================" << endl;
      cout<<"TEST: "<< name << endl;
      cout<<"SEED: "<< cout.precision(30) << seed << endl;
      cout << "=======================================" << endl;
      env->drv->toggle();
      cyc  = env->drv->sig_gen->wait_rnd_gen();
      wait(cyc);
      env->drv->init(seed);
      cyc  = env->drv->sig_gen->wait_rnd_gen();
      wait(cyc);

      // Write
      env->drv->rnd_write();
      //env->drv->write(0x40000, 1,  4294967295);
      cyc = env->drv->sig_gen->wait_rnd_gen();
      wait(cyc);

      // Read
      env->drv->seq_read();
      cyc  = env->drv->sig_gen->wait_rnd_gen();
      wait(cyc);

      // Request for simulation termination
      cout << "=======================================" << endl;
      cout << " SIMULATION END" << endl;
      cout << "=======================================" << endl;
      cyc  = env->drv->sig_gen->wait_rnd_gen();
      wait(cyc);
      env->cov->print_cov();
      intf_int->done = 1;
      // Just wait for few cycles
    }
