#include <stdint.h>
#include <avr/io.h>
#include <avr/interrupt.h>

#include "drivers/i2c/i2c_slave.h"
#include "drivers/pompe/pompe.h"

#define SLAVE_ADDR 0x20

int main(void) {
    Pompe_Init();
    i2c_slave_init(SLAVE_ADDR);
    
    // Activăm întreruperile globale obligatorii pentru modul Slave
    sei(); 

    while (1) {
        uint8_t comanda_receptionata;
        
        // Verificăm în siguranță dacă a sosit o comandă nouă de la Master
        if (i2c_slave_has_data(&comanda_receptionata)) {
            switch (comanda_receptionata) {
                case 1:
                    Pompa1_On();
                    break;
                case 2:
                    Pompa2_On();
                    break;
                case 3:
                    Pompa3_On();
                    break;
                    
                case 17: // 0x11 în Hexazecimal
                    Pompa1_Off();
                    break;
                case 18: // 0x12 în Hexazecimal
                    Pompa2_Off();
                    break;
                case 19: // 0x13 în Hexazecimal
                    Pompa3_Off();
                    break;
                    
                default:
                    // În caz de comandă necunoscută, nu acționăm releele
                    break;
            }
        }
    }
    return 0;
}