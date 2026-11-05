#include "dht22.h"
#include <util/delay.h>
#include <avr/interrupt.h>

/**
 * @file dht22.c
 * @brief Bare-metal 1-wire timing implementation for DHT22.
 */

static gpio_port_t dht_port = GPIO_PORTA;
static uint8_t dht_pin = 0;

void DHT22_Init(gpio_port_t port, uint8_t pin) {
    dht_port = port;
    dht_pin = pin;

    // Idle high with internal pull-up enabled
    GPIO_Init(dht_port, dht_pin, GPIO_INPUT_PULLUP);
}

static uint8_t wait_pin_state(gpio_state_t expected_state, uint16_t timeout_us) {
    while (GPIO_Read(dht_port, dht_pin) != expected_state) {
        if (timeout_us == 0) {
            return 0; // Timeout
        }
        _delay_us(1);
        timeout_us--;
    }
    return 1;
}

dht22_status_t DHT22_Read(dht22_data_t* data) {
    if (!data) return DHT22_ERROR_TIMEOUT;

    uint8_t raw[5] = {0, 0, 0, 0, 0};

    // 1. Host Send Start Signal: Pull LOW for 2ms
    GPIO_Init(dht_port, dht_pin, GPIO_OUTPUT);
    GPIO_Write(dht_port, dht_pin, GPIO_LOW);
    _delay_ms(2);

    // 2. Host Pull HIGH for 30us and switch to input
    GPIO_Write(dht_port, dht_pin, GPIO_HIGH);
    _delay_us(30);
    GPIO_Init(dht_port, dht_pin, GPIO_INPUT_PULLUP);

    // Critical timing section begins: disable interrupts
    uint8_t sreg = SREG;
    cli();

    // 3. Sensor response: 80us LOW followed by 80us HIGH
    if (!wait_pin_state(GPIO_LOW, 100)) {
        SREG = sreg;
        return DHT22_ERROR_TIMEOUT;
    }
    if (!wait_pin_state(GPIO_HIGH, 100)) {
        SREG = sreg;
        return DHT22_ERROR_TIMEOUT;
    }
    if (!wait_pin_state(GPIO_LOW, 100)) {
        SREG = sreg;
        return DHT22_ERROR_TIMEOUT;
    }

    // 4. Read 40 data bits (5 bytes)
    for (uint8_t i = 0; i < 40; i++) {
        // Wait for bit start (50us LOW)
        if (!wait_pin_state(GPIO_HIGH, 75)) {
            SREG = sreg;
            return DHT22_ERROR_TIMEOUT;
        }

        // Measure HIGH duration: 26-28us = '0', ~70us = '1'
        _delay_us(35);
        if (GPIO_Read(dht_port, dht_pin) == GPIO_HIGH) {
            raw[i / 8] |= (1 << (7 - (i % 8)));
            // Wait for pin to return to LOW
            if (!wait_pin_state(GPIO_LOW, 80)) {
                SREG = sreg;
                return DHT22_ERROR_TIMEOUT;
            }
        }
    }

    // Restore interrupts
    SREG = sreg;

    // 5. Verify Checksum
    uint8_t checksum = (raw[0] + raw[1] + raw[2] + raw[3]) & 0xFF;
    if (checksum != raw[4]) {
        return DHT22_ERROR_CHECKSUM;
    }

    // 6. Decode Data
    uint16_t raw_humidity = ((uint16_t)raw[0] << 8) | raw[1];
    uint16_t raw_temp = (((uint16_t)(raw[2] & 0x7F)) << 8) | raw[3];

    data->humidity = (float)raw_humidity * 0.1f;
    data->temperature = (float)raw_temp * 0.1f;

    if (raw[2] & 0x80) {
        data->temperature = -data->temperature;
    }

    return DHT22_OK;
}

bool DHT22_IsHumidityOptimalForPollination(float humidity) {
    // Condition specified in C.A.M.P.I. Section 4.2: optimal RH between 50% and 70%
    return (humidity >= 50.0f && humidity <= 70.0f);
}
