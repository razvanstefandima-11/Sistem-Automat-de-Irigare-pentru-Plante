#include "buzzer.h"
#include "drivers/gpio/gpio.h"

/**
 * @file buzzer.c
 * @brief Acoustic feedback and alarm implementation.
 */

// Hardware Pin on ATmega2560: PH5 (Pin 8 on Arduino Mega)
#define PIN_BUZZER GPIO_PORTH, 5

static bool buzzer_state = false;
static bool alarm_active = false;
static uint32_t beep_start_time = 0;
static uint16_t beep_duration = 0;
static uint32_t alarm_toggle_time = 0;

void Buzzer_Init(void) {
    GPIO_Init(PIN_BUZZER, GPIO_OUTPUT);
    Buzzer_Off();
    alarm_active = false;
    beep_duration = 0;
}

void Buzzer_On(void) {
    GPIO_Write(PIN_BUZZER, GPIO_HIGH);
    buzzer_state = true;
}

void Buzzer_Off(void) {
    GPIO_Write(PIN_BUZZER, GPIO_LOW);
    buzzer_state = false;
}

void Buzzer_Toggle(void) {
    GPIO_Toggle(PIN_BUZZER);
    buzzer_state = !buzzer_state;
}

void Buzzer_Beep(uint16_t duration_ms) {
    beep_duration = duration_ms;
    beep_start_time = 0; // Trigger on update
    Buzzer_On();
}

void Buzzer_SetAlarm(bool enable) {
    alarm_active = enable;
    if (!enable && beep_duration == 0) {
        Buzzer_Off();
    }
}

void Buzzer_Update(uint32_t current_millis) {
    // 1. Handle one-shot beep timing
    if (beep_duration > 0) {
        if (beep_start_time == 0) {
            beep_start_time = current_millis;
        } else if ((current_millis - beep_start_time) >= beep_duration) {
            beep_duration = 0;
            beep_start_time = 0;
            if (!alarm_active) {
                Buzzer_Off();
            }
        }
    }

    // 2. Handle intermittent alarm pattern (250ms ON / 250ms OFF)
    if (alarm_active && beep_duration == 0) {
        if ((current_millis - alarm_toggle_time) >= 250) {
            alarm_toggle_time = current_millis;
            Buzzer_Toggle();
        }
    }
}
