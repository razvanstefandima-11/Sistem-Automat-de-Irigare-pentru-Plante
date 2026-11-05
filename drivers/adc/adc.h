#ifndef ADC_H
#define ADC_H

#include <stdint.h>

/**
 * @file adc.h
 * @brief Analog-to-Digital Converter (ADC) Driver for ATmega2560 (C.A.M.P.I. Master).
 * 
 * Supports all 16 analog input channels (ADC0 - ADC15).
 * - Channels 0-7: PORTF (PF0 - PF7)
 * - Channels 8-15: PORTK (PK0 - PK7)
 * 
 * Configuration:
 * - Vref: AVCC (5V stable from on-board Buck converter)
 * - Prescaler: 128 (125kHz ADC clock @ 16MHz F_CPU)
 * - Resolution: 10-bit (0 - 1023)
 */

#define ADC_MAX_VALUE 1023
#define ADC_NUM_CHANNELS 16

/**
 * @brief Initializes the ADC peripheral.
 * Configures AVCC voltage reference, 128 prescaler, and executes a dummy conversion.
 */
void ADC_Init(void);

/**
 * @brief Reads a 10-bit conversion value from the specified ADC channel (0-15).
 * Handles the MUX5 bit in ADCSRB for channels 8-15 on the ATmega2560.
 * 
 * @param channel ADC channel (0 to 15).
 * @return uint16_t 10-bit conversion result (0 - 1023).
 */
uint16_t ADC_Read(uint8_t channel);

#endif // ADC_H
