#ifndef SC_TB_TEST_H
#define SC_TB_TEST_H

#include "systemc.h"

SC_MODULE (sc_tb) {
  sc_in<bool>          clk;   // clock input
  sc_in<bool>          z;     // data input
  sc_out<bool>         rst;   // reset ouput
  sc_out<bool>         w;     // data output
  sc_out<int>          done;  // Terminate sim

  void test      ();

  SC_CTOR(sc_tb) {
    SC_THREAD(test);
      sensitive << rst.pos();
      sensitive << clk.pos();
  }
};

#endif
