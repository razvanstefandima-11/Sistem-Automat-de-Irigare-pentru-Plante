#include <avr/io.h>
#include "utils/delay.h"

// Includem modulele noastre hardware (HAL)
#include "bsp/uno.h"
#include "drivers/adc/adc.h"
#include "drivers/lcd/lcd.h"
#include "drivers/i2c/i2c_master.h"
#include "drivers/buzzer/buzzer.h"
#include "drivers/led/led.h"
#include "drivers/nivel_apa/nivel_apa.h"
#include "drivers/s_umiditate/s_umiditate.h"

// Adresa plăcii de execuție (Arduino Nano)
#define SLAVE_ADDR 0x3A

// Instanțiem configurarea pentru senzorii de sol HW-390
SUmiditateConfig_t config_sol;

int main(void) {
    // ==========================================
    // 1. INIȚIALIZARE HARDWARE (SETUP)
    // ==========================================
    
    ADC_Init();           // Preluăm funcția globală de ADC
    LCD_Init();           // Inițializare ecran LCD 4-biți
    I2C_Master_Init();    // Setare Uno ca Master pe magistrală
    Buzzer_Init();        // Alarmă sonoră
    LED_Init();           // Alarmă vizuală
    NivelApa_Init();      // Siguranța rezervorului
    
    // Setăm pragurile pentru umiditate (valori ADC mici = umiditate mare)
    // VeryHigh(350), High(430), Medium(480), Low(520)
    S_Umiditate_SetConfig(&config_sol, 350, 430, 480, 520);

    // Mesaj de pornire
    LCD_SetCursor(0, 0);
    LCD_String("Sistem Irigatie ");
    LCD_SetCursor(1, 0);
    LCD_String("Initializare... ");
    Delay(2000);
    LCD_Command(LCD_CMD_CLEAR_DISPLAY);

    // ==========================================
    // 2. BUCLA PRINCIPALĂ (LOOP INFINIT)
    // ==========================================
    while (1) {
        
        // --- A. VERIFICARE FAILSAFE (NIVEL APĂ REZERVOR) ---
        uint16_t nivel_apa = NivelApa_Citeste();

        // Dacă valoarea e sub 100, senzorul e aproape în aer (rezervor gol)
        if (nivel_apa < 100) {
            // Trimitem comanda generală de oprire către Nano (0x00)
            I2C_Master_Send_Cmd(SLAVE_ADDR, 0x00);

            // Semnalizare Alarmă Optică și Sonoră
            Buzzer_On();
            LED_On();
            
            // Afișare stare critică
            LCD_SetCursor(0, 0);
            LCD_String("ALARMA CRITICA! ");
            LCD_SetCursor(1, 0);
            LCD_String("Rezervor Gol!   ");
            
            Delay(1000); // Așteptăm o secundă și reevaluăm situația
            continue;        // Sărim peste irigare până se umple rezervorul
        }

        // --- B. STARE NORMALĂ (OPRIRE ALARME) ---
        Buzzer_Off();
        LED_Off();

        // --- C. CITIRE SENZORI PLANTE (Canale ADC 0, 1, 2) ---
        SUmiditateLevel_t planta_1 = S_Umiditate_GetLevel(0, &config_sol);
        SUmiditateLevel_t planta_2 = S_Umiditate_GetLevel(1, &config_sol);
        SUmiditateLevel_t planta_3 = S_Umiditate_GetLevel(2, &config_sol);

        // --- D. LOGICĂ DE IRIGARE ȘI COMUNICARE I2C ---
        
        // Verificăm Planta 1 (Pompa 1)
        if (planta_1 == S_UMIDITATE_VERY_LOW || planta_1 == S_UMIDITATE_LOW) {
            I2C_Master_Send_Cmd(SLAVE_ADDR, 0x01); // Comandă: Pornește apa
        } else if (planta_1 == S_UMIDITATE_HIGH || planta_1 == S_UMIDITATE_VERY_HIGH) {
            I2C_Master_Send_Cmd(SLAVE_ADDR, 0x11); // Comandă: Oprește apa
        }

        // Verificăm Planta 2 (Pompa 2)
        if (planta_2 == S_UMIDITATE_VERY_LOW || planta_2 == S_UMIDITATE_LOW) {
            I2C_Master_Send_Cmd(SLAVE_ADDR, 0x02); // Comandă: Pornește apa
        } else if (planta_2 == S_UMIDITATE_HIGH || planta_2 == S_UMIDITATE_VERY_HIGH) {
            I2C_Master_Send_Cmd(SLAVE_ADDR, 0x12); // Comandă: Oprește apa
        }

        // Verificăm Planta 3 (Pompa 3)
        if (planta_3 == S_UMIDITATE_VERY_LOW || planta_3 == S_UMIDITATE_LOW) {
            I2C_Master_Send_Cmd(SLAVE_ADDR, 0x03); // Comandă: Pornește apa
        } else if (planta_3 == S_UMIDITATE_HIGH || planta_3 == S_UMIDITATE_VERY_HIGH) {
            I2C_Master_Send_Cmd(SLAVE_ADDR, 0x13); // Comandă: Oprește apa
        }

        // --- E. AFIȘARE STATUS PE LCD ---
        LCD_SetCursor(0, 0);
        LCD_String("P1:OK P2:OK P3:OK"); // Placeholder rafinabil ulterior
        LCD_SetCursor(1, 0);
        LCD_String("Sistem: ACTIV   ");

        Delay(2000); // Executăm ciclul de evaluare la fiecare 2 secunde
    }

    return 0;
}