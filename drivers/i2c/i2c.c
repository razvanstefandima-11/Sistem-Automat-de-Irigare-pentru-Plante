#include "i2c.h"
#include <avr/io.h>

/**
 * @file i2c.c
 * @brief Master hardware TWI implementation for ATmega2560.
 */

#define I2C_TIMEOUT 25000U

static uint8_t i2c_wait_twint(void) {
    uint16_t timeout = I2C_TIMEOUT;
    while (!(TWCR & (1 << TWINT))) {
        if (--timeout == 0) {
            return 0; // Hardware timeout
        }
    }
    return 1;
}

void I2C_Init(uint32_t frequency_hz) {
    // 1. Enable internal pull-ups on SDA (PD1) and SCL (PD0)
    PORTD |= (1 << PORTD1) | (1 << PORTD0);

    // 2. Set Prescaler = 1 (TWPS1 = 0, TWPS0 = 0)
    TWSR &= ~((1 << TWPS1) | (1 << TWPS0));

    // 3. Calculate Bit Rate Register (TWBR)
    // Formula: SCL = F_CPU / (16 + 2 * TWBR * Prescaler)
    // TWBR = ((F_CPU / SCL) - 16) / 2
    if (frequency_hz == 0) frequency_hz = 100000UL;
    uint32_t twbr_val = ((16000000UL / frequency_hz) - 16UL) / 2UL;
    if (twbr_val > 255) twbr_val = 255;
    TWBR = (uint8_t)twbr_val;

    // 4. Enable TWI peripheral
    TWCR = (1 << TWEN);
}

uint8_t I2C_Start(uint8_t slave_address, uint8_t rw) {
    // Transmit START condition
    TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN);
    if (!i2c_wait_twint()) return 0;

    // Check status register: 0x08 = START transmitted, 0x10 = Repeated START transmitted
    uint8_t status = TWSR & 0xF8;
    if (status != 0x08 && status != 0x10) {
        return 0;
    }

    // Send SLA + R/W
    TWDR = (slave_address << 1) | (rw & 0x01);
    TWCR = (1 << TWINT) | (1 << TWEN);
    if (!i2c_wait_twint()) return 0;

    // Check ACK received (0x18 = SLA+W ACK, 0x40 = SLA+R ACK)
    status = TWSR & 0xF8;
    if (status != 0x18 && status != 0x40) {
        return 0;
    }

    return 1;
}

void I2C_Stop(void) {
    TWCR = (1 << TWINT) | (1 << TWSTO) | (1 << TWEN);
    // Brief settling delay for bus release
    for (volatile uint8_t i = 0; i < 50; i++)
        ;
}

uint8_t I2C_Write(uint8_t data) {
    TWDR = data;
    TWCR = (1 << TWINT) | (1 << TWEN);
    if (!i2c_wait_twint()) return 0;

    // Check status: 0x28 = Data transmitted, ACK received
    uint8_t status = TWSR & 0xF8;
    return (status == 0x28);
}

uint8_t I2C_ReadAck(void) {
    TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWEA);
    if (!i2c_wait_twint()) return 0;
    return TWDR;
}

uint8_t I2C_ReadNack(void) {
    TWCR = (1 << TWINT) | (1 << TWEN);
    if (!i2c_wait_twint()) return 0;
    return TWDR;
}
