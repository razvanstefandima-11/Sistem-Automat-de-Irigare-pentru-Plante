#include "encoder.h"
#include "drivers/gpio/gpio.h"
#include <avr/io.h>
#include <avr/interrupt.h>

/**
 * @file encoder.c
 * @brief Quadrature rotary encoder external interrupt implementation.
 */

// Hardware Pin Mappings on ATmega2560
#define PIN_ENC_A_INT4  GPIO_PORTE, 4 /**< Channel A / CLK (Pin 2 / INT4) */
#define PIN_ENC_B_DT    GPIO_PORTH, 3 /**< Channel B / DT  (Pin 6) */
#define PIN_ENC_BTN_SW  GPIO_PORTH, 4 /**< Push Button / SW (Pin 7) */

static volatile int16_t encoder_position = 0;
static volatile int16_t encoder_delta = 0;

static bool button_last_reading = false;
static bool button_clicked_event = false;
static uint32_t button_debounce_timer = 0;

void Encoder_Init(void) {
    // 1. Configure pins as inputs with internal pull-ups enabled
    GPIO_Init(PIN_ENC_A_INT4, GPIO_INPUT_PULLUP);
    GPIO_Init(PIN_ENC_B_DT, GPIO_INPUT_PULLUP);
    GPIO_Init(PIN_ENC_BTN_SW, GPIO_INPUT_PULLUP);

    // 2. Configure External Interrupt 4 (INT4 on PE4)
    // EICRB: ISC41 = 1, ISC40 = 0 -> Trigger on Falling Edge
    EICRB &= ~((1 << ISC41) | (1 << ISC40));
    EICRB |= (1 << ISC41);

    // 3. Clear any pending INT4 flag and enable interrupt mask
    EIFR |= (1 << INTF4);
    EIMSK |= (1 << INT4);

    encoder_position = 0;
    encoder_delta = 0;
    button_last_reading = false;
    button_clicked_event = false;

    // Enable global interrupts
    sei();
}

/**
 * @brief INT4 ISR triggered on falling edge of Channel A (CLK).
 */
ISR(INT4_vect) {
    // Read Channel B (DT):
    // If Channel B is HIGH during falling edge of A -> Clockwise
    // If Channel B is LOW -> Counter-Clockwise
    if (GPIO_Read(PIN_ENC_B_DT) == GPIO_HIGH) {
        encoder_position++;
        encoder_delta++;
    } else {
        encoder_position--;
        encoder_delta--;
    }
}

int16_t Encoder_GetDelta(void) {
    uint8_t sreg = SREG;
    cli();
    int16_t delta = encoder_delta;
    encoder_delta = 0;
    SREG = sreg;
    return delta;
}

int16_t Encoder_GetPosition(void) {
    uint8_t sreg = SREG;
    cli();
    int16_t pos = encoder_position;
    SREG = sreg;
    return pos;
}

void Encoder_SetPosition(int16_t pos) {
    uint8_t sreg = SREG;
    cli();
    encoder_position = pos;
    SREG = sreg;
}

bool Encoder_IsButtonPressed(void) {
    // Button connected to GND with internal pull-up (LOW = Pressed)
    return (GPIO_Read(PIN_ENC_BTN_SW) == GPIO_LOW);
}

bool Encoder_HasBeenClicked(void) {
    bool clicked = button_clicked_event;
    button_clicked_event = false;
    return clicked;
}

void Encoder_Update(uint32_t current_millis) {
    bool current_pressed = Encoder_IsButtonPressed();

    if (current_pressed != button_last_reading) {
        if ((current_millis - button_debounce_timer) >= 40) { // 40ms debounce window
            button_debounce_timer = current_millis;
            button_last_reading = current_pressed;

            // Generate click on button release
            if (!current_pressed) {
                button_clicked_event = true;
            }
        }
    }
}
