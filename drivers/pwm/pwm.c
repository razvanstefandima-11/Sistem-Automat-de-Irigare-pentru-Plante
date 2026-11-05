#include "pwm.h"
#include "drivers/timer/timer1.h"
#include "drivers/timer/timer2.h"
#include "bsp/bsp.h"

/**
 * @file pwm.c
 * @brief Pulse Width Modulation (PWM) Driver adapted for ATmega2560 (C.A.M.P.I. Master).
 */

void PWM_Init(uint8_t port, uint8_t pin, uint32_t frequency_hz) {
#if defined(__AVR_ATmega2560__)
    // ATmega2560 PWM Pin mappings:
    // Timer1 (16-bit): PB5 (OC1A / Pin 11), PB6 (OC1B / Pin 12)
    // Timer2 (8-bit):  PB4 (OC2A / Pin 10), PH6 (OC2B / Pin 9)
    if (port == GPIO_PORTB && (pin == 5 || pin == 6)) {
        uint32_t f_cpu = 16000000UL;
        uint16_t prescalers[] = {1, 8, 64, 256, 1024};
        uint16_t chosen_prescaler = 0;
        uint32_t top = 0;

        for (int i = 0; i < 5; i++) {
            uint32_t temp_top = f_cpu / (prescalers[i] * frequency_hz) - 1;
            if (temp_top < 65536) {
                chosen_prescaler = prescalers[i];
                top = (uint16_t)temp_top;
                break;
            }
        }
        if (chosen_prescaler > 0) {
            Timer1_FastPWM_Init(chosen_prescaler, (uint16_t)top);
        }
    } else if ((port == GPIO_PORTB && pin == 4) || (port == GPIO_PORTH && pin == 6)) {
        uint32_t f_cpu = 16000000UL;
        uint16_t prescalers[] = {1, 8, 32, 64, 128, 256, 1024};
        uint16_t best_prescaler = 1024;
        uint32_t min_diff = 0xFFFFFFFF;

        for (int i = 0; i < 7; i++) {
            uint32_t actual_freq = f_cpu / (prescalers[i] * 256);
            uint32_t diff = (actual_freq > frequency_hz) ? (actual_freq - frequency_hz) : (frequency_hz - actual_freq);
            if (diff < min_diff) {
                min_diff = diff;
                best_prescaler = prescalers[i];
            }
        }
        Timer2_FastPWM_Init(best_prescaler);
    }
#else
    // ATmega328P / Unit Test Mock mappings
    if (port == GPIO_PORTB) {
        if (pin == 1 || pin == 2) {
            uint32_t f_cpu = 16000000UL;
            uint16_t prescalers[] = {1, 8, 64, 256, 1024};
            uint16_t chosen_prescaler = 0;
            uint32_t top = 0;

            for (int i = 0; i < 5; i++) {
                uint32_t temp_top = f_cpu / (prescalers[i] * frequency_hz) - 1;
                if (temp_top < 65536) {
                    chosen_prescaler = prescalers[i];
                    top = (uint16_t)temp_top;
                    break;
                }
            }
            if (chosen_prescaler > 0) {
                Timer1_FastPWM_Init(chosen_prescaler, (uint16_t)top);
            }
        } else if (pin == 3) {
            uint32_t f_cpu = 16000000UL;
            uint16_t prescalers[] = {1, 8, 32, 64, 128, 256, 1024};
            uint16_t best_prescaler = 1024;
            uint32_t min_diff = 0xFFFFFFFF;

            for (int i = 0; i < 7; i++) {
                uint32_t actual_freq = f_cpu / (prescalers[i] * 256);
                uint32_t diff = (actual_freq > frequency_hz) ? (actual_freq - frequency_hz) : (frequency_hz - actual_freq);
                if (diff < min_diff) {
                    min_diff = diff;
                    best_prescaler = prescalers[i];
                }
            }
            Timer2_FastPWM_Init(best_prescaler);
        }
    } else if (port == GPIO_PORTD) {
        if (pin == 3) {
            uint32_t f_cpu = 16000000UL;
            uint16_t prescalers[] = {1, 8, 32, 64, 128, 256, 1024};
            uint16_t best_prescaler = 1024;
            uint32_t min_diff = 0xFFFFFFFF;

            for (int i = 0; i < 7; i++) {
                uint32_t actual_freq = f_cpu / (prescalers[i] * 256);
                uint32_t diff = (actual_freq > frequency_hz) ? (actual_freq - frequency_hz) : (frequency_hz - actual_freq);
                if (diff < min_diff) {
                    min_diff = diff;
                    best_prescaler = prescalers[i];
                }
            }
            Timer2_FastPWM_Init(best_prescaler);
        }
    }
#endif
}

void PWM_SetDutyCycle(uint8_t port, uint8_t pin, uint8_t duty) {
#if defined(__AVR_ATmega2560__)
    if (port == GPIO_PORTB) {
        if (pin == 5) { // PB5 / OC1A
            uint32_t val = ((uint32_t)duty * ICR1) / 255;
            Timer1_SetDutyCycleA((uint16_t)val);
        } else if (pin == 6) { // PB6 / OC1B
            uint32_t val = ((uint32_t)duty * ICR1) / 255;
            Timer1_SetDutyCycleB((uint16_t)val);
        } else if (pin == 4) { // PB4 / OC2A
            Timer2_SetDutyCycleA(duty);
        }
    } else if (port == GPIO_PORTH && pin == 6) { // PH6 / OC2B
        Timer2_SetDutyCycleB(duty);
    }
#else
    if (port == GPIO_PORTB) {
        if (pin == 1) {
            uint32_t val = ((uint32_t)duty * ICR1) / 255;
            Timer1_SetDutyCycleA((uint16_t)val);
        } else if (pin == 2) {
            uint32_t val = ((uint32_t)duty * ICR1) / 255;
            Timer1_SetDutyCycleB((uint16_t)val);
        } else if (pin == 3) {
            Timer2_SetDutyCycleA(duty);
        }
    } else if (port == GPIO_PORTD && pin == 3) {
        Timer2_SetDutyCycleB(duty);
    }
#endif
}

void PWM_Stop(uint8_t port, uint8_t pin) {
#if defined(__AVR_ATmega2560__)
    if (port == GPIO_PORTB && (pin == 5 || pin == 6)) {
        Timer1_Stop();
    } else if ((port == GPIO_PORTB && pin == 4) || (port == GPIO_PORTH && pin == 6)) {
        Timer2_Stop();
    }
#else
    if (port == GPIO_PORTB) {
        if (pin == 1 || pin == 2) {
            Timer1_Stop();
        } else if (pin == 3) {
            Timer2_Stop();
        }
    } else if (port == GPIO_PORTD && pin == 3) {
        Timer2_Stop();
    }
#endif
}
