#include "gpio.h"
#include "utils.h"

/**
 * @file gpio.c
 * @brief GPIO driver implementation for ATmega2560 (C.A.M.P.I. Master).
 */

void GPIO_Init(gpio_port_t port, uint8_t pin, gpio_dir_t dir) {
    switch (port) {
#ifndef UNIT_TEST
        case GPIO_PORTA:
            if (dir == GPIO_OUTPUT) {
                SET_BIT(DDRA, pin);
            } else {
                CLEAR_BIT(DDRA, pin);
                if (dir == GPIO_INPUT_PULLUP) SET_BIT(PORTA, pin);
                else CLEAR_BIT(PORTA, pin);
            }
            break;
#endif
        case GPIO_PORTB:
            if (dir == GPIO_OUTPUT) {
                SET_BIT(DDRB, pin);
            } else {
                CLEAR_BIT(DDRB, pin);
                if (dir == GPIO_INPUT_PULLUP) SET_BIT(PORTB, pin);
                else CLEAR_BIT(PORTB, pin);
            }
            break;
        case GPIO_PORTC:
            if (dir == GPIO_OUTPUT) {
                SET_BIT(DDRC, pin);
            } else {
                CLEAR_BIT(DDRC, pin);
                if (dir == GPIO_INPUT_PULLUP) SET_BIT(PORTC, pin);
                else CLEAR_BIT(PORTC, pin);
            }
            break;
        case GPIO_PORTD:
            if (dir == GPIO_OUTPUT) {
                SET_BIT(DDRD, pin);
            } else {
                CLEAR_BIT(DDRD, pin);
                if (dir == GPIO_INPUT_PULLUP) SET_BIT(PORTD, pin);
                else CLEAR_BIT(PORTD, pin);
            }
            break;
#ifndef UNIT_TEST
        case GPIO_PORTE:
            if (dir == GPIO_OUTPUT) {
                SET_BIT(DDRE, pin);
            } else {
                CLEAR_BIT(DDRE, pin);
                if (dir == GPIO_INPUT_PULLUP) SET_BIT(PORTE, pin);
                else CLEAR_BIT(PORTE, pin);
            }
            break;
        case GPIO_PORTF:
            if (dir == GPIO_OUTPUT) {
                SET_BIT(DDRF, pin);
            } else {
                CLEAR_BIT(DDRF, pin);
                if (dir == GPIO_INPUT_PULLUP) SET_BIT(PORTF, pin);
                else CLEAR_BIT(PORTF, pin);
            }
            break;
        case GPIO_PORTG:
            if (dir == GPIO_OUTPUT) {
                SET_BIT(DDRG, pin);
            } else {
                CLEAR_BIT(DDRG, pin);
                if (dir == GPIO_INPUT_PULLUP) SET_BIT(PORTG, pin);
                else CLEAR_BIT(PORTG, pin);
            }
            break;
        case GPIO_PORTH:
            if (dir == GPIO_OUTPUT) {
                SET_BIT(DDRH, pin);
            } else {
                CLEAR_BIT(DDRH, pin);
                if (dir == GPIO_INPUT_PULLUP) SET_BIT(PORTH, pin);
                else CLEAR_BIT(PORTH, pin);
            }
            break;
        case GPIO_PORTJ:
            if (dir == GPIO_OUTPUT) {
                SET_BIT(DDRJ, pin);
            } else {
                CLEAR_BIT(DDRJ, pin);
                if (dir == GPIO_INPUT_PULLUP) SET_BIT(PORTJ, pin);
                else CLEAR_BIT(PORTJ, pin);
            }
            break;
        case GPIO_PORTK:
            if (dir == GPIO_OUTPUT) {
                SET_BIT(DDRK, pin);
            } else {
                CLEAR_BIT(DDRK, pin);
                if (dir == GPIO_INPUT_PULLUP) SET_BIT(PORTK, pin);
                else CLEAR_BIT(PORTK, pin);
            }
            break;
        case GPIO_PORTL:
            if (dir == GPIO_OUTPUT) {
                SET_BIT(DDRL, pin);
            } else {
                CLEAR_BIT(DDRL, pin);
                if (dir == GPIO_INPUT_PULLUP) SET_BIT(PORTL, pin);
                else CLEAR_BIT(PORTL, pin);
            }
            break;
#endif
        default:
            break;
    }
}

void GPIO_Write(gpio_port_t port, uint8_t pin, gpio_state_t state) {
    switch (port) {
#ifndef UNIT_TEST
        case GPIO_PORTA:
            WRITE_BIT(PORTA, pin, state);
            break;
#endif
        case GPIO_PORTB:
            WRITE_BIT(PORTB, pin, state);
            break;
        case GPIO_PORTC:
            WRITE_BIT(PORTC, pin, state);
            break;
        case GPIO_PORTD:
            WRITE_BIT(PORTD, pin, state);
            break;
#ifndef UNIT_TEST
        case GPIO_PORTE:
            WRITE_BIT(PORTE, pin, state);
            break;
        case GPIO_PORTF:
            WRITE_BIT(PORTF, pin, state);
            break;
        case GPIO_PORTG:
            WRITE_BIT(PORTG, pin, state);
            break;
        case GPIO_PORTH:
            WRITE_BIT(PORTH, pin, state);
            break;
        case GPIO_PORTJ:
            WRITE_BIT(PORTJ, pin, state);
            break;
        case GPIO_PORTK:
            WRITE_BIT(PORTK, pin, state);
            break;
        case GPIO_PORTL:
            WRITE_BIT(PORTL, pin, state);
            break;
#endif
        default:
            break;
    }
}

gpio_state_t GPIO_Read(gpio_port_t port, uint8_t pin) {
    switch (port) {
#ifndef UNIT_TEST
        case GPIO_PORTA:
            return CHECK_BIT(PINA, pin);
#endif
        case GPIO_PORTB:
            return CHECK_BIT(PINB, pin);
        case GPIO_PORTC:
            return CHECK_BIT(PINC, pin);
        case GPIO_PORTD:
            return CHECK_BIT(PIND, pin);
#ifndef UNIT_TEST
        case GPIO_PORTE:
            return CHECK_BIT(PINE, pin);
        case GPIO_PORTF:
            return CHECK_BIT(PINF, pin);
        case GPIO_PORTG:
            return CHECK_BIT(PING, pin);
        case GPIO_PORTH:
            return CHECK_BIT(PINH, pin);
        case GPIO_PORTJ:
            return CHECK_BIT(PINJ, pin);
        case GPIO_PORTK:
            return CHECK_BIT(PINK, pin);
        case GPIO_PORTL:
            return CHECK_BIT(PINL, pin);
#endif
        default:
            break;
    }
    return GPIO_LOW;
}

void GPIO_Toggle(gpio_port_t port, uint8_t pin) {
    switch (port) {
#ifndef UNIT_TEST
        case GPIO_PORTA:
            TOGGLE_BIT(PORTA, pin);
            break;
#endif
        case GPIO_PORTB:
            TOGGLE_BIT(PORTB, pin);
            break;
        case GPIO_PORTC:
            TOGGLE_BIT(PORTC, pin);
            break;
        case GPIO_PORTD:
            TOGGLE_BIT(PORTD, pin);
            break;
#ifndef UNIT_TEST
        case GPIO_PORTE:
            TOGGLE_BIT(PORTE, pin);
            break;
        case GPIO_PORTF:
            TOGGLE_BIT(PORTF, pin);
            break;
        case GPIO_PORTG:
            TOGGLE_BIT(PORTG, pin);
            break;
        case GPIO_PORTH:
            TOGGLE_BIT(PORTH, pin);
            break;
        case GPIO_PORTJ:
            TOGGLE_BIT(PORTJ, pin);
            break;
        case GPIO_PORTK:
            TOGGLE_BIT(PORTK, pin);
            break;
        case GPIO_PORTL:
            TOGGLE_BIT(PORTL, pin);
            break;
#endif
        default:
            break;
    }
}

void GPIO_SetPullUp(gpio_port_t port, uint8_t pin, uint8_t enable) {
    if (enable) {
        GPIO_Init(port, pin, GPIO_INPUT_PULLUP);
    } else {
        GPIO_Init(port, pin, GPIO_INPUT);
    }
}
