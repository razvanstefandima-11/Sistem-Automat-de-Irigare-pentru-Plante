#ifndef TIMER0_H
#define TIMER0_H

#include <stdint.h>
#include <avr/io.h>
#include <avr/interrupt.h>

/**
 * @brief Initialize Timer0 as a System Tick generator (1ms).
 * Configures Timer0 in CTC mode or Normal mode to generate 
 * an interrupt every 1ms.
 */
void Timer0_Init(void);

/**
 * @brief Get the number of milliseconds since startup.
 * 
 * @return uint32_t Milliseconds
 */
uint32_t Millis(void);

#endif // TIMER0_H