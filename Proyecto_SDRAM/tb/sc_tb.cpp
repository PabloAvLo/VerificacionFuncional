#include "sc_tb.h"

  /*void driver::reset(){
    intf_int->rst = true;
    cout<<"@"<<sc_time_stamp()<<" Started Reset " << endl;
    wait(10);
    intf_int->rst = false;
    intf_int->wr_cs = true;
    intf_int->rd_cs = true;
    cout<<"@"<<sc_time_stamp()<<" Finished Reset " << endl;
  }

  void driver::write(){
    cout<<"@"<<sc_time_stamp()<<" Writing " << endl;
    intf_int->data_in = rand();
    intf_int->wr_en = true;
    wait(1);
    scb_int->fifo.write(intf_int->data_in);
    intf_int->data_in = 0;
    intf_int->wr_en = false;

  }

  void driver::read(){
    cout<<"@"<<sc_time_stamp()<<" Reading " << endl;
    intf_int->rd_en = true;
    wait(1);
    intf_int->rd_en = false;
  }

  void monitor::mnt_out(){
    while(true){
    wait(1);
    data_out_exp = scb_int->fifo.read();
    data_out_read = intf_int->data_out;
    cout<<"@"<<sc_time_stamp()<<" Monitor data_out:" << data_out_exp << endl;
    cout<<"@"<<sc_time_stamp()<<" Scoreboard data_out:" << data_out_read << endl;
    //Checker
    if (data_out_exp != data_out_read)
      //assert(data_out_exp == data_out_read);
      cout<<"@"<<sc_time_stamp()<<" ERROR: data read and expected mismatch!" << endl;
    }
}*/


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
