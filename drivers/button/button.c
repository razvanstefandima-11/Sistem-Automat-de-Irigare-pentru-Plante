#include "button.h"
#include "bsp/uno.h"            // Aducem definițiile pinilor (UNO_D9, etc.)
#include "drivers/gpio/gpio.h"  // Aducem funcțiile GPIO_Init, GPIO_Write, GPIO_Read

// Mapăm logic butoanele pe pinii fizici definiți în uno.h
#define BTN_MINUS  UNO_D9
#define BTN_SELECT UNO_D10
#define BTN_PLUS   UNO_D13

void BUTTON_Init(void) {
    // 1. Configurăm pinii ca INTRĂRI
    GPIO_Init(BTN_MINUS, GPIO_INPUT);
    GPIO_Init(BTN_SELECT, GPIO_INPUT);
    GPIO_Init(BTN_PLUS, GPIO_INPUT);
    
    // 2. Activăm rezistențele interne de Pull-up scriind HIGH pe pinul de intrare
    GPIO_Write(BTN_MINUS, GPIO_HIGH);
    GPIO_Write(BTN_SELECT, GPIO_HIGH);
    GPIO_Write(BTN_PLUS, GPIO_HIGH);
}

uint8_t BUTTON_IsMinusPressed(void) {
    // Dacă butonul e apăsat, face scurt la masă (GND) și citim LOW
    if (GPIO_Read(BTN_MINUS) == GPIO_LOW) {
        return 1;
    }
    return 0;
}

uint8_t BUTTON_IsSelectPressed(void) {
    if (GPIO_Read(BTN_SELECT) == GPIO_LOW) {
        return 1;
    }
    return 0;
}

uint8_t BUTTON_IsPlusPressed(void) {
    if (GPIO_Read(BTN_PLUS) == GPIO_LOW) {
        return 1;
    }
    return 0;
}