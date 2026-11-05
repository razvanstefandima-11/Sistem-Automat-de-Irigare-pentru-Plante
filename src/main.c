#include <stdint.h>
#include <stdlib.h>
#include <stdio.h> 
#include <string.h>

#include "drivers/i2c/i2c_master.h"
#include "drivers/lcd/lcd.h"
#include "drivers/adc/adc.h"
#include "drivers/button/button.h"
#include "drivers/led/led.h"
#include "drivers/buzzer/buzzer.h"
#include "drivers/nivel_apa/nivel_apa.h"
#include "drivers/gpio/gpio.h"
#include "drivers/usart/usart.h" 
#include "bsp/uno.h"
#include "utils/delay.h"

#define SLAVE_ADDR   0x20

uint8_t stare_pompe[3] = {0, 0, 0};
uint8_t senzor_selectat = 0;               
uint8_t praguri_irigare[3] = {40, 40, 40}; 
uint8_t toggle_alarma = 0;

void format_2digits(uint8_t val, char* buf) {
    if (val > 99) val = 99; 
    if (val < 10) {
        buf[0] = ' '; buf[1] = val + '0';
    } else {
        buf[0] = (val / 10) + '0'; buf[1] = (val % 10) + '0';
    }
    buf[2] = '\0';
}

uint8_t calculeaza_procent_calibrat(uint16_t valoare_raw) {
    uint16_t val_uscat = 570; 
    uint16_t val_ud = 210;    
    if (valoare_raw >= val_uscat) return 0;   
    if (valoare_raw <= val_ud) return 99;     
    return (uint32_t)(val_uscat - valoare_raw) * 99 / (val_uscat - val_ud);
}

void trimite_comanda_i2c(uint8_t comanda) {
    if (i2c_master_start() == 0) return;
    if (i2c_master_write(SLAVE_ADDR << 1) == 0) return;
    if (i2c_master_write(comanda) == 0) return;
    i2c_master_stop();
}

void actualizeaza_ecran_normal(uint8_t p1, uint8_t p2, uint8_t p3) {
    char b1[3], b2[3], b3[3], b_prag[3];
    format_2digits(p1, b1);
    format_2digits(p2, b2);
    format_2digits(p3, b3);
    format_2digits(praguri_irigare[senzor_selectat], b_prag);

    LCD_SetCursor(0, 0);
    LCD_String("1:"); LCD_String(b1);
    LCD_String(" 2:"); LCD_String(b2);
    LCD_String(" 3:"); LCD_String(b3);
    LCD_String("  "); 
    
    LCD_SetCursor(1, 0);
    LCD_String("A:OK P");
    char c_num[2] = {(senzor_selectat + 1) + '0', '\0'};
    LCD_String(c_num);
    LCD_String("=");
    LCD_String(b_prag);
    LCD_String("% ");

    if (stare_pompe[0] || stare_pompe[1] || stare_pompe[2]) {
        LCD_String("IRIG");
    } else {
        LCD_String("STBY");
    }
}

int main(void) {
    ADC_Init(); 
    LCD_Init(); 
    i2c_master_init(); 
    BUTTON_Init(); 
    LED_Init();
    Buzzer_Init();
    NivelApa_Init();
    USART_Init(16000000UL, 9600); 

    LCD_String("Sistem Automat");
    Delay(1000);
    LCD_Command(LCD_CMD_CLEAR_DISPLAY);

    uint8_t contor_senzori = 0;
    uint8_t debounce_minus = 0;
    uint8_t debounce_select = 0;
    uint8_t debounce_plus = 0;

    while (1) {
        if (debounce_minus > 0) debounce_minus--;
        if (debounce_select > 0) debounce_select--;
        if (debounce_plus > 0) debounce_plus--;

        if (BUTTON_IsMinusPressed() && debounce_minus == 0) { 
            if (praguri_irigare[senzor_selectat] > 5) praguri_irigare[senzor_selectat] -= 5;
            debounce_minus = 3; 
        }

        if (BUTTON_IsSelectPressed() && debounce_select == 0) { 
            senzor_selectat++;
            if (senzor_selectat > 2) senzor_selectat = 0;
            debounce_select = 4; 
        }

        if (BUTTON_IsPlusPressed() && debounce_plus == 0) { 
            if (praguri_irigare[senzor_selectat] < 95) praguri_irigare[senzor_selectat] += 5;
            debounce_plus = 3; 
        }

        contor_senzori++;
        if (contor_senzori >= 10) {
            contor_senzori = 0; 

            uint8_t apa_ok = NivelApa_Read(); 
            uint8_t pct_sol[3] = {
                calculeaza_procent_calibrat(ADC_Read(0)),
                calculeaza_procent_calibrat(ADC_Read(1)),
                calculeaza_procent_calibrat(ADC_Read(2))
            };
            
            char buffer_uart[64];

            if (apa_ok == 1) {
                LED_Off();
                Buzzer_Off();

                for(uint8_t i = 0; i < 3; i++) {
                    uint8_t prag_pornire = praguri_irigare[i];
                    uint8_t prag_oprire = prag_pornire + 5; 

                    if (pct_sol[i] < prag_pornire && !stare_pompe[i]) {
                        trimite_comanda_i2c(i + 1); 
                        stare_pompe[i] = 1;
                    } else if (pct_sol[i] >= prag_oprire && stare_pompe[i]) {
                        trimite_comanda_i2c(i + 17); 
                        stare_pompe[i] = 0;
                    }
                }

                // [NOU] Trimitem toate pragurile si indexul senzorului selectat (11 valori in total)
                sprintf(buffer_uart, "DATA:%u,%u,%u,%u,%u,%u,1,%u,%u,%u,%u\n", 
                        pct_sol[0], pct_sol[1], pct_sol[2],
                        stare_pompe[0], stare_pompe[1], stare_pompe[2],
                        praguri_irigare[0], praguri_irigare[1], praguri_irigare[2],
                        senzor_selectat);
                USART_Transmit(buffer_uart, strlen(buffer_uart)); 

                actualizeaza_ecran_normal(pct_sol[0], pct_sol[1], pct_sol[2]);

            } else {
                for(uint8_t i = 0; i < 3; i++) {
                    if (stare_pompe[i]) {
                        trimite_comanda_i2c(i + 17); 
                        stare_pompe[i] = 0;
                    }
                }

                if (toggle_alarma == 0) {
                    LED_On(); Buzzer_On(); toggle_alarma = 1;
                } else {
                    LED_Off(); Buzzer_Off(); toggle_alarma = 0;
                }

                // [NOU] Trimitem formatul extins chiar si in modul alarma (Apă = 0)
                sprintf(buffer_uart, "DATA:%u,%u,%u,%u,%u,%u,0,%u,%u,%u,%u\n", 
                        pct_sol[0], pct_sol[1], pct_sol[2],
                        stare_pompe[0], stare_pompe[1], stare_pompe[2],
                        praguri_irigare[0], praguri_irigare[1], praguri_irigare[2],
                        senzor_selectat);
                USART_Transmit(buffer_uart, strlen(buffer_uart));

                LCD_SetCursor(0, 0);
                LCD_String("  SISTEM OPRIT  ");
                LCD_SetCursor(1, 0);
                LCD_String("ALARMA:LIPSA APA");
                
            }
        }

        Delay(50); 
    }
    return 0;
}