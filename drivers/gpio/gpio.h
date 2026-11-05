#ifndef GPIO_H
#define GPIO_H

#include <stdint.h>
#include <avr/io.h>

/**
 * @file gpio.h
 * @brief General Purpose Input/Output (GPIO) Driver for ATmega2560 (C.A.M.P.I. Master).
 * 
 * Expanded to support all ports (PORTA through PORTL) of the ATmega2560 microcontroller
 * and internal Pull-Up configuration.
 */

// Port definitions for ATmega2560
typedef enum {
    GPIO_PORTA = 0,
    GPIO_PORTB = 1,
    GPIO_PORTC = 2,
    GPIO_PORTD = 3,
    GPIO_PORTE = 4,
    GPIO_PORTF = 5,
    GPIO_PORTG = 6,
    GPIO_PORTH = 7,
    GPIO_PORTJ = 8,
    GPIO_PORTK = 9,
    GPIO_PORTL = 10
} gpio_port_t;

// Pin direction definitions
typedef enum {
    GPIO_INPUT = 0,
    GPIO_OUTPUT = 1,
    GPIO_INPUT_PULLUP = 2
} gpio_dir_t;

// Pin state definitions
typedef enum {
    GPIO_LOW = 0,
    GPIO_HIGH = 1
} gpio_state_t;

// Function Prototypes
void GPIO_Init(gpio_port_t port, uint8_t pin, gpio_dir_t dir);
void GPIO_Write(gpio_port_t port, uint8_t pin, gpio_state_t state);
gpio_state_t GPIO_Read(gpio_port_t port, uint8_t pin);
void GPIO_Toggle(gpio_port_t port, uint8_t pin);
void GPIO_SetPullUp(gpio_port_t port, uint8_t pin, uint8_t enable);

#endif // GPIO_H
