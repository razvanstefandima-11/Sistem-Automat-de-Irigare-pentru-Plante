#include "i2c_slave.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include "drivers/gpio/gpio.h" // Pentru a acționa releele de pe Nano

void I2C_Slave_Init(uint8_t slave_addr) {
    // 1. TWAR (TWI Address Register)
    // Setăm adresa la care răspunde placa noastră Nano. 
    // Bitul 0 este pentru recunoașterea adreselor generale (General Call), îl lăsăm 0.
    TWAR = (slave_addr << 1);
    
    // 2. TWCR (TWI Control Register)
    // TWEN = Activează magistrala I2C.
    // TWEA = Activează răspunsul (ACK). Nano va zice "Prezent!" când își aude adresa.
    // TWIE = Activează întreruperile TWI. Permite saltul la funcția ISR de mai jos.
    TWCR = (1 << TWEN) | (1 << TWEA) | (1 << TWIE);
}

// =========================================================================
// RUTINA DE TRATARE A ÎNTRERUPERII (ISR - Interrupt Service Routine)
// Acest cod NU este apelat de tine din main(). Microcontroller-ul pune pauză
// programului principal și execută acest bloc automat când primește date.
// =========================================================================
ISR(TWI_vect) {
    // Citim statusul exact al comunicării.
    // Mascăm (ignorăm) primii 3 biți (cu 0xF8) deoarece aceia sunt pentru prescaler.
    uint8_t status = TWSR & 0xF8;

    // 0x60 înseamnă: "Am fost strigat pe nume și am dat un ACK".
    // 0x80 înseamnă: "Am primit un octet de date și am dat un ACK".
    if (status == 0x60 || status == 0x80) {
        
        // Citim "ordinul" primit de la Master (Uno) direct din registrul de date
        uint8_t comanda = TWDR;
        
        // Executăm partea de forță (managementul termic) pe baza comenzii
        switch (comanda) {
            case 0x00: 
                // AVARIE TERMICA: Oprește tot! (Failsafe)
                GPIO_Write(GPIO_PORTB, 0, GPIO_LOW); // Oprește Ventilator 1 (D8)
                GPIO_Write(GPIO_PORTB, 1, GPIO_LOW); // Oprește Sistemul de Încălzire (D9)
                GPIO_Write(GPIO_PORTB, 2, GPIO_LOW); // Oprește Ventilator 2 (D10)
                break;
                
            case 0x01:
                GPIO_Write(GPIO_PORTB, 0, GPIO_HIGH); // Pornește Ventilator 1 (Răcire)
                break;
                
            case 0x11:
                GPIO_Write(GPIO_PORTB, 0, GPIO_LOW);  // Oprește Ventilator 1
                break;
                
            case 0x02:
                GPIO_Write(GPIO_PORTB, 1, GPIO_HIGH); // Pornește Sistemul de Încălzire
                break;
                
            case 0x12:
                GPIO_Write(GPIO_PORTB, 1, GPIO_LOW);  // Oprește Sistemul de Încălzire
                break;
        }
    }
    
    // La finalul întreruperii, trebuie să curățăm "flag-ul" TWINT scriind un 1 pe el.
    // Asta eliberează magistrala și permite Nano-ului să asculte următoarea comandă.
    // Păstrăm biții de funcționare, răspuns și întrerupere activi!
    TWCR = (1 << TWINT) | (1 << TWEA) | (1 << TWEN) | (1 << TWIE);
}