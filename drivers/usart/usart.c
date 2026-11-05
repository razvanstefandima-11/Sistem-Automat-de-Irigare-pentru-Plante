/**
 * @file usart.c
 * @brief Asynchronous USART1 Driver implementation for ATmega2560 (C.A.M.P.I. Master).
 *
 * Implements interrupt-driven asynchronous reception using an efficient circular ring buffer.
 * Enables non-blocking command reception from the ESP32 IoT Gateway.
 */

#include "usart.h"
#include <avr/io.h>
#include <avr/interrupt.h>

#define USART1_RX_BUFFER_MASK (USART1_RX_BUFFER_SIZE - 1)

#if (USART1_RX_BUFFER_SIZE & USART1_RX_BUFFER_MASK)
#error "USART1_RX_BUFFER_SIZE must be a power of 2!"
#endif

// Circular RX Ring Buffer
static volatile uint8_t rx_buffer[USART1_RX_BUFFER_SIZE];
static volatile uint8_t rx_head = 0;
static volatile uint8_t rx_tail = 0;
static volatile uint8_t rx_overflow_flag = 0;

/**
 * @brief USART1 RX Complete Interrupt Service Routine.
 * 
 * Automatically triggered when a new byte arrives on the RX1 pin (Pin 19, PD2).
 * Reads the byte from UDR1 into the circular buffer.
 */
ISR(USART1_RX_vect) {
    uint8_t data = UDR1;
    uint8_t next_head = (rx_head + 1) & USART1_RX_BUFFER_MASK;

    if (next_head != rx_tail) {
        rx_buffer[rx_head] = data;
        rx_head = next_head;
    } else {
        // Buffer is full: incoming byte is dropped, set overflow flag
        rx_overflow_flag = 1;
    }
}

void USART1_Init(uint32_t fosc, uint32_t baud) {
    uint16_t ubrr;

    // Use Double Speed mode (U2X1 = 1) for baud rates >= 57600 to minimize divisor error
    if (baud >= 57600UL) {
        UCSR1A = (1 << U2X1);
        ubrr = (uint16_t)(((fosc + (4UL * baud)) / (8UL * baud)) - 1UL);
    } else {
        UCSR1A = 0;
        ubrr = (uint16_t)(((fosc + (8UL * baud)) / (16UL * baud)) - 1UL);
    }

    UBRR1H = (uint8_t)(ubrr >> 8);
    UBRR1L = (uint8_t)(ubrr & 0xFF);

    // Set frame format: Asynchronous, 8 data bits, 1 stop bit, no parity (8N1)
    UCSR1C = (1 << UCSZ11) | (1 << UCSZ10);

    // Reset ring buffer indices
    rx_head = 0;
    rx_tail = 0;
    rx_overflow_flag = 0;

    // Enable Receiver, Transmitter, and RX Complete Interrupt
    UCSR1B = (1 << RXEN1) | (1 << TXEN1) | (1 << RXCIE1);

    // Enable global interrupts so ISR can execute
    sei();
}

void USART1_TransmitChar(char c) {
    // Wait for empty transmit buffer (UDRE1 bit in UCSR1A)
    while (!(UCSR1A & (1 << UDRE1)))
        ;
    UDR1 = (uint8_t)c;
}

void USART1_Transmit(const void* data, uint16_t size) {
    const uint8_t* ptr = (const uint8_t*)data;
    for (uint16_t i = 0; i < size; i++) {
        USART1_TransmitChar((char)ptr[i]);
    }
}

void USART1_TransmitString(const char* str) {
    if (!str) return;
    while (*str) {
        USART1_TransmitChar(*str++);
    }
}

uint8_t USART1_Available(void) {
    uint8_t head = rx_head;
    uint8_t tail = rx_tail;
    return (head - tail) & USART1_RX_BUFFER_MASK;
}

int16_t USART1_Read(void) {
    if (rx_head == rx_tail) {
        return -1; // Empty buffer
    }

    uint8_t byte = rx_buffer[rx_tail];
    rx_tail = (rx_tail + 1) & USART1_RX_BUFFER_MASK;
    return (int16_t)byte;
}

int16_t USART1_Peek(void) {
    if (rx_head == rx_tail) {
        return -1; // Empty buffer
    }
    return (int16_t)rx_buffer[rx_tail];
}

uint8_t USART1_IsLineAvailable(void) {
    uint8_t tail = rx_tail;
    uint8_t head = rx_head;

    while (tail != head) {
        uint8_t c = rx_buffer[tail];
        if (c == '\n' || c == '\r') {
            return 1;
        }
        tail = (tail + 1) & USART1_RX_BUFFER_MASK;
    }
    return 0;
}

uint8_t USART1_ReadLine(char* buffer, uint8_t max_len) {
    if (!buffer || max_len < 2) {
        return 0;
    }

    // Do not consume bytes if a full line has not arrived yet (strictly non-blocking)
    if (!USART1_IsLineAvailable()) {
        return 0;
    }

    uint8_t idx = 0;
    while (rx_head != rx_tail && idx < (max_len - 1)) {
        uint8_t c = rx_buffer[rx_tail];
        rx_tail = (rx_tail + 1) & USART1_RX_BUFFER_MASK;

        // Handle line endings (\r, \n, or \r\n)
        if (c == '\r') {
            // Check if followed by \n and discard it as part of CRLF
            if (rx_head != rx_tail && rx_buffer[rx_tail] == '\n') {
                rx_tail = (rx_tail + 1) & USART1_RX_BUFFER_MASK;
            }
            break;
        }
        if (c == '\n') {
            break;
        }

        buffer[idx++] = (char)c;
    }

    buffer[idx] = '\0';
    return idx;
}

int USART1_Receive(void* buffer) {
    return (int)USART1_ReadLine((char*)buffer, MAX_SIZE_RECEIVE_USART);
}

void USART1_FlushRx(void) {
    uint8_t sreg = SREG;
    cli();
    rx_head = 0;
    rx_tail = 0;
    rx_overflow_flag = 0;
    SREG = sreg;
}

uint8_t USART1_CheckAndClearOverflow(void) {
    uint8_t overflow = rx_overflow_flag;
    rx_overflow_flag = 0;
    return overflow;
}
