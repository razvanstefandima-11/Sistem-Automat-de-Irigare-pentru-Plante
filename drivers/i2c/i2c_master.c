#include "i2c_master.h"

void i2c_master_init(void) {
    // Seteaza bit-rate-ul in registrul TWBR pe baza F_CPU si F_SCL
    TWBR = (uint8_t)((((F_CPU / F_SCL) / PRESCALER_1) - 16) / 2);
    // Prescaler-ul este 00 in TWSR (Factorul de divizare = 1)
    TWSR &= ~((1<<TWPS1) | (1<<TWPS0));
}

void i2c_master_start(void) {
    // Trimite conditia de START
    TWCR = (1<<TWINT) | (1<<TWSTA) | (1<<TWEN);
    // Asteapta finalizarea transmisiei semnalului START
    while (!(TWCR & (1<<TWINT)));
}

void i2c_master_stop(void) {
    // Trimite conditia de STOP
    TWCR = (1<<TWINT) | (1<<TWSTO) | (1<<TWEN);
}

void i2c_master_write(uint8_t data) {
    // Incarca datele in registrul TWDR
    TWDR = data;
    // Clear TWINT pentru a incepe transmisia byte-ului
    TWCR = (1<<TWINT) | (1<<TWEN);
    // Asteapta pana cand flag-ul de intrerupere se seteaza din nou (transmisie reusita)
    while (!(TWCR & (1<<TWINT)));
}

uint8_t i2c_master_read_ack(void) {
    // Citeste datele si trimite ACK catre Slave pentru a continua receptia
    TWCR = (1<<TWINT) | (1<<TWEN) | (1<<TWEA);
    while (!(TWCR & (1<<TWINT)));
    return TWDR;
}

uint8_t i2c_master_read_nack(void) {
    // Citeste ultimul byte de date fara a mai trimite ACK (NACK trimis catre Slave)
    TWCR = (1<<TWINT) | (1<<TWEN);
    while (!(TWCR & (1<<TWINT)));
    return TWDR;
}
