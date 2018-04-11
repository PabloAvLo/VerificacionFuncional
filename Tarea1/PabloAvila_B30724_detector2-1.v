/***************************************************************************
                           Universidad de Costa Rica
                 		     IE-1119 
 				Verificación Funcional
                                  I Ciclo 2018

 Autor: Pablo Avila B30724

 Descripcion:
 Se requiere disenar un detector de flancos, en el cual despues de 4 1's
 consecutivos en la entrada 'w', la salida 'z' se ponga en alto, incluyendo 
 secuencias traslapadas. El detector cuenta con entradas y salidas sincronicas.

*****************************************************************************/
`timescale 1s / 100ms

module detector2(
  clock,	// Clock
  rst,		// Reset: Active high, sync.
  w, 			// Detector data input
  z				// Detector output
  ); 

//------------- Input Ports --------------------------------
  input wire clock, rst, w;


//------------- Output Ports -------------------------------
  output reg z;


//------------- Internal Constants--------------------------
parameter SIZE = 3;
parameter S0 = 3'b000, S1 = 3'b001, S2 = 3'b010,
          S3 = 3'b011, S4 = 3'b100;    


//------------- Internal Variables---------------------------
reg [SIZE-1:0]	state        ; // Seq part of the FSM
reg [SIZE-1:0]	next_state   ; // combo part of FSM
reg             first_w;		 


//------------- Next State Logic ----------------------------
always @ (state)
begin : NEXT_STATE_LOGIC

 case(state)
   S0 : begin
          next_state = S1;
					first_w = w;
        end
   S1 : if (w == first_w) 
                next_state = S2;
          else
                next_state = S0;
   S2 : if (w == first_w)
                next_state = S3;
          else
                next_state = S0;
   S3 : if (w == first_w)
                next_state = S4;
          else
                next_state = S0;
   S4 : if (w == first_w)
                next_state = S4;
          else
                next_state = S0;
   default : next_state = S0;
  endcase

end

//------------- Sequential Logic-----------------------------
always @ (posedge clock or posedge rst)
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

endmodule // End of Module detector2


//------------- Test Bench -----------------------
module Detector2_TestBench;

//------------- Signals --------------------------
reg clock, rst, w;
wire z;

//------------- Clock -----------------------------
always begin
 #1 clock = ~clock;
end

//------------- Device Under Test ------------------
detector2 Detector (
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
  #10 w = 0; 

  $display("\nCASE B: Four consecutive 1's and 0's with rst = 0");
  #8 rst = 0;
  w = 1;
  #8 w = 0;

  $display("\nCASE C: Five consecutive 1's and 0's with rst = 0");
  #8 w = 1;
  #10 w = 0;

  $display("\nCASE D: Demonstration of asynchronous reset");
  #10 w =1;
  #3.5 rst = 1;	 

  #4 $display("\n***Simulation end \n");
  $finish;
  
end

endmodule
