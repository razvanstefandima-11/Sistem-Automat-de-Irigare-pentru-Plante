#ifndef BSP_H
#define BSP_H

// Board Selection Logic
#if defined(BOARD_NANO)
    #include "nano.h"
#elif defined(BOARD_UNO)
    #include "uno.h"
#else
    #error "No Board Defined! Please define BOARD_NANO or BOARD_UNO."
#endif

#endif // BSP_H
