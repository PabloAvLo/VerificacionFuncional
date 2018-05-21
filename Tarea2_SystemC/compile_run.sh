#!/bin/sh
echo 'Cleaning the directory'
rm -rf *.o *.vvp *.vpi *vcd
echo 'Compiling *.c *cpp files'
g++ -fpic -c sc_tb_vpi.c sc_vpi.c sc_tb.cpp -I /usr/include/iverilog/ -I /usr/local/system-2.3.2/include/
echo 'Creating vpi file'
g++ -shared -L /usr/local/system-2.3.2/lib-linux64  -Wl,-rpath=/usr/local/system-2.3.2/lib-linux64 -o sc_tb_vpi.vpi *.o -lsystemc -lm -lvpi
echo 'Compiling verilog files'
iverilog -o P_B30724_detector3.vvp P_B30724_detector3.v
echo 'Simulation Started'
vvp -n -M. -msc_tb_vpi P_B30724_detector3.vvp
echo 'Simulation Ended'
