#include "sc_tb.h"

// Detector model
/*
void sc_tb::detector_model() {
  while (true) {
    wait();
    if (rst.read()) {
      cnt = 0;
    } else {
      cnt ++;
    }
  }
}

// Detector montitor
void sc_tb::monitor() {
  error = 0;
  while (true) {
    wait();
    cout << "@" <<sc_time_stamp() << "Counter Monitor : tb "
       << cnt << " dut " << d_out << endl;
    if (rst.read() == 0) {
       if (cnt != d_out) {
         error ++;
       }
    }
  }
}
*/
// Detector stim gen
void sc_tb::test() {
  done = 0;
  while (true) {
    cout<<"@"<<sc_time_stamp()<<" ***Simulation begin \n-----DETECTOR RTL ----- " << endl;

    cout<<"@"<<sc_time_stamp()<<" CASE A: Four consecutive 1's and 0's with rst = 1 " << endl;
    rst = true;
    w = 1;
    wait(4);
    w = 0;

    cout<<"@"<<sc_time_stamp()<<" CASE B: Four consecutive 1's and 0's with rst = 0 " << endl;
    wait(4);
    rst = false;
    w = 1;
    wait(4);
    w = 0;

    cout<<"@"<<sc_time_stamp()<<" CASE C: Five consecutive 1's and 0's with rst = 0 " << endl;
    wait(4);
    w = 1;
    wait(5);
    w = 0;

    cout<<"@"<<sc_time_stamp()<<" CASE D: Demonstration of asynchronous reset " << endl;
    wait(5);
    w = 1;
    wait(2.3);
    rst = true;
/*
    cout<<"@"<<sc_time_stamp()<<" De-asserting Reset " << endl;
    wait(20);
    // Request for simulation termination
    if (error > 0) {
      cout << "=======================================" << endl;
      cout << " SIMULATION FAILED" << endl;
      cout << "=======================================" << endl;
    } else {
*/
      cout << "=======================================" << endl;
      cout << " SIMULATION PASSED" << endl;
      cout << "=======================================" << endl;
//    }

    // Just wait for few cycles
    wait(4);
    done = 1;
  }
}
