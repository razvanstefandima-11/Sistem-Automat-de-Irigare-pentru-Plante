#include "i2c_master.h"
#include <avr/io.h>

void I2C_Master_Init(void)
{
    // Setăm prescalerul la 1
    TWSR = 0x00;
    
    // Configurare bit rate register pentru frecvența de 100 kHz la un ceas de 16 MHz
    TWBR = 72;
    
    // Activăm perifericul TWI (Two Wire Interface)
    TWCR = (1 << TWEN);
}

void I2C_Start(void)
{
    // Trimitem condiția de START prin setarea bitului TWSTA
    TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN);
    
    // Așteptăm ca bitul TWINT să devină 1
    while (!(TWCR & (1 << TWINT))) {
        ;
    }
}

void I2C_Stop(void)
{
    // Trimitem condiția de STOP
    TWCR = (1 << TWINT) | (1 << TWSTO) | (1 << TWEN);
}

void I2C_Write(uint8_t data)
{
    // Încărcăm datele
    TWDR = data;
    
    // Pornim transmisia
    TWCR = (1 << TWINT) | (1 << TWEN);
    
    // Așteptăm finalizarea
    while (!(TWCR & (1 << TWINT))) {
        ;
    }
}

void I2C_Master_Send_Cmd(uint8_t slave_addr, uint8_t cmd)
{
    I2C_Start(); 
    I2C_Write((slave_addr << 1) | 0); // Adresa + bitul de Write (0)
    I2C_Write(cmd); // Comanda
    I2C_Stop();
}