#ifndef NANO_H
#define NANO_H

#include "drivers/gpio/gpio.h"

// Arduino Nano Pin Mappings
// Professional driver implementation: definitions for silk screen labels

// Digital Pins
#define D0  GPIO_PORTD, 0  // RX
#define D1  GPIO_PORTD, 1  // TX
#define D2  GPIO_PORTD, 2
#define D3  GPIO_PORTD, 3
#define D4  GPIO_PORTD, 4
#define D5  GPIO_PORTD, 5
#define D6  GPIO_PORTD, 6
#define D7  GPIO_PORTD, 7

#define D8  GPIO_PORTB, 0
#define D9  GPIO_PORTB, 1
#define D10 GPIO_PORTB, 2
#define D11 GPIO_PORTB, 3
#define D12 GPIO_PORTB, 4
#define D13 GPIO_PORTB, 5  // Onboard LED

// Analog Pins (usable as Digital GPIO)
#define A0  GPIO_PORTC, 0
#define A1  GPIO_PORTC, 1
#define A2  GPIO_PORTC, 2
#define A3  GPIO_PORTC, 3
#define A4  GPIO_PORTC, 4
#define A5  GPIO_PORTC, 5

// Note: A6 and A7 on Nano are Analog Input ONLY. 
// They are not supported by the digital GPIO driver.

// Aliases
#define LED_BUILTIN D13
#define RX          D0
#define TX          D1

#endif // NANO_H