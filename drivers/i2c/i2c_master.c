#include "i2c_master.h"
#include <avr/io.h>
#include "bsp/uno.h"            // Aducem harta pinilor (UNO_A4, UNO_A5)
#include "drivers/gpio/gpio.h"  // Folosim driverul nostru uniform

void i2c_master_init(void) {
    // 1. Activăm pull-up-urile folosind HAL-ul nostru, pentru consistență!
    // Chiar dacă avem rezistențele de 1k fizice externe, este un strat de protecție util.
    GPIO_Init(UNO_A4, GPIO_INPUT);
    GPIO_Write(UNO_A4, GPIO_HIGH);
    
    GPIO_Init(UNO_A5, GPIO_INPUT);
    GPIO_Write(UNO_A5, GPIO_HIGH);

    // 2. Configurarea ratei de transfer (Baud Rate)
    TWBR = (uint8_t)((((F_CPU / F_SCL) / PRESCALER_1) - 16) / 2);
    TWSR &= ~((1 << TWPS1) | (1 << TWPS0)); // Prescaler = 1
}

// Funcție internă izolată (static) pentru așteptarea flag-ului hardware
static uint8_t i2c_wait_ready(void) {
    uint16_t timeout = 10000; 
    while (!(TWCR & (1 << TWINT))) {
        timeout--;
        if (timeout == 0) {
            return 0; // Eroare critică (Timeout)
        }
    }
    return 1; // Flag-ul a fost ridicat (Succes)
}

uint8_t i2c_master_start(void) {
    // Trimitem condiția de START pe magistrală
    TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN);
    
    // Așteptăm terminarea transmisiei
    if (!i2c_wait_ready()) return 0;
    
    // VERIFICARE STRICTĂ: Citim regiștrii hardware pentru a vedea dacă START-ul s-a trimis
    uint8_t status = TWSR & 0xF8;
    if (status != 0x08 && status != 0x10) {
        return 0; // 0x08 = START, 0x10 = Repeta START. Altceva înseamnă eroare pe fir.
    }
    
    return 1;
}

void i2c_master_stop(void) {
    // Condiția de STOP nu așteaptă TWINT, hardware-ul o execută și eliberează magistrala
    TWCR = (1 << TWINT) | (1 << TWSTO) | (1 << TWEN);
}

uint8_t i2c_master_write(uint8_t data) {
    // Încărcăm datele în registru și declanșăm transmisia
    TWDR = data;
    TWCR = (1 << TWINT) | (1 << TWEN);
    
    // Așteptăm terminarea transmisiei
    if (!i2c_wait_ready()) return 0;
    
    // VERIFICARE STRICTĂ: Verificăm dacă Slave-ul (NANO) a confirmat primirea
    uint8_t status = TWSR & 0xF8;
    
    // 0x18 = S-a trimis o adresă + Write, și am primit ACK
    // 0x28 = S-a trimis un pachet de date, și am primit ACK
    if (status != 0x18 && status != 0x28) {
        return 0; // NANO-ul nu a răspuns (NACK). Anulăm transmisia!
    }
    
    return 1;
}