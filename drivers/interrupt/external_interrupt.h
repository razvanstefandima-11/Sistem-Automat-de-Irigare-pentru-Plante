#ifndef EXTERNAL_INTERRUPT_H
#define EXTERNAL_INTERRUPT_H

#include <stdint.h>
#include <avr/io.h>
#include <avr/interrupt.h>

// Interrupt definitions
#define INT_0 0
#define INT_1 1

// Trigger modes (based on EICRA register)
typedef enum {
    EXT_INT_LOW_LEVEL = 0,
    EXT_INT_ANY_CHANGE = 1,
    EXT_INT_FALLING_EDGE = 2,
    EXT_INT_RISING_EDGE = 3
} ext_int_trigger_t;

/**
 * @brief Initialize an external interrupt with a callback function.
 * 
 * @param interrupt_num INT_0 or INT_1
 * @param trigger Trigger mode (LOW, CHANGE, RISING, FALLING)
 * @param callback Function pointer to the ISR handler
 */
void ExtInt_Init(uint8_t interrupt_num, ext_int_trigger_t trigger, void (*callback)(void));

/**
 * @brief Enable the specified external interrupt.
 * 
 * @param interrupt_num INT_0 or INT_1
 */
void ExtInt_Enable(uint8_t interrupt_num);

/**
 * @brief Disable the specified external interrupt.
 * 
 * @param interrupt_num INT_0 or INT_1
 */
void ExtInt_Disable(uint8_t interrupt_num);

#endif // EXTERNAL_INTERRUPT_H