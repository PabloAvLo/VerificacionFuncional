#ifndef SC_TB_PORTS_H
#define SC_TB_PORTS_H

// Define Complex Type of Input and Out for DUT
struct tagInput {
  unsigned long  clk;
  unsigned long  z;
};

struct tagOutput {
  unsigned long  rst;
  unsigned long  w;
  int done;
};

typedef struct tagInput    INVECTOR;
typedef struct tagOutput  OUTVECTOR;

#endif
