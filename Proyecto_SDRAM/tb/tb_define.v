// Core Parameters

/*  SDRAM DATA Width Selection:
     8 ->  8 bit SDRAM Mode
    16 -> 16 bit SDRAM Mode
    32 -> 32 bit SDRAM Mode   */
`define SDR_DW    32

/*  SDRAM BYTE Width Selection:
    1 ->  8 bit SDRAM Mode
    2 -> 16 bit SDRAM Mode
    4 -> 32 bit SDRAM Mode   */
`define SDR_BW    4

`define dw        32 // Data width
`define tw        8  // Tag id width
`define bl        5  // Burst Lenght Width
`define APP_AW    26 // Application Address Width

`define SDR_RFSH_TIMER_W    12
`define SDR_RFSH_ROW_CNT_W  3

`define P_SYS     10 // System Clock Period -> 100MHz
`define P_SDR     20 // SDRAM Clock Period  -> 50MHz
