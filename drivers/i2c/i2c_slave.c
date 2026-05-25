#include "i2c_slave.h"

// Buffer volatil partajat cu rutina de intrerupere
extern volatile uint8_t i2c_received_byte;
extern volatile uint8_t i2c_data_ready;

void i2c_slave_init(uint8_t address) {
    // Incarca adresa in registrul TWAR (shiftata la stanga cu 1 bit)
    TWAR = (address << 1);
    
    // Configurare TWCR:
    // TWEN  - Activeaza modulul TWI
    // TWIE  - Activeaza intreruperile hardware TWI
    // TWEA  - Activeaza recunoasterea propriei adrese prin generare de ACK
    // TWINT - Curata flag-ul initial pentru a deschide bus-ul
    TWCR = (1<<TWEN) | (1<<TWIE) | (1<<TWEA) | (1<<TWINT);
    
    // Activeaza intreruperile globale
    sei();
}

ISR(TWI_vect) {
    // Citeste bitii de status ai magistralei, ignorand bitii de prescaler (0xF8)
    uint8_t status = TWSR & 0xF8;
    
    switch(status) {
        case 0x60: // SLA+W primit, ACK a fost trimis
        case 0x68: // Master a pierdut arbitrajul, SLA+W primit, ACK trimis
            // Pregateste hardware-ul pentru urmatorul byte
            TWCR |= (1<<TWINT) | (1<<TWEA);
            break;
            
        case 0x80: // Date primite anterior prin adresa proprie, ACK trimis
            i2c_received_byte = TWDR; // Salveaza datele din buffer-ul hardware
            i2c_data_ready = 1;       // Seteaza flag software pentru main
            TWCR |= (1<<TWINT) | (1<<TWEA);
            break;
            
        case 0xA0: // Conditie de STOP sau REPEATED START receptionata in timp ce era Slave
            TWCR |= (1<<TWINT) | (1<<TWEA);
            break;
            
        default:
            // Pentru orice alt status neprevazut, reseteaza flag-ul si mentine ACK
            TWCR |= (1<<TWINT) | (1<<TWEA);
            break;
    }
}
