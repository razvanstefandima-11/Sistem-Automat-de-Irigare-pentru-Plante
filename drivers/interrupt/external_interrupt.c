#include "external_interrupt.h"
#include <stddef.h>

// Static callback pointers for ISR usage
static void (*int0_callback)(void) = NULL;
static void (*int1_callback)(void) = NULL;

/**
 * @brief Initializes an External Interrupt (INT0 or INT1).
 * 
 * Configures the interrupt trigger mode (Low Level, Any Change, Falling Edge, Rising Edge)
 * and registers the user callback function.
 * Disables global interrupts during configuration to ensure atomicity.
 * 
 * @param interrupt_num The interrupt to configure (INT_0 or INT_1).
 * @param trigger The trigger mode (e.g., EXT_INT_FALLING_EDGE).
 * @param callback Pointer to the function to be called when interrupt occurs.
 */
void ExtInt_Init(uint8_t interrupt_num, ext_int_trigger_t trigger, void (*callback)(void)) {
    cli();

    if (interrupt_num == INT_0) {
        // Configure ISC01, ISC00 in EICRA
        EICRA &= ~(0x03);
        EICRA |= (trigger << ISC00);
        int0_callback = callback;
        EIMSK |= (1 << INT0);
    } else if (interrupt_num == INT_1) {
        // Configure ISC11, ISC10 in EICRA
        EICRA &= ~(0x0C);
        EICRA |= (trigger << ISC10);
        int1_callback = callback;
        EIMSK |= (1 << INT1);
    }

    sei();
}

/**
 * @brief Enables the specific external interrupt.
 * 
 * Sets the corresponding bit in the EIMSK register.
 * 
 * @param interrupt_num INT_0 or INT_1.
 */
void ExtInt_Enable(uint8_t interrupt_num) {
    if (interrupt_num == INT_0) {
        EIMSK |= (1 << INT0);
    } else if (interrupt_num == INT_1) {
        EIMSK |= (1 << INT1);
    }
}

/**
 * @brief Disables the specific external interrupt.
 * 
 * Clears the corresponding bit in the EIMSK register.
 * 
 * @param interrupt_num INT_0 or INT_1.
 */
void ExtInt_Disable(uint8_t interrupt_num) {
    if (interrupt_num == INT_0) {
        EIMSK &= ~(1 << INT0);
    } else if (interrupt_num == INT_1) {
        EIMSK &= ~(1 << INT1);
    }
}

/**
 * @brief Interrupt Service Routine for External Interrupt 0.
 * Calls the registered user callback if it exists.
 */
ISR(INT0_vect) {
    if (int0_callback != NULL) {
        int0_callback();
    }
}

/**
 * @brief Interrupt Service Routine for External Interrupt 1.
 * Calls the registered user callback if it exists.
 */
ISR(INT1_vect) {
    if (int1_callback != NULL) {
        int1_callback();
    }
}