#include <stdint.h>
#include <avr/io.h>

// Includem noul tău driver I2C Master (cu litere mici)
#include "drivers/i2c/i2c_master.h"

// Includem restul modulelor sistemului
#include "drivers/adc/adc.h"
#include "drivers/lcd/lcd.h"
#include "drivers/buzzer/buzzer.h"
#include "drivers/led/led.h"
#include "drivers/nivel_apa/nivel_apa.h"
#include "drivers/s_umiditate/s_umiditate.h"
#include "utils/delay.h" // Folosim STRICT delay-ul tău customizat

// Adresa Slave-ului (Nano), setată la 0x20 conform noilor tale fișiere
#define SLAVE_ADDR 0x20

// Configurația pentru pragurile senzorilor HW-390
SUmiditateConfig_t config_sol;

/**
 * @brief Funcție helper locală pentru a transmite un pachet complet I2C curat.
 * Folosește funcțiile noi din i2c_master.c
 */
void trimite_comanda_i2c(uint8_t comanda) {
    i2c_master_start();                  // Condiție START
    i2c_master_write(SLAVE_ADDR << 1);   // Trimitem adresa în regim SLA+W
    i2c_master_write(comanda);           // Trimitem byte-ul de comandă
    i2c_master_stop();                   // Condiție STOP
}

int main(void) {
    // ==========================================
    // 1. INIȚIALIZARE HARDWARE (SETUP)
    // ==========================================
    ADC_Init();           
    LCD_Init();           
    i2c_master_init(); // Inițializarea din noul tău driver
    Buzzer_Init();        
    LED_Init();           
    NivelApa_Init();      
    
    // Setăm pragurile de umiditate pentru sol
    S_Umiditate_SetConfig(&config_sol, 350, 430, 480, 520);

    // Mesaj de pornire (acum aliniat corect datorită noului tău lcd.c)
    LCD_SetCursor(0, 0);
    LCD_String("Sistem Irigatie ");
    LCD_SetCursor(1, 0);
    LCD_String("Initializare... ");
    
    Delay(2000); 
    LCD_Command(LCD_CMD_CLEAR_DISPLAY);

    // ==========================================
    // 2. BUCLA PRINCIPALĂ
    // ==========================================
    while (1) {
        
        // --- A. VERIFICARE FAILSAFE (COMENTATĂ TEMPORAR PENTRU TEST) ---
        /*
        uint16_t nivel_apa = NivelApa_Citeste();
        if (nivel_apa < 100) {
            trimite_comanda_i2c(0x00); // Trimite comanda de oprire totală către Nano
            Buzzer_On();
            LED_On();
            LCD_SetCursor(0, 0);
            LCD_String("ALARMA CRITICA! ");
            LCD_SetCursor(1, 0);
            LCD_String("Rezervor Gol!   ");
            Delay(1000); 
            continue;        
        }
        */

        // Stare normală - oprim avertizările locale pe Uno
        Buzzer_Off();
        LED_Off();

        // --- B. CITIRE SENZORI REALI SOL (Canale ADC 0, 1, 2) ---
        SUmiditateLevel_t planta_1 = S_Umiditate_GetLevel(0, &config_sol);
        SUmiditateLevel_t planta_2 = S_Umiditate_GetLevel(1, &config_sol);
        SUmiditateLevel_t planta_3 = S_Umiditate_GetLevel(2, &config_sol);

        // --- C. LOGICĂ DE IRIGARE ȘI TRANSMISIE PRIN NOUL I2C ---
        
        // Control Releu / Pompa 1
        if (planta_1 == S_UMIDITATE_VERY_LOW || planta_1 == S_UMIDITATE_LOW) {
            trimite_comanda_i2c(0x01); 
        } else if (planta_1 == S_UMIDITATE_HIGH || planta_1 == S_UMIDITATE_VERY_HIGH) {
            trimite_comanda_i2c(0x11); 
        }

        // Control Releu / Pompa 2
        if (planta_2 == S_UMIDITATE_VERY_LOW || planta_2 == S_UMIDITATE_LOW) {
            trimite_comanda_i2c(0x02); 
        } else if (planta_2 == S_UMIDITATE_HIGH || planta_2 == S_UMIDITATE_VERY_HIGH) {
            trimite_comanda_i2c(0x12); 
        }

        // Control Releu / Pompa 3
        if (planta_3 == S_UMIDITATE_VERY_LOW || planta_3 == S_UMIDITATE_LOW) {
            trimite_comanda_i2c(0x03); 
        } else if (planta_3 == S_UMIDITATE_HIGH || planta_3 == S_UMIDITATE_VERY_HIGH) {
            trimite_comanda_i2c(0x13); 
        }

        // --- D. AFIȘARE STATUS PE LCD ---
        LCD_SetCursor(0, 0);
        LCD_String("P1:OK P2:OK P3:OK"); 
        LCD_SetCursor(1, 0);
        LCD_String("Sistem: ACTIV   ");

        Delay(2000); 
    }

    return 0;
}