#include <stdio.h>
#include <assert.h>
#include "drivers/timer/timer1.h"
#include "drivers/timer/timer2.h"
#include "test/mocks/avr/io.h"

// External helper to reset registers
void Reset_Registers(void);

void test_timer1_init(void) {
    Reset_Registers();
    printf("Testing Timer1 Init...\n");

    // Initialize with Prescaler 64 and TOP 20000 (e.g., 50Hz at 1MHz)
    Timer1_FastPWM_Init(64, 20000);

    // Verify TCCR1A (COM1A1 | COM1B1 | WGM11)
    // COM1A1=7 -> bit 7, COM1B1=5 -> bit 5, WGM11=1 -> bit 1
    // 10100010 = 0xA2
    // Wait, WGM11 is bit 1. (1<<1) = 2.
    // (1<<7) | (1<<5) | (1<<1) = 128 + 32 + 2 = 162 = 0xA2
    assert(TCCR1A == ((1 << COM1A1) | (1 << COM1B1) | (1 << WGM11)));

    // Verify TCCR1B (WGM13 | WGM12 | CS11 | CS10)
    // WGM13=4 -> bit 4, WGM12=3 -> bit 3
    // CS11=1 -> bit 1, CS10=0 -> bit 0 (for prescaler 64)
    // (1<<4) | (1<<3) | (1<<1) | (1<<0) = 16 + 8 + 2 + 1 = 27 = 0x1B
    assert(TCCR1B == ((1 << WGM13) | (1 << WGM12) | (1 << CS11) | (1 << CS10)));

    // Verify ICR1
    assert(ICR1 == 20000);

    // Verify DDRB
    assert((DDRB & (1 << DDB1)) != 0);
    assert((DDRB & (1 << DDB2)) != 0);

    printf("Timer1 Init Passed.\n");
}

void test_timer1_duty(void) {
    printf("Testing Timer1 Duty Cycle...\n");
    Reset_Registers();
    ICR1 = 1000;

    Timer1_SetDutyCycleA(500);
    assert(OCR1A == 500);

    Timer1_SetDutyCycleA(1500); // Should clamp
    assert(OCR1A == 1000);

    printf("Timer1 Duty Cycle Passed.\n");
}

void test_timer2_init(void) {
    Reset_Registers();
    printf("Testing Timer2 Init...\n");

    // Initialize with Prescaler 64
    Timer2_FastPWM_Init(64);

    // Verify TCCR2A (COM2A1 | COM2B1 | WGM21 | WGM20)
    // WGM21=1, WGM20=0
    // (1<<7) | (1<<5) | (1<<1) | (1<<0) = 128 + 32 + 2 + 1 = 163 = 0xA3
    assert(TCCR2A == ((1 << COM2A1) | (1 << COM2B1) | (1 << WGM21) | (1 << WGM20)));

    // Verify TCCR2B (CS22 only for 64)
    // CS22=2
    // (1<<2) = 4
    assert(TCCR2B == (1 << CS22));

    // Verify DDR
    assert((DDRB & (1 << DDB3)) != 0);
    assert((DDRD & (1 << DDD3)) != 0);

    printf("Timer2 Init Passed.\n");
}

int main(void) {
    test_timer1_init();
    test_timer1_duty();
    test_timer2_init();
    
    printf("All PWM Tests Passed!\n");
    return 0;
}