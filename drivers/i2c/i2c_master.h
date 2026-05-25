#ifndef I2C_SLAVE_H
#define I2C_SLAVE_H

#ifndef F_CPU
#define F_CPU 16000000UL
#endif

#include <avr/io.h>
#include <avr/interrupt.h>

// Expunem variabilele global pentru a fi vizibile în tot proiectul
extern volatile uint8_t i2c_received_byte;
extern volatile uint8_t i2c_data_ready;

void i2c_slave_init(uint8_t address);

#endif /* I2C_SLAVE_H */