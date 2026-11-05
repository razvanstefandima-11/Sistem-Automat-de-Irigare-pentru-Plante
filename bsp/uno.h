#ifndef UNO_H
#define UNO_H

#include "drivers/gpio/gpio.h"

// Arduino Uno Pin Mappings

// Digital Pins
#define UNO_D0  GPIO_PORTD, 0
#define UNO_D1  GPIO_PORTD, 1
#define UNO_D2  GPIO_PORTD, 2
#define UNO_D3  GPIO_PORTD, 3
#define UNO_D4  GPIO_PORTD, 4
#define UNO_D5  GPIO_PORTD, 5
#define UNO_D6  GPIO_PORTD, 6
#define UNO_D7  GPIO_PORTD, 7

#define UNO_D8  GPIO_PORTB, 0
#define UNO_D9  GPIO_PORTB, 1
#define UNO_D10 GPIO_PORTB, 2
#define UNO_D11 GPIO_PORTB, 3
#define UNO_D12 GPIO_PORTB, 4
#define UNO_D13 GPIO_PORTB, 5 // Onboard LED

// Analog Input Pins (can be used as digital GPIO)
#define UNO_A0  GPIO_PORTC, 0
#define UNO_A1  GPIO_PORTC, 1
#define UNO_A2  GPIO_PORTC, 2
#define UNO_A3  GPIO_PORTC, 3
#define UNO_A4  GPIO_PORTC, 4
#define UNO_A5  GPIO_PORTC, 5

// Onboard LED
#define LED_BUILTIN UNO_D13

#endif // UNO_H