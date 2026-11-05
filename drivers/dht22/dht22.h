#ifndef DHT22_H
#define DHT22_H

#include <stdint.h>
#include <stdbool.h>
#include "drivers/gpio/gpio.h"

/**
 * @file dht22.h
 * @brief Bare-Metal DHT22 Temperature & Humidity Sensor Driver for ATmega2560.
 * 
 * Used in C.A.M.P.I. for microclimate monitoring, thermal heating hysteresis,
 * shading trigger (>32°C), and automated pollination gating (50% - 70% RH).
 */

typedef enum {
    DHT22_OK = 0,
    DHT22_ERROR_TIMEOUT = 1,
    DHT22_ERROR_CHECKSUM = 2
} dht22_status_t;

typedef struct {
    float temperature; /**< Temperature in °C (-40.0°C to +80.0°C) */
    float humidity;    /**< Relative Humidity in % (0.0% to 100.0%) */
} dht22_data_t;

/**
 * @brief Initializes the DHT22 data pin.
 * 
 * @param port GPIO Port.
 * @param pin Pin number (0-7).
 */
void DHT22_Init(gpio_port_t port, uint8_t pin);

/**
 * @brief Performs a 1-wire readout from the DHT22 sensor.
 * 
 * @param data Pointer to dht22_data_t output struct.
 * @return dht22_status_t DHT22_OK on success, or error code.
 */
dht22_status_t DHT22_Read(dht22_data_t* data);

/**
 * @brief Checks if current relative humidity is inside the optimal range for pollination (50% - 70%).
 * 
 * @param humidity Relative humidity in %.
 * @return true if 50% <= humidity <= 70%, false otherwise.
 */
bool DHT22_IsHumidityOptimalForPollination(float humidity);

#endif // DHT22_H

