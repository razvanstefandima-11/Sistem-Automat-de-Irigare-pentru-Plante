#include "adc.h"
#include <avr/io.h>

/**
 * @brief Initializes the Analog-to-Digital Converter.
 * 
 * Configuration:
 * - Reference Voltage: AVCC (5V on Arduino Nano).
 * - Prescaler: 128 (Resulting clock: 125kHz @ 16MHz).
 * - Status: Enabled.
 */
void ADC_Init(void) {
    // Select Vref = AVCC
    ADMUX = (1 << REFS0);

    // Set Prescaler to 128 (16MHz/128 = 125kHz)
    ADCSRA = (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);

    // Enable ADC
    ADCSRA |= (1 << ADEN);

    // Dummy conversion after enabling ADC
    while (ADCSRA & (1 << ADSC));
    (void)ADCL;
    (void)ADCH;
}

/**
 * @brief Reads an analog value from the specified channel.
 * 
 * This is a blocking function. It starts the conversion and waits
 * until the hardware flag (ADSC) clears, indicating completion.
 * 
 * @param channel The ADC channel (0-7).
 * @return uint16_t The 10-bit conversion result (0-1023).
 */
uint16_t ADC_Read(uint8_t channel) {
    // Select ADC Channel (0-7), clearing the MUX bits first
    // 0xF0 mask preserves REFS0/1 (Voltage Ref) and ADLAR (Left Adjust)
    ADMUX = (ADMUX & 0xF0) | (channel & 0x07);

    // Start Conversion
    ADCSRA |= (1 << ADSC);

    // Wait for conversion to complete
    while (ADCSRA & (1 << ADSC));

    // Return the ADC result
    uint8_t low  = ADCL;
    uint8_t high = ADCH;

    return ((uint16_t) high << 8) | low;
}