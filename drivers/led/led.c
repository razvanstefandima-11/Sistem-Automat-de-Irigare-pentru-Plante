#include "led.h"
#include "drivers/gpio/gpio.h"
#include "bsp/uno.h"

// Maparea hardware: LED-ul de alarmă vizuală stă pe pinul D8 (PB0)
#define PIN_LED_ALARMA UNO_D8 

void LED_Init(void) {
    GPIO_Init(PIN_LED_ALARMA, GPIO_OUTPUT);
    GPIO_Write(PIN_LED_ALARMA, GPIO_LOW); // Asigurăm starea inițială stinsă
}

void LED_On(void) {
    GPIO_Write(PIN_LED_ALARMA, GPIO_HIGH);
}

void LED_Off(void) {
    GPIO_Write(PIN_LED_ALARMA, GPIO_LOW);
}

void LED_Toggle(void) {
    GPIO_Toggle(PIN_LED_ALARMA);
}