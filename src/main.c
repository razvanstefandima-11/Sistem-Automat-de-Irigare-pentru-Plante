#include <stdint.h>
#include <avr/io.h>
#include <stdlib.h>

// Căile către driverele tale Bare Metal
#include "drivers/i2c/i2c_master.h"
#include "drivers/lcd/lcd.h"
#include "drivers/adc/adc.h"
#include "utils/delay.h"

#define SLAVE_ADDR 0x20

uint8_t stare_pompe[3] = {0, 0, 0};
uint8_t toggle_alarma = 0; // Variabilă pentru a intercala (pulsa) LED-ul și Buzzer-ul

void trimite_comanda_i2c(uint8_t comanda) {
    if (i2c_master_start() == 0) return;
    if (i2c_master_write(SLAVE_ADDR << 1) == 0) return;
    if (i2c_master_write(comanda) == 0) return;
    i2c_master_stop();
}

int main(void) {
    // --- INIȚIALIZARE HARDWARE ---
    ADC_Init(); 
    LCD_Init(); 
    i2c_master_init(); // Acum include și rezistențele de Pull-up pe care le-am adăugat

    // Configurare pini pentru Alarme și Alimentare Senzor Apă direct din regiștri
    // D6 (Buzzer) și D7 (VCC Senzor Apă) sunt pe PORTD [cite: 16, 17]
    DDRD |= (1 << 6) | (1 << 7); 
    // D8 (LED Alarma) este pe PORTB [cite: 17]
    DDRB |= (1 << 0);               

    // Asigurăm-ne că sunt oprite la pornire
    PORTD &= ~((1 << 6) | (1 << 7)); // Buzzer OFF, Senzor Apă OPRIT
    PORTB &= ~(1 << 0);

    LCD_String("Sistem Activ...");
    Delay(2000);
    LCD_Command(0x01); // CLEAR

    while (1) {
        // --- 1. CITIRE ANTI-ELECTROLIZĂ SENZOR APĂ ---
        PORTD |= (1 << 7);          // 1. Pornim alimentarea senzorului pe D7 [cite: 33]
        Delay(10);                       // 2. Așteptăm scurt stabilizarea tensiunii
        uint16_t apa_brut = ADC_Read(3); // 3. Citim nivelul de pe A3 [cite: 16]
        PORTD &= ~(1 << 7);         // 4. Oprim imediat alimentarea pentru a preveni coroziunea [cite: 33]

        // Verificăm dacă nivelul apei este sub pragul critic (prag de 100)
        uint8_t apa_ok = (apa_brut > 100) ? 1 : 0; 

        // --- 2. CITIRE SENZORI SOL ---
        // Se citesc pinii analogici A0, A1, A2 
        uint16_t s1 = ADC_Read(0);
        uint16_t s2 = ADC_Read(1);
        uint16_t s3 = ADC_Read(2);
        uint16_t senzori[3] = {s1, s2, s3};

        // --- 3. LOGICĂ DE CONTROL ȘI ALARMĂ ---
        if (apa_ok == 1) {
            // Avem apă, oprim alarmele hardware
            PORTD &= ~(1 << 6); // Buzzer OFF
            PORTB &= ~(1 << 0); // LED OFF

            // Logica normală de irigare pentru fiecare din cele 3 pompe
            for(uint8_t i = 0; i < 3; i++) {
                if (senzori[i] > 600 && !stare_pompe[i]) {
                    // Valori zecimale: 1 (0x01), 2 (0x02), 3 (0x03)
                    trimite_comanda_i2c(i + 1); 
                    stare_pompe[i] = 1;
                } else if (senzori[i] < 550 && stare_pompe[i]) {
                    // Valori zecimale: 17 (0x11), 18 (0x12), 19 (0x13)
                    trimite_comanda_i2c(i + 17); 
                    stare_pompe[i] = 0;
                }
            }

            // Afișare stare normală pe LCD [cite: 14]
            char b1[4], b2[4], b3[4];
            itoa(s1/100, b1, 10); 
            itoa(s2/100, b2, 10); 
            itoa(s3/100, b3, 10);

            LCD_SetCursor(0, 0);
            LCD_String("S1:"); LCD_String(b1);
            LCD_String(" S2:"); LCD_String(b2);
            LCD_String(" S3:"); LCD_String(b3);
            
            LCD_SetCursor(1, 0);
            LCD_String("Apa:OK  ");
            if (stare_pompe[0] || stare_pompe[1] || stare_pompe[2]) {
                LCD_String(" IRIGA  ");
            } else {
                LCD_String(" STBY   ");
            }

        } else {
            // --- MOD AVARIE: LIPSĂ APĂ ---
            
            // 1. Oprim forțat toate pompele pentru a proteja echipamentul [cite: 29]
            for(uint8_t i = 0; i < 3; i++) {
                if (stare_pompe[i]) {
                    trimite_comanda_i2c(i + 17);
                    stare_pompe[i] = 0;
                }
            }

            // 2. Pulsăm LED-ul și Buzzer-ul (Intermitent) [cite: 30]
            if (toggle_alarma == 0) {
                PORTD |= (1 << 6); // Buzzer ON
                PORTB |= (1 << 0); // LED ON
                toggle_alarma = 1;
            } else {
                PORTD &= ~(1 << 6); // Buzzer OFF
                PORTB &= ~(1 << 0); // LED OFF
                toggle_alarma = 0;
            }

            // 3. Afișare mesaj critic pe LCD [cite: 30]
            LCD_SetCursor(0, 0);
            LCD_String("  SISTEM OPRIT  ");
            LCD_SetCursor(1, 0);
            LCD_String("ALARMA:LIPSA APA"); // [cite: 30]
        }

        Delay(500); 
    }
    
    return 0;
}