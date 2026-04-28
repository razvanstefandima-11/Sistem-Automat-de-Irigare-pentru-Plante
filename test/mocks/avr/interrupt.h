#ifndef _AVR_INTERRUPT_H_
#define _AVR_INTERRUPT_H_

// Mock Interrupt Macros
#define cli() 
#define sei() 
#define ISR(vector) void vector(void)

// Mock Vector Names (must match what's used in drivers)
#define INT0_vect mock_INT0_vect
#define INT1_vect mock_INT1_vect
#define TIMER0_COMPA_vect mock_TIMER0_COMPA_vect

// Global SREG mock variable for saving/restoring interrupt state
extern volatile uint8_t SREG;

#endif // _AVR_INTERRUPT_H_