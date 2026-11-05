#include "pwm.h"
#include "drivers/timer/timer1.h"
#include "drivers/timer/timer2.h"
#include "bsp/uno.h"

// Helper to determine Timer and Channel from Port/Pin
// Timer1: D9 (PB1), D10 (PB2)
// Timer2: D3 (PD3), D11 (PB3)

/**
 * @brief Initialize PWM on a specific pin.
 * 
 * Configures the associated timer for the given pin. 
 * - Port B, Pin 1 (D9) -> Timer1 OC1A
 * - Port B, Pin 2 (D10) -> Timer1 OC1B
 * - Port B, Pin 3 (D11) -> Timer2 OC2A
 * - Port D, Pin 3 (D3) -> Timer2 OC2B
 * 
 * @param port GPIO Port (GPIO_PORTB, GPIO_PORTD).
 * @param pin Pin number (0-7).
 * @param frequency_hz Desired PWM frequency in Hz.
 */
void PWM_Init(uint8_t port, uint8_t pin, uint32_t frequency_hz) {
    if (port == GPIO_PORTB) {
        if (pin == 1 || pin == 2) {
            // Timer1 (16-bit)
            // F_CPU = 16000000
            // TOP = F_CPU / (Prescaler * Freq) - 1
            
            uint32_t f_cpu = 16000000UL;
            uint16_t prescalers[] = {1, 8, 64, 256, 1024};
            uint16_t chosen_prescaler = 0;
            uint32_t top = 0;

            for (int i = 0; i < 5; i++) {
                uint32_t temp_top = f_cpu / (prescalers[i] * frequency_hz) - 1;
                // Check if TOP fits in 16-bit register
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
            // Timer2 (OC2A / D11)
            // Timer2 is 8-bit, Mode 3 (Fast PWM) has fixed TOP=MAX=0xFF (255).
            // Freq = F_CPU / (Prescaler * 256)
            
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
            // Timer2 (OC2B / D3)
            // Same logic as D11/Timer2
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
}

/**
 * @brief Set the duty cycle for a specific pin.
 * 
 * Maps the 0-255 duty cycle value to the underlying timer's register range.
 * - For Timer2 (8-bit), it maps 1:1.
 * - For Timer1 (16-bit), it scales 0-255 to 0-TOP (ICR1).
 * 
 * @param port GPIO Port.
 * @param pin Pin number.
 * @param duty Duty cycle (0-255).
 */
void PWM_SetDutyCycle(uint8_t port, uint8_t pin, uint8_t duty) {
    if (port == GPIO_PORTB) {
        if (pin == 1) { // D9 / OC1A
            // Map 0-255 to 0-ICR1
            // Use 32-bit to prevent overflow during calc
            uint32_t val = ((uint32_t)duty * ICR1) / 255;
            Timer1_SetDutyCycleA((uint16_t)val);
        } else if (pin == 2) { // D10 / OC1B
            uint32_t val = ((uint32_t)duty * ICR1) / 255;
            Timer1_SetDutyCycleB((uint16_t)val);
        } else if (pin == 3) { // D11 / OC2A
            Timer2_SetDutyCycleA(duty);
        }
    } else if (port == GPIO_PORTD) {
        if (pin == 3) { // D3 / OC2B
            Timer2_SetDutyCycleB(duty);
        }
    }
}

/**
 * @brief Stop PWM on a specific pin.
 * 
 * Disables the PWM output for the associated timer channel.
 * Note: Currently stops the entire timer if shared.
 * 
 * @param port GPIO Port.
 * @param pin Pin number.
 */
void PWM_Stop(uint8_t port, uint8_t pin) {
    // Basic stop implementation mapping to timers
    // Note: This stops the whole timer, which might affect other channel. 
    // Ideally we should just disconnect the pin from the timer (COM bits).
    // For now, let's keep it simple as per previous driver simple Stop.
    
     if (port == GPIO_PORTB) {
        if (pin == 1 || pin == 2) {
             Timer1_Stop();
        } else if (pin == 3) {
             Timer2_Stop();
        }
    } else if (port == GPIO_PORTD) {
        if (pin == 3) {
             Timer2_Stop();
        }
    }
}