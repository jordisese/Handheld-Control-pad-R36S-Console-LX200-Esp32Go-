#ifndef CONF_H_INCLUDED
//#define K36S
#define NORTH_BTN 14
#define SOUTH_BTN 15
#define WEST_BTN 17
#define EAST_BTN 16

#define NORTH_HAT 1
#define SOUTH_HAT 4
#define WEST_HAT 2
#define EAST_HAT 8
#define GUIDE_BTN 4
#define CENTER_BTN 6
#define SLEW_BTN 5
#define FIND_BTN 7

#ifdef K36S

#define JOY1_BTN 11
#define SELECT_BTN 8
#define START_BTN 9
#define X_BTN 2
#define Y_BTN 3
#define A_BTN 0
#define B_BTN  1
#define FN_BTN 13
//#
#define OPEN_DB "/roms/esp32go/deep.db3"
#else

#define FN_BTN 8
#define JOY1_BTN 9
#define X_BTN 3
#define Y_BTN 2
#define A_BTN 1
#define B_BTN  0
#define SELECT_BTN 8
//#define OPEN_DB "OpNGC.db"
#define OPEN_DB "deep.db3"



#endif // K36S
#define ESP_ADDRESS "192.168.1.17"
#ifndef CUSTOMTTF
#define DVSANSBOLD "/usr/share/fonts/truetype/dejavu/DejaVuSans-Bold.ttf"
#define GX_SIZE 49
#else
#define DVSANSBOLD "DejaVuSans-Bold.ttf"
#define GX_SIZE 50
#endif


#endif
