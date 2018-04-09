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

module detector(
  clock,	// Clock
  rst,	// Reset: Active high, sync.
  w, 	// Detector data input
  Z	// Detector output
  ); 

//------------- Input Ports ----------------------
  input wire clock, rst, w;

//------------- Output Ports ---------------------
  output wire Z;


//------------- Internal Signals -----------------
  wire b2, b1, b0; // Present states bits for 5 states
  wire B2, B1, B0; // Proximate state bits
  wire z;		

  wire w_Nrst; // = w * rst' 
  wire bc, b_nc, nb_c, na_nc;
  wire a_bc, bnc_nbc, nb_nc; 

//------------- Next State Logic ------------------
  and w_nrst (w_Nrst, w, ~rst);

// B2 = (b2 + b1*b0) * w * rst'
  and and1 (bc, b1, b0);
  or or1(a_bc, b2, bc);
  and and2 (B2, a_bc, w_Nrst);	

// B1 = (b1*b0' + b1'*b0) * w * rst'
  and and3 (b_nc, b1, ~b0);
  and and4 (nb_c, ~b1, b0);
  or or2 (bnc_nbc, b_nc, nb_c);
  and and5 (B1, bnc_nbc, w_Nrst);

// BC = (b2'*b0') * w * rst'
  and and6 (na_nc, ~b2, ~b0);
  and and7 (B0, na_nc, w_Nrst);


//Conexion explicita de flip-flops
  ff_d ffB2 (.q(b2), .d(B2), .clk(clock));
  ff_d ffB1 (.q(b1), .d(B1), .clk(clock));
  ff_d ffB0 (.q(b0), .d(B0), .clk(clock));

//------------- Output Logic ------------------
  and and8 (nb_nc, ~b1, ~b0);
  and and9 (z, b2, nb_nc);

  //Conexion explicita de flip-flops
  ff_d ffZ (.q(Z), .d(z), .clk(clock));

endmodule

//------------- Test Bench -----------------------
module Detector_TestBench;

//------------- Signals --------------------------
reg clock, rst, w;
wire z;

//------------- Unit Under Test ------------------
detector U0 (
.clock (clock),
.rst (rst),
.w (w),
.Z (z)
);

initial begin

  $dumpfile("prueba.vcd");
  $dumpvars(0);

// CASE A: Four consecutive 1's with rst = 1.
  clock = 0;
  rst = 1;
  w = 1;
  #14 w = 0;

// CASE B: Four consecutive 1's with rst = 0.
  #4 rst = 0;
  w = 1;
  #8 w = 0;

// CASE C: Five consecutive 1's with rst = 0.
  #4 w = 1;
  #10 w = 0;

  #6 $finish;
end

always begin
 #1 clock = ~clock;
end

endmodule

//------------ LATCH D --------------------------
// Obtenido del ejemplo en clase

module latch_d (q,d,c);

  output q;
  wire q, qn;
  input d, c;

  //Latch1 D
  nand U1 (x,d,c);
  nand U2 (y,~d,c);
  or   U3 (q,~qn,~x);
  or   U4 (qn,~q,~y);

endmodule

//------------ Flip Flop D --------------------------
// Obtenido del ejemplo en clase
module ff_d (q,d,clk);

  input clk;
  input d;
  wire q_tmp; 
  output q;

  latch_d latch1 (q_tmp,d,~clk);
  latch_d latch2 (q,q_tmp,clk);

endmodule
