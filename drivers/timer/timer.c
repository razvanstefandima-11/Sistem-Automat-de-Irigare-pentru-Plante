#include "timer.h"
#include <avr/io.h>
#include <avr/interrupt.h>

/**
 * @file timer.c
 * @brief System Tick Timer0 implementation.
 */

static volatile uint32_t system_millis = 0;

void Timer_Init(void) {
    // 1. Configure Timer0 in CTC Mode (WGM01 = 1, WGM00 = 0, WGM02 = 0)
    TCCR0A = (1 << WGM01);

    // 2. Set Compare Match value for 1ms interval:
    // F_CPU = 16,000,000 Hz, Prescaler = 64 -> Timer clock = 250,000 Hz (4us per tick)
    // 250 ticks * 4us = 1000us = 1ms -> OCR0A = 250 - 1 = 249
    OCR0A = 249;

    // 3. Clear initial timer counter
    TCNT0 = 0;

    // 4. Enable Output Compare Match A Interrupt
    TIMSK0 |= (1 << OCIE0A);

    // 5. Start timer with prescaler 64 (CS01 = 1, CS00 = 1)
    TCCR0B = (1 << CS01) | (1 << CS00);

    // 6. Enable global interrupts
    sei();
}

ISR(TIMER0_COMPA_vect) {
    system_millis++;
}

uint32_t Millis(void) {
    uint32_t m;
    uint8_t old_sreg = SREG;
    cli();
    m = system_millis;
    SREG = old_sreg;
    return m;
}

uint8_t Timer_HasElapsed(uint32_t start_time, uint32_t duration_ms) {
    return ((Millis() - start_time) >= duration_ms);
}

void Timer_DelayMs(uint32_t ms) {
    uint32_t start = Millis();
    while ((Millis() - start) < ms)
        ;
}
