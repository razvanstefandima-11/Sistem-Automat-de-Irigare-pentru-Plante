#include <stdio.h>
#include <assert.h>
#include "drivers/pwm/pwm.h"
#include "drivers/timer/timer1.h"
#include "drivers/timer/timer2.h"
#include "test/mocks/avr/io.h"
#include "bsp/uno.h"

// External helper to reset registers
void Reset_Registers(void);

void test_pwm_init_timer1(void) {
    Reset_Registers();
    printf("Testing PWM Init (Timer1 / D9)...\n");

    // Initialize Timer1 on D9 (PB1) for 50Hz
    // 50Hz
    // Algorithm:
    // P=1: TOP = 16M/50 - 1 = 319999 (Too big)
    // P=8: TOP = 2M/50 - 1 = 39999 (Fits!)
    // Should choose Prescaler 8.
    
    PWM_Init(GPIO_PORTB, 1, 50);

    // Verify Prescaler 8 (CS11) = 2
    // TCCR1B & 0x7 should be 2
    assert((TCCR1B & 0x07) == 2);

    // Verify TOP (ICR1)
    // 39999
    assert(ICR1 == 39999);

    printf("PWM Init Timer1 Passed.\n");
}

void test_pwm_init_timer2(void) {
    Reset_Registers();
    printf("Testing PWM Init (Timer2 / D11)...\n");

    // Initialize Timer2 on D11 (PB3) for ~1kHz
    // Timer2 is 8-bit, fixed TOP=255.
    // Freq = 16MHz / (Prescaler * 256)
    // 1000 = 16000000 / (P * 256)
    // P = 16000000 / 256000 = 62.5 -> Closest is 64
    PWM_Init(GPIO_PORTB, 3, 1000);

    // Verify Prescaler 64 (CS22) = 4 (1<<2)
    assert((TCCR2B & 0x07) == 4);

    printf("PWM Init Timer2 Passed.\n");
}

void test_pwm_duty_cycle(void) {
    Reset_Registers();
    printf("Testing PWM Duty Cycle...\n");

    // Init Timer1 first to set ICR1
    ICR1 = 1000;
    
    // Set 50% Duty on D9 (PB1) 
    // Duty 127/255 -> ~50%
    // Expected OCR1A = (127 * 1000) / 255 = 498
    PWM_SetDutyCycle(GPIO_PORTB, 1, 127);
    assert(OCR1A == 498);

    // Set 100% Duty
    PWM_SetDutyCycle(GPIO_PORTB, 1, 255);
    assert(OCR1A == 1000);

    printf("PWM Duty Cycle Passed.\n");
}

int main(void) {
    test_pwm_init_timer1();
    test_pwm_init_timer2();
    test_pwm_duty_cycle();
    
    printf("All PWM Wrapper Tests Passed!\n");
    return 0;
}