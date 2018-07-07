// Core Parameters

/*  SDRAM DATA Width Selection:
     8 ->  8 bit SDRAM Mode
    16 -> 16 bit SDRAM Mode
    32 -> 32 bit SDRAM Mode   */
#define SDR_DW   32

/*  SDRAM BYTE Width Selection:
    1 ->  8 bit SDRAM Mode
    2 -> 16 bit SDRAM Mode
    4 -> 32 bit SDRAM Mode   */
#define SDR_BW   4

#define DW       32 // Data width
#define APP_AW   26 // Application Address Width

#define SDR_RFSH_TIMER_W    12
#define SDR_RFSH_ROW_CNT_W  3

// Limits for constrains

#define MAX_DATA    0xFFFFFFFF
#define MIN_DATA    0
#define MAX_ADDR    0x003FFFFF
#define MIN_ADDR    0x0
#define MAX_BL      8
#define MIN_BL      1
// #define LOW_LIM_BL  5
// #define HIGH_LIM_BL 7
#define NO_VALID_BL1  3
#define NO_VALID_BL2  5
#define NO_VALID_BL3  6
#define NO_VALID_BL4  7
#define MIN_CYC     2
#define MAX_CYC     10
