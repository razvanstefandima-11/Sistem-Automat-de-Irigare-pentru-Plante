#ifndef I2C_MASTER_H_
#define I2C_MASTER_H_

#ifndef F_CPU
#define F_CPU 16000000UL
#endif

#include <avr/io.h>

#define F_SCL 100000UL // Frecvența I2C Standard: 100 kHz
#define PRESCALER_1 1

void i2c_master_init(void);
uint8_t i2c_master_start(void); // Modificat să returneze status (pentru timeout)
void i2c_master_stop(void);
uint8_t i2c_master_write(uint8_t data); // Modificat să returneze status

#endif /* I2C_MASTER_H_ */