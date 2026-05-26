#include "i2c_slave.h"
#include "drivers/pompe/pompe.h"

// DEFINIRE: Variabilele trăiesc fizic aici
volatile uint8_t i2c_received_byte = 0;
volatile uint8_t i2c_data_ready = 0;

void i2c_slave_init(uint8_t address) {
    TWAR = (address << 1);
    TWCR = (1<<TWEN) | (1<<TWIE) | (1<<TWEA) | (1<<TWINT);
    sei();
}

ISR(TWI_vect) {
    uint8_t status = TWSR & 0xF8;
    if (status == 0x60 || status == 0x80) {
        i2c_received_byte = TWDR;
        i2c_data_ready = 1;
        TWCR |= (1<<TWINT) | (1<<TWEA);
    } else {
        TWCR |= (1<<TWINT) | (1<<TWEA);
    }
}