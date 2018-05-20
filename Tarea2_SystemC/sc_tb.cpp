#include "sc_tb.h"

// Counter model
void sc_tb::cnt_model() {
  while (true) {
    wait();
    if (rst.read()) {
      cnt = 0;
    } else {
      cnt ++;
    }
  }
}
// Counter montitor
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
// Counter stim gen
void sc_tb::test() {
  done = 0;
  while (true) {
    rst = true;
    cout<<"@"<<sc_time_stamp()<<" Asserting Reset " << endl;
    wait(10);
    rst = false;
    cout<<"@"<<sc_time_stamp()<<" De-asserting Reset " << endl;
    wait(20);
    // Request for simulation termination
    if (error > 0) {
      cout << "=======================================" << endl;
      cout << " SIMULATION FAILED" << endl;
      cout << "=======================================" << endl;
    } else {
      cout << "=======================================" << endl;
      cout << " SIMULATION PASSED" << endl;
      cout << "=======================================" << endl;
    }
    
    // Just wait for few cycles
    wait(100);
    
    done = 1;
    
  }
}
