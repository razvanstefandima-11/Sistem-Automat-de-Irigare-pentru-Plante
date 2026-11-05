#ifndef I2C_H
#define I2C_H

#include <stdint.h>

/**
 * @file i2c.h
 * @brief Master Hardware Two-Wire Interface (TWI/I2C) Driver for ATmega2560.
 * 
 * Hardware Pins on ATmega2560:
 *  - SCL: PD0 (Pin 21)
 *  - SDA: PD1 (Pin 20)
 */

#define I2C_WRITE 0
#define I2C_READ  1

/**
 * @brief Initializes the hardware I2C master peripheral.
 * 
 * @param frequency_hz Desired SCL frequency (e.g. 100000UL for 100kHz, 400000UL for 400kHz).
 */
void I2C_Init(uint32_t frequency_hz);

/**
 * @brief Sends an I2C START condition followed by the slave address.
 * 
 * @param slave_address 7-bit slave address.
 * @param rw I2C_WRITE (0) or I2C_READ (1).
 * @return uint8_t 1 on ACK received, 0 on failure/timeout.
 */
uint8_t I2C_Start(uint8_t slave_address, uint8_t rw);

/**
 * @brief Sends an I2C STOP condition and releases the bus.
 */
void I2C_Stop(void);

/**
 * @brief Transmits a single byte over the I2C bus.
 * 
 * @param data Byte to send.
 * @return uint8_t 1 on ACK received, 0 on NACK/timeout.
 */
uint8_t I2C_Write(uint8_t data);

/**
 * @brief Reads a byte from the bus and acknowledges receipt (ACK).
 * 
 * @return uint8_t Received byte.
 */
uint8_t I2C_ReadAck(void);

/**
 * @brief Reads a byte from the bus without acknowledging (NACK - last byte).
 * 
 * @return uint8_t Received byte.
 */
uint8_t I2C_ReadNack(void);

#endif // I2C_H
