#include "buzzer.h"
#include "drivers/gpio/gpio.h"
#include "bsp/uno.h" // Aducem maparea pinilor

// Definim aici pinul ca să nu poluăm main.c
#define PIN_BUZZER UNO_D6 

void Buzzer_Init(void) {
    GPIO_Init(PIN_BUZZER, GPIO_OUTPUT);
    GPIO_Write(PIN_BUZZER, GPIO_LOW); // Asigurăm starea inițială oprită
}

void Buzzer_On(void) {
    GPIO_Write(PIN_BUZZER, GPIO_HIGH);
}

void Buzzer_Off(void) {
    GPIO_Write(PIN_BUZZER, GPIO_LOW);
}

void Buzzer_Toggle(void) {
    GPIO_Toggle(PIN_BUZZER);
}