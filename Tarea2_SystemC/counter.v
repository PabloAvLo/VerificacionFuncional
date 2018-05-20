`timescale 1ns / 1ns
// Verilog DUT 
module counter(
rst,   // Reset input
clk,   // Clock Input
d_out  // Counter output
);
// Port Declation
input      rst;
input      clk;
output  [31:0]  d_out;
// Internal data type
reg  [31:0]  d_out;
// Code starts here
always @ (posedge clk)
  if (rst)
  begin
    d_out <= 0;
  end
  else
  begin
    d_out <= d_out + 1;
  end

endmodule

// Testbench top level
module tb();
reg      rst;
reg      clk;
wire  [31:0]  d_out;

initial begin
 $dumpfile("verilog.vcd");
 $dumpvars(0);
 $display("[DUT Counter]: Starting sc_counter...");
 $sc_tb;// Testbench Connection
 clk = 0;
end
// Clock generator
always #1 clk = ~clk;
  
// DUT connection
counter dut (
    // Inputs
    rst,
    clk,
    // Outputs
    d_out
);

endmodule
