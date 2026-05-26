#include "drivers/timer/timer0.h"
#include "usart.h"
/**
 * @brief Initializes USART communication with the specified baud rate.
 *
 * @param fosc Oscillator frequency in Hz (e.g. 16000000 for 16MHz)
 * @param baud Desired baud rate (e.g. 57600)
 */
void USART_Init(unsigned long fosc, unsigned int baud)
{
    unsigned long ubrr = (fosc / (16UL * baud)) - 1;

    UBRR0H = (unsigned char)(ubrr >> 8);
    UBRR0L = (unsigned char)(ubrr);

    UCSR0B = (1 << RXEN0) | (1 << TXEN0);

    // 8 data bits, 1 stop bit, no parity
    UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);
}

/**
 * @brief Transmits a buffer of bytes over USART.
 *
 * @param buffer Pointer to the data buffer to transmit
 * @param size   Number of bytes to send
 */
void USART_Transmit(void* buffer, uint8_t size)
{
    unsigned char* data = (unsigned char*)buffer;
    for (int i = 0; i < size; i++) {
        while (!(UCSR0A & (1 << UDRE0)))
            ;
        UDR0 = data[i];
    }
}

/**
 * @brief Receives bytes over USART into a buffer until a newline, carriage
 *        return, or 1000ms timeout. The result is always null-terminated.
 *
 * @param buffer Pointer to destination buffer (minimum 51 bytes recommended)
 * @return       Number of bytes received, excluding the null terminator
 */
int USART_Receive(void* buffer)
{
    unsigned char* data = (unsigned char*)buffer;
    int i = 0;
    uint32_t last_time = Millis();

    while ((Millis() - last_time <= TIMEOUT_USART) && (i < MAX_SIZE_RECEIVE_USART)) {
        if (UCSR0A & (1 << RXC0)) {
            data[i] = UDR0;
            last_time = Millis();

            if (data[i] == '\n' || data[i] == '\r') {
                break;
            }
            i++;
        }
    }

    data[i] = '\0';
    return i;
}