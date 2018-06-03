#!/bin/sh
echo 'Cleaning the directory'
rm -rf *.o *.vvp *.vpi *vcd
echo 'Compiling *.c *cpp files'
g++ -fpic -c sc_tb_vpi.c sc_vpi.c sc_tb.cpp -I /usr/include/iverilog/ -I /usr/local/systemc-2.3.2/include/
echo 'Creating vpi file'
g++ -shared -L /usr/local/systemc-2.3.2/lib-linux64  -Wl,-rpath=/usr/local/systemc-2.3.2/lib-linux64 -o sc_tb_vpi.vpi *.o -lsystemc -lm -lvpi
echo 'Compiling verilog files'
iverilog -o tb.vvp tb.v ../rtl/top/sdrc_top.v ../rtl/wb2sdrc/wb2sdrc.v ../rtl/core/sdrc_core.v ../rtl/core/sdrc_bank_ctl.v ../rtl/core/sdrc_bank_fsm.v ../rtl/core/sdrc_req_gen.v ../rtl/core/sdrc_xfr_ctl.v ../rtl/core/sdrc_bs_convert.v ../rtl/lib/async_fifo.v
echo 'Simulation Started'
vvp -n -M. -msc_tb_vpi tb.vvp
echo 'Simulation Ended'
