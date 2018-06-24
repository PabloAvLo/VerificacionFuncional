#include "sc_tb.h"

// ************************** DRIVER SETUP
void driver::SetUpTopWishbone(){
 // Config Parameters
  intf_int->cfg_sdr_width    = 0b10;
  intf_int->cfg_colbits      = 0b00;
  intf_int->cfg_req_depth    = 0x3;
  intf_int->cfg_sdr_en        = 0b1;
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

  intf_int->wb_addr_i      = 0;
  intf_int->wb_dat_i       = 0;
  intf_int->wb_sel_i       = 0x0;
  intf_int->wb_we_i        = false;
  intf_int->wb_stb_i       = 0;
  intf_int->wb_cyc_i       = 0;
  intf_int->wb_rst_i       = 0;
  intf_int->sdram_resetn   = 1;
  //intf_int->sdram_wr_en_n  = 0xf;
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

// ************************** DRIVER WRITE
void driver::writeTopWishbone(sc_uint<32> &address, sc_uint<8> &burstLenght){

  scb_int->addr_fifo.write(address);
  scb_int->buLen_fifo.write(burstLenght);

  //intf_int->sdram_wr_en_n     = 0x0; // Enable write mode

  wait(5, SC_NS);
  cout<<"Write Address: "<< address <<", Burst Size: "<< burstLenght <<endl;
  for(int i=0; i < burstLenght; i++){
    intf_int->wb_stb_i        = true;
    intf_int->wb_cyc_i        = true;
    intf_int->wb_we_i         = true;
    intf_int->wb_sel_i        = 0b1111;
    intf_int->wb_addr_i       = (address >> 2) + i; // Address[31:2]+i;
    intf_int->wb_dat_i        = rand() & 0xFF; //  $random & 32'hFFFFFFFF;

    wait(1);
    while(true) {
        wait(5, SC_NS);
        if (intf_int->wb_ack_o == true) {
            break;
        }
        wait(5, SC_NS);
    }
    scb_int->data_fifo.write(intf_int->wb_dat_i);

    cout<<"Status: Burst-No: "<< i <<", Write Address: "<< intf_int->wb_addr_i
    <<", WriteData: "<< intf_int->wb_dat_i << endl;
  }
  intf_int->wb_stb_i        = false;
  intf_int->wb_cyc_i        = false;
  intf_int->wb_we_i         = false; //'hx;
  intf_int->wb_sel_i        = 0; //'hx;
  intf_int->wb_addr_i       = 0x3FFFFFFF; //'hx;
  intf_int->wb_dat_i        = 0xFF; //'hx;
  //intf_int->sdram_wr_en_n   = 0xf;  // Disable write mode
}

// ************************** DRIVER READ
void driver::readTopWishbone(sc_uint<32> &address, sc_uint<8> &burstLenght){

  wait(5, SC_NS);

  for(int j=0; j < burstLenght; j++) {
    intf_int->wb_stb_i        = true;
    intf_int->wb_cyc_i        = true;
    intf_int->wb_we_i         = false;
    intf_int->wb_addr_i       = (address >> 2) + j; // Address[31:2]+j;

    wait(1);
    while(intf_int->wb_ack_o == 0b0) {
        wait(1, SC_NS);
    }

    cout<<"@"<<sc_time_stamp()<<" SDRAM data read:" << intf_int->wb_dat_o << endl;
    wait(5, SC_NS);
  }

  intf_int->wb_stb_i        = false;
  intf_int->wb_cyc_i        = false;
  intf_int->wb_we_i         = true; //'hx;
  intf_int->wb_addr_i       = 0x3FFFFFFF; //'hx;
}

// ************************** MONITOR
void monitor::mnt_out(){

  // wb_dat_i, wb_dat_o, wb_addr_i, wb_ack_o,
  // sdr_addr, sdr_dq, app_req*, sdr_dout* (sdr_dq), sdr_den_n*,
  cout<<"@"<<sc_time_stamp()<<" Monitor data_out:" << intf_int->wb_dat_o << endl;
}

// ************************** CHECKER
void checker::verify(int mnt_value, string pass_msg){

  int scb_value =0; // scb->search();
  if(mnt_value == scb_value){
    cout<< "PASS: "<<pass_msg <<endl;
  }
  else{
    cout<< "FAIL: Monitor value "<< mnt_value << "does not match with expected scoreboard value "<<scb_value <<endl;
    intf_int->errCnt += 1;
  }


/*
      // -------------------------- Checker
      if(intf_int->wb_dat_o != expectedData) {
        cout<<"READ ERROR: Burst-No: "<< j <<", Addr: "<< intf_int->wb_addr_i
        <<" Rxp: " << intf_int->wb_dat_o <<", Exd: "<< expectedData << endl;

        intf_int->errCnt += 1;
      } else {
        cout<<"READ STATUS: Burst-No: "<< j <<", Addr: "<< intf_int->wb_addr_i
        <<" Rxp: " << intf_int->wb_dat_o << endl;
      }

  }*/
}


// ************************** TEST
  void base_test::test() {
    intf_int->done = 0;
    sc_uint<32> adr = 0x00000FF0;
    sc_uint<8> bl = 0x04;

    // Initialization
    env->scb->n_cases = 5; // Indicates the maximum number of read/writes the test will do.
    env->drv->SetUpTopWishbone();

    // TEST CASE 1
      env->drv->writeTopWishbone(adr, bl);
      wait(100);

      env->drv->readTopWishbone(adr, bl);
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
