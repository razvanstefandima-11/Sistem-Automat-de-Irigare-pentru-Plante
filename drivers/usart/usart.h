#ifndef USART_H
#define USART_H

#include <stdint.h>
#include <stdbool.h>

/**
 * @file usart.h
 * @brief Asynchronous USART1 Communication Driver for ATmega2560 (C.A.M.P.I. Master).
 *
 * Configured for USART1 hardware peripheral with interrupt-driven (ISR) reception.
 * Interfaces with the ESP32 IoT Gateway via bidirectional level shifter.
 *
 * Hardware Pin Mapping (ATmega2560 / Arduino Mega):
 *  - TX1: Pin 18 (PD3 / TXD1) -> Level Shifter -> ESP32 RX
 *  - RX1: Pin 19 (PD2 / RXD1) <- Level Shifter <- ESP32 TX
 */

/* Buffer configuration */
#define USART1_RX_BUFFER_SIZE  128  /**< Ring buffer size (must be a power of 2) */
#define USART1_DEFAULT_BAUD    115200UL
#define USART1_DEFAULT_FOSC    16000000UL

#define MAX_SIZE_RECEIVE_USART 64
#define TIMEOUT_USART          1000

/**
 * @brief Initializes hardware USART1 for asynchronous serial communication (8N1)
 *        with RX Complete Interrupt enabled.
 *
 * @param fosc Oscillator frequency in Hz (e.g. 16000000UL for 16MHz)
 * @param baud Desired baud rate (e.g. 115200 or 9600)
 */
void USART1_Init(uint32_t fosc, uint32_t baud);

/**
 * @brief Initializes USART1 with default settings (16MHz, 115200 baud).
 */
#define USART1_Init_Default() USART1_Init(USART1_DEFAULT_FOSC, USART1_DEFAULT_BAUD)

/**
 * @brief Transmits a single character over USART1 (blocking until transmitter ready).
 *
 * @param c Character to transmit.
 */
void USART1_TransmitChar(char c);

/**
 * @brief Transmits a buffer of bytes over USART1.
 *
 * @param data Pointer to data buffer.
 * @param size Number of bytes to transmit.
 */
void USART1_Transmit(const void* data, uint16_t size);

/**
 * @brief Transmits a null-terminated string over USART1.
 *
 * @param str Pointer to null-terminated string.
 */
void USART1_TransmitString(const char* str);

/**
 * @brief Returns the number of bytes currently available in the RX ring buffer.
 *
 * @return uint8_t Number of bytes available.
 */
uint8_t USART1_Available(void);

/**
 * @brief Reads a single byte from the RX ring buffer without blocking.
 *
 * @return int16_t Byte read (0-255), or -1 if buffer is empty.
 */
int16_t USART1_Read(void);

/**
 * @brief Peeks at the next byte in the RX ring buffer without removing it.
 *
 * @return int16_t Byte (0-255), or -1 if buffer is empty.
 */
int16_t USART1_Peek(void);

/**
 * @brief Checks if a full command line (terminated by '\n' or '\r') is available in the buffer.
 *
 * @return uint8_t 1 if a full line is ready, 0 otherwise.
 */
uint8_t USART1_IsLineAvailable(void);

/**
 * @brief Reads a full line of text asynchronously from the RX buffer without blocking.
 *        If no complete line (terminated by '\n' or '\r') has arrived yet, returns 0 immediately.
 *        Terminating CRLF characters are stripped and a null terminator is added.
 *
 * @param buffer Destination buffer.
 * @param max_len Maximum length of destination buffer (including null terminator).
 * @return uint8_t Number of bytes written to buffer (excluding null terminator), or 0 if no line ready.
 */
uint8_t USART1_ReadLine(char* buffer, uint8_t max_len);

/**
 * @brief Non-blocking receive function for backward compatibility.
 *        Transfers a completed line into the buffer.
 *
 * @param buffer Destination buffer (minimum MAX_SIZE_RECEIVE_USART bytes recommended).
 * @return int Number of bytes received (0 if nothing ready).
 */
int USART1_Receive(void* buffer);

/**
 * @brief Flushes (empties) the RX ring buffer.
 */
void USART1_FlushRx(void);

/**
 * @brief Checks if an RX buffer overflow occurred and resets the overflow flag.
 *
 * @return uint8_t 1 if overflow occurred since last check, 0 otherwise.
 */
uint8_t USART1_CheckAndClearOverflow(void);

/* --- Legacy / Drop-in API Aliases for Backward Compatibility --- */
#define USART_Init(fosc, baud)    USART1_Init((fosc), (baud))
#define USART_Init_Default()      USART1_Init_Default()
#define USART_Transmit(data, sz)  USART1_Transmit((data), (sz))
#define USART_TransmitString(str) USART1_TransmitString((str))
#define USART_Receive(buf)        USART1_Receive((buf))

#endif // USART_H
