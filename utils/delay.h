#ifndef DELAY_H
#define DELAY_H

#include <stdint.h>

/**
 * @brief Blocking delay function using software loop and NOPs.
 * 
 * @param ms Number of milliseconds to delay.
 */
void Delay(uint32_t ms);

#endif // DELAY_H