#ifndef BSP_H
#define BSP_H

// Board Selection Logic
#if defined(BOARD_MEGA2560) || defined(BOARD_MEGA)
    #include "mega2560.h"
#elif defined(BOARD_NANO)
    #include "nano.h"
#elif defined(BOARD_UNO) || defined(UNIT_TEST)
    #include "uno.h"
#else
    #error "No Board Defined! Please define BOARD_MEGA2560, BOARD_NANO, or BOARD_UNO."
#endif

#endif // BSP_H
