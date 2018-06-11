#include "sc_tb.h"


/* No Operation: When Chip selected is not asserted or no operation
is requested. */
void driver::NOP(){
  cout<<"@"<<sc_time_stamp()<<" Started NOP " << endl;
  intf_int->sdr_ras_n = true;
  intf_int->sdr_cas_n = true;
  intf_int->sdr_we_n = true;
  cout<<"@"<<sc_time_stamp()<<" Finished NOP " << endl;
}

/* Active: To Open Banks for read from or write to addresses. */
void driver::Active(){
  cout<<"@"<<sc_time_stamp()<<" Started Active " << endl;
  intf_int->sdr_ras_n = false;
  intf_int->sdr_cas_n = true;
  intf_int->sdr_we_n = true;
  cout<<"@"<<sc_time_stamp()<<" Finished Active " << endl;
}

/* For the read command, the initial data appears on the data bus 1-4 clock
cycles later. CAS Latency. */
double driver::Read(sc_uint<8> &address){
  cout<<"@"<<sc_time_stamp()<<" Started Read " << endl;
  intf_int->sdr_ras_n = true;
  intf_int->sdr_cas_n = false;
  intf_int->sdr_we_n = true;

  wait(4);
  double data =0;

  return data;
  cout<<"@"<<sc_time_stamp()<<" Finished Read " << endl;
}

void driver::Write(sc_uint<8> &address){
  cout<<"@"<<sc_time_stamp()<<" Started Write " << endl;
  intf_int->sdr_ras_n = true;
  intf_int->sdr_cas_n = false;
  intf_int->sdr_we_n = false;
  cout<<"@"<<sc_time_stamp()<<" Finished Write " << endl;
}

/* After the initial Read or Write command, sequential read and writes will
continue until the burst length is reached or a Burst Terminate command is
issued. */
void driver::BurstTerminate(){
  cout<<"@"<<sc_time_stamp()<<" Started Burst Terminate " << endl;
  intf_int->sdr_ras_n = true;
  intf_int->sdr_cas_n = true;
  intf_int->sdr_we_n = false;
  cout<<"@"<<sc_time_stamp()<<" Finished Burst Terminate " << endl;
}

/* Recharge: To close the bank. When a new row on a bank is accessed for a read
or a write it may be necessary to first close the bank and then re-open
the bank to the new row. */
void driver::Recharge(){
  cout<<"@"<<sc_time_stamp()<<" Started Recharge " << endl;
  intf_int->sdr_ras_n = false;
  intf_int->sdr_cas_n = true;
  intf_int->sdr_we_n = false;
  cout<<"@"<<sc_time_stamp()<<" Finished Recharge " << endl;
}

/*SDRAM devices require periodic refresh operations to maintain the integrity
of the stored data. The SDRAM Controller Core automatically issues the
Auto Refresh command periodically. No user intervention is required.*/
void driver::AutoRefresh(){
  cout<<"@"<<sc_time_stamp()<<" Started Auto Refresh " << endl;
  intf_int->sdr_ras_n = false;
  intf_int->sdr_cas_n = false;
  intf_int->sdr_we_n = true;
  cout<<"@"<<sc_time_stamp()<<" Finished Auto Refresh " << endl;
}

void driver::LoadModeRegister(){
  cout<<"@"<<sc_time_stamp()<<" Started Load Mode Register " << endl;
  intf_int->sdr_ras_n = false;
  intf_int->sdr_cas_n = false;
  intf_int->sdr_we_n = false;
  cout<<"@"<<sc_time_stamp()<<" Finished Load Mode Register " << endl;
}

void driver::initializationTopWishbone(){
 //Config Parameters
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

  intf_int->wb_addr_i      = 0;
  intf_int->wb_dat_i       = 0;
  intf_int->wb_sel_i       = 0x0;
  intf_int->wb_we_i        = false;
  intf_int->wb_stb_i       = 0;
  intf_int->wb_cyc_i       = 0;
  intf_int->wb_rst_i       = 0;
  intf_int->sdram_resetn   = 1;
  intf_int->errCnt         = 0;
  wait(10);

  // Applying reset
  intf_int->wb_rst_i       = 1;
  intf_int->sdram_resetn   = 0;
  wait(1000);

  // Releasing reset
  intf_int->wb_rst_i       = 0;
  intf_int->sdram_resetn   = 1;
  wait(100);
  cout<<"Waiting for SDRAM to initialize, sdr_init_done: "<<intf_int->sdr_init_done<<endl;
  while(intf_int->sdr_init_done == 0) {
      wait(1);
  }
  cout<<"SDRAM is ready! sdr_init_done: "<<intf_int->sdr_init_done<<endl;
  wait(100);
}

void driver::writeTopWishbone(sc_uint<32> &address, sc_uint<8> &burstLenght){

  driver::afifo.push(address);
  driver::bfifo.push(burstLenght);
  //   @ (negedge sys_clk);
  cout<<"Write Address: "<< address <<", Burst Size: "<< burstLenght <<endl;
  for(int i=0; i < burstLenght; i++){
    intf_int->wb_stb_i        = true;
    intf_int->wb_cyc_i        = true;
    intf_int->wb_we_i         = true;
    intf_int->wb_sel_i        = 0b1111;
    intf_int->wb_addr_i       = (address >> 2) + i; // Address[31:2]+i;
    intf_int->wb_dat_i        = rand() & 0xFF; //  $random & 32'hFFFFFFFF;

    wait(2);
    while(intf_int->wb_ack_o == false) {
        wait(1);
    }
    driver::dfifo.push(intf_int->wb_dat_i);

    cout<<"Status: Burst-No: "<< i <<", Write Address: "<< intf_int->wb_addr_i
    <<", WriteData: "<< intf_int->wb_dat_i << endl;
  }

  intf_int->wb_stb_i        = false;
  intf_int->wb_cyc_i        = false;
  intf_int->wb_we_i         = false; //'hx;
  intf_int->wb_sel_i        = 0; //'hx;
  intf_int->wb_addr_i       = 0; //'hx;
  intf_int->wb_dat_i        = 0; //'hx;
}

void driver::readTopWishbone(){

  sc_uint<32> address = driver::afifo.front();
  driver::afifo.pop();
  sc_uint<8>  burstLenght = driver::bfifo.front();
  driver::bfifo.pop();
  sc_uint<32> expectedData;

  wait(1);
  // @ (negedge sys_clk);

  for(int j=0; j < burstLenght; j++){
    intf_int->wb_stb_i        = true;
    intf_int->wb_cyc_i        = true;
    intf_int->wb_we_i         = false;
    intf_int->wb_addr_i       = (address >> 2) + j; // Address[31:2]+j;
    expectedData    = driver::dfifo.front(); // Exptected Read Data
    driver::dfifo.pop();

    /*
    do begin
      @ (posedge sys_clk);
    end while(wb_ack_o == 1'b0);
    */
    wait(2);
    while(intf_int->wb_ack_o == 0b0) {
        wait(1);
    }

    if(intf_int->wb_dat_o != expectedData){
      cout<<"READ ERROR: Burst-No: "<< j <<", Addr: "<< intf_int->wb_addr_i
      <<" Rxp: " << intf_int->wb_dat_o <<", Exd: "<< expectedData << endl;

      intf_int->errCnt += 1;
    } else {
      cout<<"READ STATUS: Burst-No: "<< j <<", Addr: "<< intf_int->wb_addr_i
      <<" Rxp: " << intf_int->wb_dat_o << endl;
    }

    // @ (negedge sdram_clk);
  }

  intf_int->wb_stb_i        = false;
  intf_int->wb_cyc_i        = false;
  intf_int->wb_we_i         = false; //'hx;
  intf_int->wb_addr_i       = 0; //'hx;
}

/* Need to monitor the Status
  void monitor::mnt_out(){
    while(true){
    wait(1);
    data_out_exp = scb_int->fifo.read();
    data_out_read = intf_int->sdr_data_out;
    cout<<"@"<<sc_time_stamp()<<" Monitor data_out:" << data_out_exp << endl;
    cout<<"@"<<sc_time_stamp()<<" Scoreboard data_out:" << data_out_read << endl;
    //Checker
    if (data_out_exp != data_out_read)
      //assert(data_out_exp == data_out_read);
      cout<<"@"<<sc_time_stamp()<<" ERROR: data read and expected mismatch!" << endl;
    }
}
*/

  //Test
  void base_test::test() {
    intf_int->done = 0;
    sc_uint<32> adr = 0x00000FF0;
    sc_uint<8> bl = 0x04;

    // Initialization
    env->drv->initializationTopWishbone();

    // TEST CASE 1
    //for (int i=4; i<=4; i+=4){
      env->drv->writeTopWishbone(adr, bl);
      wait(100);
      //adr += i;
      //bl += i;
    //}
    //wait(10);
    //for (int i=0; i<1; i++){
      env->drv->readTopWishbone();
      wait(10);
    //}
    // Request for simulation termination
    cout << "=======================================" << endl;
    cout << " SIMULATION END" << endl;
    cout << "=======================================" << endl;
    wait(10);
    intf_int->done = 1;
    // Just wait for few cycles
  }
