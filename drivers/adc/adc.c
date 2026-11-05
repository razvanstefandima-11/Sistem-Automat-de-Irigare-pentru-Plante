#include "adc.h"
#include <avr/io.h>

/**
 * @file adc.c
 * @brief Analog-to-Digital Converter implementation for ATmega2560 (C.A.M.P.I. Master).
 */

void ADC_Init(void) {
    // Select Vref = AVCC (5V reference)
    ADMUX = (1 << REFS0);

#ifndef UNIT_TEST
    // Ensure MUX5 is cleared (channels 0-7 by default)
    ADCSRB &= ~(1 << MUX5);
#endif

    // Set Prescaler to 128 (16MHz / 128 = 125kHz, inside optimal 50-200kHz range)
    // Enable ADC peripheral
    ADCSRA = (1 << ADEN) | (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);

    // Dummy conversion to initialize the analog circuitry
    ADCSRA |= (1 << ADSC);
    while (ADCSRA & (1 << ADSC))
        ;
    (void)ADCL;
    (void)ADCH;
}

uint16_t ADC_Read(uint8_t channel) {
    if (channel >= ADC_NUM_CHANNELS) {
        channel = 0;
    }

#ifndef UNIT_TEST
    // ATmega2560: Channels 8-15 require setting the MUX5 bit in ADCSRB
    if (channel >= 8) {
        ADCSRB |= (1 << MUX5);
        channel -= 8;
    } else {
        ADCSRB &= ~(1 << MUX5);
    }
#endif

    // Select channel (bits MUX2..0 in ADMUX).
    // Mask 0xE0 preserves REFS1, REFS0, and ADLAR
    ADMUX = (ADMUX & 0xE0) | (channel & 0x07);

    // Start conversion
    ADCSRA |= (1 << ADSC);

    // Wait for conversion to complete
    while (ADCSRA & (1 << ADSC))
        ;

    // Read low byte first, then high byte (or 16-bit register ADC)
    uint8_t low  = ADCL;
    uint8_t high = ADCH;

    return ((uint16_t)high << 8) | low;
}
