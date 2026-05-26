#include "i2c_slave.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include "bsp/nano.h"           // Aducem harta pinilor (A4, A5)
#include "drivers/gpio/gpio.h"  // Folosim driverul nostru uniform

// Variabile STATICE (ascunse) - pot fi vazute doar de acest fisier!
static volatile uint8_t internal_data_ready = 0;
static volatile uint8_t internal_received_byte = 0;

void i2c_slave_init(uint8_t address) {
    // 1. Activăm pull-up-urile folosind HAL-ul nostru pentru protectie suplimentara
    GPIO_Init(A4, GPIO_INPUT);
    GPIO_Write(A4, GPIO_HIGH);
    
    GPIO_Init(A5, GPIO_INPUT);
    GPIO_Write(A5, GPIO_HIGH);
    
    // 2. Setăm adresa Slave-ului (shiftată cu 1 bit la stânga)
    TWAR = (address << 1);
    
    // 3. Activăm I2C, Întreruperile și confirmarea (ACK)
    TWCR = (1 << TWEN) | (1 << TWIE) | (1 << TWEA) | (1 << TWINT);
}

// Această funcție citește datele protejându-le de rescrieri accidentale
uint8_t i2c_slave_has_data(uint8_t *data) {
    if (internal_data_ready) {
        // SECȚIUNE CRITICĂ: Oprim temporar întreruperile globale
        cli(); 
        
        *data = internal_received_byte; // Copiem byte-ul
        internal_data_ready = 0;        // Resetăm flag-ul
        
        // Repornim întreruperile globale
        sei(); 
        
        return 1; // Succes, avem date
    }
    return 0; // Nu avem date
}

// Vectorul de întrerupere hardware (curățat)
ISR(TWI_vect) {
    uint8_t status = TWSR & 0xF8;
    
    switch(status) {
        case 0x60: // Master-ul ne-a apelat cu propria adresa
            break; // Nu facem nimic deosebit, doar asteptam datele
            
        case 0x80: // Am receptionat un byte de la Master
            internal_received_byte = TWDR; 
            internal_data_ready = 1;       
            break;
            
        case 0xA0: // Master-ul a terminat de transmis (conditia de STOP)
            break;
            
        default:
            break;
    }
    
    // Eliberăm magistrala și ne pregătim pentru următoarea operațiune.
    // Scriind asta o singura data la final (DRY), economisim memorie flash.
    TWCR = (1 << TWEN) | (1 << TWIE) | (1 << TWINT) | (1 << TWEA);
}