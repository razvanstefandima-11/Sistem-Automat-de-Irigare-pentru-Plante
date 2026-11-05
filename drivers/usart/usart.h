#ifndef USART_H
#define USART_H

#include <stdint.h>

/**
 * @file usart.h
 * @brief USART communication driver for AVR microcontrollers.
 *
 * Provides initialization, byte transmission, and byte reception
 * over the hardware USART peripheral (USART0).
 */

 /**
  * @brief Initializes USART with default settings (16MHz, 57600 baud).
  */
#define USART_Init_Default() USART_Init(16000000, 57600)

#define MAX_SIZE_RECEIVE_USART 50
#define TIMEOUT_USART 1000
  /**
   * @brief Initializes USART communication with the specified baud rate.
   *
   * @param fosc Oscillator frequency in Hz (e.g. 16000000 for 16MHz)
   * @param baud Desired baud rate (e.g. 57600)
   */
void USART_Init(unsigned long fosc, unsigned int baud);

/**
 * @brief Transmits a buffer of bytes over USART.
 *
 * @param data Pointer to the data buffer to transmit
 * @param size Number of bytes to send
 */
void USART_Transmit(void* data, uint8_t size);

/**
 * @brief Receives bytes over USART into a buffer until a newline,
 *        carriage return, or timeout. Result is always null-terminated.
 *
 * @param data    Pointer to destination buffer (minimum 51 bytes recommended)
 * @param timeout Maximum wait time in milliseconds before returning
 * @return        Number of bytes received, excluding the null terminator
 */
int USART_Receive(void* data);

#endif // USART_H