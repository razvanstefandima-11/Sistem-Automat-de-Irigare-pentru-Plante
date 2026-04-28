#include "gpio.h"
#include "utils.h"

/**
 * @brief Initializes a specific GPIO pin with the given direction.
 * 
 * Configures the Data Direction Register (DDR) for the specified port.
 * 
 * @param port The GPIO port (GPIO_PORTB, GPIO_PORTC, GPIO_PORTD).
 * @param pin The pin number (0-7).
 * @param dir The direction (GPIO_INPUT or GPIO_OUTPUT).
 */
void GPIO_Init(gpio_port_t port, uint8_t pin, gpio_dir_t dir) {
    switch (port) {
        case GPIO_PORTB:
            if (dir == GPIO_OUTPUT) {
                SET_BIT(DDRB, pin);
            } else {
                CLEAR_BIT(DDRB, pin);
            }
            break;
        case GPIO_PORTC:
            if (dir == GPIO_OUTPUT) {
                SET_BIT(DDRC, pin);
            } else {
                CLEAR_BIT(DDRC, pin);
            }
            break;
        case GPIO_PORTD:
            if (dir == GPIO_OUTPUT) {
                SET_BIT(DDRD, pin);
            } else {
                CLEAR_BIT(DDRD, pin);
            }
            break;
    }
}

/**
 * @brief Writes a digital value (HIGH or LOW) to a GPIO pin.
 * 
 * Manipulates the PORT register to set the pin state.
 * 
 * @param port The GPIO port.
 * @param pin The pin number (0-7).
 * @param state The state to write (GPIO_LOW or GPIO_HIGH).
 */
void GPIO_Write(gpio_port_t port, uint8_t pin, gpio_state_t state) {
    switch (port) {
        case GPIO_PORTB:
            WRITE_BIT(PORTB, pin, state);
            break;
        case GPIO_PORTC:
            WRITE_BIT(PORTC, pin, state);
            break;
        case GPIO_PORTD:
            WRITE_BIT(PORTD, pin, state);
            break;
    }
}

/**
 * @brief Reads the logical state of a GPIO pin.
 * 
 * Reads from the PIN register to determing input state.
 * 
 * @param port The GPIO port.
 * @param pin The pin number (0-7).
 * @return gpio_state_t The current state (GPIO_LOW or GPIO_HIGH).
 */
gpio_state_t GPIO_Read(gpio_port_t port, uint8_t pin) {
    switch (port) {
        case GPIO_PORTB:
            return CHECK_BIT(PINB, pin);
        case GPIO_PORTC:
            return CHECK_BIT(PINC, pin);
        case GPIO_PORTD:
            return CHECK_BIT(PIND, pin);
    }
    return GPIO_LOW;
}

/**
 * @brief Toggles the state of a GPIO pin.
 * 
 * Inverts the current bit in the PORT register.
 * 
 * @param port The GPIO port.
 * @param pin The pin number (0-7).
 */
void GPIO_Toggle(gpio_port_t port, uint8_t pin) {
    switch (port) {
        case GPIO_PORTB:
            TOGGLE_BIT(PORTB, pin);
            break;
        case GPIO_PORTC:
            TOGGLE_BIT(PORTC, pin);
            break;
        case GPIO_PORTD:
            TOGGLE_BIT(PORTD, pin);
            break;
    }
}