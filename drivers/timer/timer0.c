#include "timer0.h"

// System tick counter, increments every 1ms
static volatile uint32_t system_millis = 0;

/**
 * @brief Initializes Timer0 as a System Tick generator (1ms interval).
 * 
 * Configuration details:
 * - Mode: CTC (Clear Timer on Compare Match).
 * - Prescaler: 64.
 * - Frequency Calculation: 16MHz / 64 = 250kHz (4us per tick).
 * - Compare Value: 249 (250 ticks * 4us = 1000us = 1ms).
 * - Interrupt: Enabled on Compare Match A.
 */
void Timer0_Init(void) {
    // Set CTC mode
    TCCR0A |= (1 << WGM01);
    
    // Set Compare Match value for 1ms
    OCR0A = 249;
    
    // Enable Compare Match A Interrupt
    TIMSK0 |= (1 << OCIE0A);
    
    // Set Prescaler to 64
    TCCR0B |= (1 << CS01) | (1 << CS00);
    
    // Enable global interrupts
    sei();
}

/**
 * @brief Interrupt Service Routine for Timer0 Compare Match A.
 * 
 * Increments the system millisecond counter.
 * Triggered every 1ms.
 */
ISR(TIMER0_COMPA_vect) {
    system_millis++;
}

/**
 * @brief Retrieves the number of milliseconds since system startup.
 * 
 * Reads the 32-bit system_millis counter atomically by disabling 
 * interrupts during the read operation.
 * 
 * @return uint32_t Milliseconds elapsed.
 */
uint32_t Millis(void) {
    uint32_t m;
    
    // Atomic read
    uint8_t oldSREG = SREG;
    cli();
    m = system_millis;
    SREG = oldSREG;
    
    return m;
}