#ifndef GPIO_H
#define GPIO_H

#include <stdint.h>
#include <avr/io.h>

// Port definitions
typedef enum {
    GPIO_PORTB,
    GPIO_PORTC,
    GPIO_PORTD
} gpio_port_t;

// Pin direction definitions
typedef enum {
    GPIO_INPUT = 0,
    GPIO_OUTPUT = 1
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

#endif // GPIO_H