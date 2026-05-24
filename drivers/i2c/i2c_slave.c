#include "i2c_slave.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include "drivers/pompe/pompe.h" 

// ... (funcția I2C_Slave_Init rămâne la fel) ...

ISR(TWI_vect) {
    uint8_t status = TWSR & 0xF8;

    if (status == 0x60 || status == 0x80) {
        uint8_t comanda = TWDR;
        
        switch (comanda) {
            case 0x00: 
                Pompe_OprireUrgenta(); // Rezervor gol! Oprește tot!
                break;
                
            case 0x01: Pompa1_On();  break; // Udă Planta 1
            case 0x11: Pompa1_Off(); break; // Oprește apa Planta 1
                
            case 0x02: Pompa2_On();  break; // Udă Planta 2
            case 0x12: Pompa2_Off(); break; // Oprește apa Planta 2
                
            case 0x03: Pompa3_On();  break; // Udă Planta 3
            case 0x13: Pompa3_Off(); break; // Oprește apa Planta 3
        }
    }
    
    // Curățăm flag-ul și așteptăm următoarea comandă
    TWCR = (1 << TWINT) | (1 << TWEA) | (1 << TWEN) | (1 << TWIE);
}