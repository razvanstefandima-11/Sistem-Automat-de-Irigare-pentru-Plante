#ifndef TIMER_H
#define TIMER_H

#include <stdint.h>

/**
 * @file timer.h
 * @brief System Tick Timer Driver for ATmega2560 (C.A.M.P.I. Master).
 * 
 * Configures 8-bit Timer0 in CTC mode to generate a precise 1ms interrupt.
 * Provides a non-blocking Millis() equivalent for timekeeping and scheduling.
 */

/**
 * @brief Initializes Timer0 as the 1ms System Tick source.
 */
void Timer_Init(void);

/**
 * @brief Returns the number of milliseconds elapsed since system startup.
 * Executed as an atomic read.
 * 
 * @return uint32_t Elapsed milliseconds.
 */
uint32_t Millis(void);

/**
 * @brief Non-blocking delay comparison helper.
 * 
 * @param start_time Timestamp obtained from Millis().
 * @param duration_ms Desired duration in ms.
 * @return 1 if duration has elapsed, 0 otherwise.
 */
uint8_t Timer_HasElapsed(uint32_t start_time, uint32_t duration_ms);

/**
 * @brief Blocking delay using system ticks.
 * 
 * @param ms Delay time in milliseconds.
 */
void Timer_DelayMs(uint32_t ms);

#endif // TIMER_H
