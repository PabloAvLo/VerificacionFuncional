// Core Parameters

/*  SDRAM DATA Width Selection:
     8 ->  8 bit SDRAM Mode
    16 -> 16 bit SDRAM Mode
    32 -> 32 bit SDRAM Mode   */
#define SDR_DW   8

/*  SDRAM BYTE Width Selection:
    1 ->  8 bit SDRAM Mode
    2 -> 16 bit SDRAM Mode
    4 -> 32 bit SDRAM Mode   */
#define SDR_BW   1

#define DW       32 // Data width
#define APP_AW   26 // Application Address Width

#define SDR_RFSH_TIMER_W    12
#define SDR_RFSH_ROW_CNT_W  3
