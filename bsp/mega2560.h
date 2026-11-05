#ifndef MEGA2560_H
#define MEGA2560_H

#include "drivers/gpio/gpio.h"

/**
 * @file mega2560.h
 * @brief Board Support Package (BSP) for Arduino Mega 2560 / ATmega2560 Master Unit.
 * 
 * Hardware definitions for C.A.M.P.I. Master Controller:
 *  - Communication: Hardware USART1 (TX1: Pin 18, RX1: Pin 19)
 *  - Onboard Status LED: Pin 13 (PB7)
 */

// UART Pins
#define USART1_TX_PIN   18  // PD3 (TXD1)
#define USART1_RX_PIN   19  // PD2 (RXD1)

// Legacy Uno/Nano Pin Compatibility for existing drivers during porting phase
#define UNO_D0   GPIO_PORTD, 0
#define UNO_D1   GPIO_PORTD, 1
#define UNO_D2   GPIO_PORTD, 2
#define UNO_D3   GPIO_PORTD, 3
#define UNO_D4   GPIO_PORTD, 4
#define UNO_D5   GPIO_PORTD, 5
#define UNO_D6   GPIO_PORTD, 6
#define UNO_D7   GPIO_PORTD, 7

#define UNO_D8   GPIO_PORTB, 0
#define UNO_D9   GPIO_PORTB, 1
#define UNO_D10  GPIO_PORTB, 2
#define UNO_D11  GPIO_PORTB, 3
#define UNO_D12  GPIO_PORTB, 4
#define UNO_D13  GPIO_PORTB, 7 // On ATmega2560, Pin 13 LED is on PB7 (unlike Uno PB5)

#define UNO_A0   GPIO_PORTC, 0
#define UNO_A1   GPIO_PORTC, 1
#define UNO_A2   GPIO_PORTC, 2
#define UNO_A3   GPIO_PORTC, 3
#define UNO_A4   GPIO_PORTC, 4
#define UNO_A5   GPIO_PORTC, 5

// Onboard LED
#define LED_BUILTIN  UNO_D13

#endif // MEGA2560_H

