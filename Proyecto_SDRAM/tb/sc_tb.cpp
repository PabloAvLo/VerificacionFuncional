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
    //env->drv->reset();
    wait(10);
    for (int i=0; i<10; i++){
      //env->drv->write();
      wait(10);
    }
    wait(10);
    for (int i=0; i<10; i++){
      //env->drv->read();
      wait(10);
    }
    // Request for simulation termination
    cout << "=======================================" << endl;
    cout << " SIMULATION END" << endl;
    cout << "=======================================" << endl;
    wait(10);
    intf_int->done = 1;
    // Just wait for few cycles
  }
