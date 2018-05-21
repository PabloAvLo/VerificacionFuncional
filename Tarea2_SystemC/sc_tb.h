#ifndef SC_TB_TEST_H
#define SC_TB_TEST_H

#include "systemc.h"

SC_MODULE (sc_tb) {
  sc_in<bool>          clk;   // clock input
  sc_in<bool>          z;     // data input
  sc_out<bool>         rst;   // reset ouput
  sc_out<sc_uint<32> >         w;     // data output
  sc_out<int>          done;  // Terminate sim

//  void detector_model ();
//  void monitor   ();
  void test      ();

//  sc_uint<32>  cnt; // counter model
//  int error; // Error status

  SC_CTOR(sc_tb) {
//    SC_CTHREAD(monitor,clk.pos());
//    SC_CTHREAD(detector_model,clk.pos());
    SC_CTHREAD(test,clk.pos());
  }
};

#endif
