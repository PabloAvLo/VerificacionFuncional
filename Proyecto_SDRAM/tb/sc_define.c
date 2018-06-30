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

// Limits for constrains

#define MAX_DATA    511
#define MIN_DATA    0
#define MAX_ADDR    2048
#define MIN_ADDR    1024
#define MAX_BL      8
#define MIN_BL      1
#define LOW_LIM_BL  5
#define HIGH_LIM_BL 7
#define NO_VALID_BL 3
#define MIN_CYC     2
#define MAX_CYC     10
