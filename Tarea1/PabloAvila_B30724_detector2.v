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
  rst,	// Reset: Active high, sync.
  w, 	// Detector data input
  z	// Detector output
  ); 

//------------- Input Ports --------------------------------
  input wire clock, rst, w;


//------------- Output Ports -------------------------------
  output reg z;


//------------- Internal Constants--------------------------
parameter SIZE = 3;
parameter IDLE  = 3'b001,GNT0 = 3'b010,GNT1 = 3'b100 ;


//------------- Internal Variables---------------------------
reg   [SIZE-1:0]          state        ;// Seq part of the FSM
reg   [SIZE-1:0]          next_state   ;// combo part of FSM


//------------- Next State Logic ----------------------------
always @ (state)
begin : FSM_COMBO
 next_state = 3'b000;
 case(state)
   IDLE : if (w == 1'b1)
                next_state = GNT0;
          else 
                next_state = IDLE;
   GNT0 : if (w == 1'b1)
                next_state = GNT0;
          else
                next_state = IDLE;
   GNT1 : if (w == 1'b1)
                next_state = GNT1;
          else
                next_state = IDLE;
   default : next_state = IDLE;
  endcase
end

//------------- Sequential Logic-----------------------------
always @ (posedge clock or posedge rst)
begin : FSM_SEQ

  if (rst == 1'b1)
    state <= IDLE;
  else
    state <= next_state;

end

//------------- Output Logic-----------------------------
always @ (state)
begin : OUTPUT_LOGIC

  case(state)

    IDLE    : z <= 1'b0;
    GNT0    : z <= 1'b0;
    GNT1    : z <= 1'b1;
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

  $monitor ("w=%b, z=%b", w, z);
  $display("\n***Simulation begin \n\n-----DETECTOR RTL -----");


  $display("\nCASE A: Four consecutive 1's and 0's with rst = 1");
  clock = 0;
  rst = 1;
  w = 1;
  #10 w = 0; 

  $display("\nCASE B: Four consecutive 1's with rst = 0");
  #8 rst = 0;
  w = 1;
  #8 w = 0;

  $display("\nCASE C: Five consecutive 1's with rst = 0");
  #8 w = 1;
  #10 w = 0;

  $display("\nCASE D: Demonstration of asynchronous reset");
  #10 w =1;
  #4.4 rst = 1;	 

  #4 $display("\n***Simulation end \n");
  $finish;
  
end

endmodule
