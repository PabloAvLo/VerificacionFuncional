/***************************************************************************
                           Universidad de Costa Rica
                 		                  IE-1119
 				                      Verificación Funcional
                                  I Ciclo 2018

 Autor: Pablo Avila B30724

 Descripcion:
 Se requiere disenar un  Test Bench en System C para el detector de flancos,
 en el cual despues de 4 1's consecutivos en la entrada 'w', la salida 'z' se
 ponga en alto, incluyendo secuencias traslapadas. El detector cuenta con
 entradas y salidas sincronicas.

*****************************************************************************/
`timescale 1s / 100ms

module detector(
  clk,	// Clock
  rst,		// Reset: Active high, sync.
  w, 			// Detector data input
  z				// Detector output
  );

//------------- Input Ports --------------------------------
  input wire clk, rst, w;

//------------- Output Ports -------------------------------
  output reg z;

//------------- Internal Constants--------------------------
parameter SIZE = 3;
parameter S0 = 3'b000, S1 = 3'b001, S2 = 3'b010,
          S3 = 3'b011, S4 = 3'b100;


//------------- Internal Variables---------------------------
reg [SIZE-1:0]	state        ; // Seq part of the FSM
reg [SIZE-1:0]	next_state   ; // combo part of FSM
reg             last_w;

//------------- Next State Logic ----------------------------
always @ (state or w)
begin : NEXT_STATE_LOGIC

 case(state)
   S0 : begin
          next_state = S1;
				  last_w = w;
				end

   S1 : begin
					if (w == last_w) begin
                next_state = S2;
          end else begin
                next_state = S2;
					end
				  last_w = w;
				end

   S2 : begin
 				  if (w == last_w)
                next_state = S3;
          else
                next_state = S1;
				end

   S3 : begin
				  if (w == last_w)
                next_state = S4;
          else
                next_state = S1;
				end

   S4 : begin
					if (w == last_w)
                next_state = S4;
          else
                next_state = S1;
				end

   default : begin
								next_state = S0;
						 end
endcase

end

//------------- Sequential Logic-----------------------------
always @ (posedge clk or posedge rst)
begin : FSM_SEQ

  if (rst == 1'b1)
    state <= S0;
  else
    state <= next_state;

end

//------------- Output Logic-----------------------------
always @ (state)
begin : OUTPUT_LOGIC

  case(state)

    S0    	: z <= 1'b0;
    S1    	: z <= 1'b0;
    S2    	: z <= 1'b0;
    S3    	: z <= 1'b0;
    S4    	: z <= 1'b1;
    default : z <= 1'b0;

  endcase
end

endmodule // End of Module detector


//------------- Test Bench Top Level --------------------
module tb();

//------------- Signals --------------------------
reg clk, rst, w;
wire z;

//------------- Clock Generator ------------------
always #1 clk = ~clk;

//------------- Device Under Test ------------------
detector dut (
// Inputs
.clk (clk),
.rst (rst),
.w (w),
// Outputs
.z (z)
);

initial begin
 $dumpfile("prueba.vcd");
 $dumpvars(0);
 $display("[DUT Detector]: Starting SystemC_Detector...");
 $sc_tb;// Testbench Connection
 clk = 0;
end

endmodule


/*
//------------- Test Bench -----------------------
module detector_TestBench;

//------------- Signals --------------------------
reg clock, rst, w;
wire z;

//------------- Clock -----------------------------
always begin
 #1 clock = ~clock;
end

//------------- Device Under Test ------------------
detector Detector (
.clock (clock),
.rst (rst),
.w (w),
.z (z)
);

initial begin

  $dumpfile("prueba.vcd");
  $dumpvars(0);

  $monitor ("w= %b, rst= %b, z= %b", w, rst, z);
  $display("\n***Simulation begin \n\n-----DETECTOR RTL -----");


  $display("\nCASE A: Four consecutive 1's and 0's with rst = 1");
  clock = 0;
  rst = 1;
  w = 1;
  #8 w = 0;

  $display("\nCASE B: Four consecutive 1's and 0's with rst = 0");
  #8 rst = 0;
  w = 1;
  #8 w = 0;

  $display("\nCASE C: Five consecutive 1's and 0's with rst = 0");
  #8 w = 1;
  #10 w = 0;

  $display("\nCASE D: Demonstration of asynchronous reset");
  #10 w =1;
  #8.3 rst = 1;

  #4 $display("\n***Simulation end \n");
  $finish;

end

endmodule
*/
