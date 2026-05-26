#include "i2c_master.h"
#include <avr/io.h> // Avem nevoie de această bibliotecă pentru a accesa PORTC

void i2c_master_init(void) {
    // --- LINIA MAGICĂ ---
    // Activăm rezistențele interne de Pull-up pe pinii A4 (SDA) și A5 (SCL)
    PORTC |= (1 << PORTC4) | (1 << PORTC5);

    // Configurarea vitezei de transmisie
    TWBR = (uint8_t)((((F_CPU / F_SCL) / PRESCALER_1) - 16) / 2);
    TWSR &= ~((1<<TWPS1) | (1<<TWPS0));
}

// Funcție internă de Timeout (aprox. 5ms la 16MHz)
static uint8_t i2c_wait_timeout(void) {
    uint16_t timeout = 10000; 
    while (!(TWCR & (1<<TWINT))) {
        timeout--;
        if (timeout == 0) {
            return 0; // 0 înseamnă că Slave-ul nu a răspuns (Timeout)
        }
    }
    return 1; // 1 înseamnă Succes
}

uint8_t i2c_master_start(void) {
    TWCR = (1<<TWINT) | (1<<TWSTA) | (1<<TWEN);
    return i2c_wait_timeout();
}

void i2c_master_stop(void) {
    TWCR = (1<<TWINT) | (1<<TWSTO) | (1<<TWEN);
}

uint8_t i2c_master_write(uint8_t data) {
    TWDR = data;
    TWCR = (1<<TWINT) | (1<<TWEN);
    return i2c_wait_timeout();
}