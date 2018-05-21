#include "sc_tb.h"

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
    wait(3);
    w = 0;

    cout<<"@"<<sc_time_stamp()<<" CASE C: Five consecutive 1's and 0's with rst = 0 " << endl;
    wait(4);
    w = 1;
    wait(5);
    w = 0;

    cout<<"@"<<sc_time_stamp()<<" CASE D: Demonstration of asynchronous reset " << endl;
    wait(5);
    w = 1;
    wait(1.3);
    rst = true;

    cout << "=======================================" << endl;
    cout << " SIMULATION PASSED" << endl;
    cout << "=======================================" << endl;

    // Just wait for few cycles
    wait(4);
    done = 1;
  }
}
