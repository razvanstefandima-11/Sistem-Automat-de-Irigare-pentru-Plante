/**
 * @file main.c
 * @brief C.A.M.P.I. - Automated Control & Monitoring of Indoor Plants
 * Precision Smart Farming System for Hydroponic Dwarf Tomatoes (Micro Tom)
 * 
 * Master Unit: Microchip ATmega2560 (16MHz, Bare-Metal C / avr-gcc)
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include "drivers/timer/timer.h"
#include "drivers/gpio/gpio.h"
#include "drivers/adc/adc.h"
#include "drivers/usart/usart.h"
#include "drivers/dht22/dht22.h"
#include "drivers/moisture/moisture.h"
#include "drivers/float_switch/float_switch.h"
#include "drivers/dock_sensor/dock_sensor.h"
#include "drivers/pumps/pumps.h"
#include "drivers/valves/valves.h"
#include "drivers/climate_control/climate_control.h"
#include "drivers/grow_lights/grow_lights.h"
#include "drivers/servo_roof/servo_roof.h"
#include "drivers/pollinator/pollinator.h"
#include "drivers/i2c/i2c.h"
#include "drivers/ssd1306/ssd1306.h"
#include "drivers/encoder/encoder.h"
#include "drivers/buzzer/buzzer.h"
#include "drivers/eeprom/eeprom.h"

// System Timing Intervals
#define SENSOR_POLL_INTERVAL_MS 2000UL
#define TELEMETRY_INTERVAL_MS   3000UL
#define OLED_REFRESH_INTERVAL_MS 500UL

// Plant Growth Stages (per slot 0..4)
static uint8_t slot_stages[DOCK_NUM_SLOTS] = {0, 0, 0, 0, 0};
static uint8_t selected_menu_slot = 0;

static dht22_data_t env_climate = {24.0f, 60.0f};

static void update_oled_display(void) {
    SSD1306_SetCursor(0, 0);
    SSD1306_WriteString("C.A.M.P.I. Expert");

    // Line 1: Environmental Readings (DHT22)
    char env_buf[22];
    snprintf(env_buf, sizeof(env_buf), "T:%d.%dC H:%d.%d%%",
             (int)env_climate.temperature, (int)(env_climate.temperature * 10) % 10,
             (int)env_climate.humidity, (int)(env_climate.humidity * 10) % 10);
    SSD1306_SetCursor(1, 0);
    SSD1306_WriteString(env_buf);

    // Line 2: Reservoir Status & Thermal state
    SSD1306_SetCursor(2, 0);
    if (FloatSwitch_IsDryRunLocked()) {
        SSD1306_WriteString("TANK: EMPTY [ALARM] ");
    } else {
        SSD1306_WriteString("TANK: OK   PTC:");
        SSD1306_WriteString(ClimateControl_IsHeatingActive() ? "ON " : "OFF");
    }

    // Line 3: Selected Slot Info
    char slot_buf[22];
    const char* stage_names[] = {"SEED", "VEGE", "FLOW"};
    uint8_t m = Moisture_GetPercentage(selected_menu_slot);
    snprintf(slot_buf, sizeof(slot_buf), "S%u [%s] M:%u%%",
             selected_menu_slot + 1,
             stage_names[slot_stages[selected_menu_slot]],
             m);
    SSD1306_SetCursor(3, 0);
    SSD1306_WriteString(slot_buf);

    // Line 4: Dock Connection Status
    SSD1306_SetCursor(4, 0);
    if (DockSensor_IsDockReady(selected_menu_slot)) {
        SSD1306_WriteString("Dock: LOCKED & READY");
    } else {
        SSD1306_WriteString("Dock: EMPTY / INSERT");
    }

    // Line 5: Moisture Progress Bar
    SSD1306_DrawProgressBar(5, 0, 120, m);
}

int main(void) {
    // 1. Initialize Base Hardware Systems
    Timer_Init();
    ADC_Init();
    USART1_Init_Default();
    Buzzer_Init();

    // 2. Initialize Environmental & Plant Sensors
    DHT22_Init(GPIO_PORTA, 0); // Pin 22 (PA0)
    Moisture_Init();
    FloatSwitch_Init(GPIO_PORTD, 7, GPIO_PORTG, 2); // Pins 38 & 39
    DockSensor_Init(); // Port L Pins 49..45

    // 3. Initialize Actuators & Force Drivers
    Pumps_Init();
    Valves_Init();
    ClimateControl_Init();
    GrowLights_Init();
    ServoRoof_Init();
    Pollinator_Init();

    // 4. Initialize Local HMI (OLED + Rotary Encoder)
    SSD1306_Init();
    Encoder_Init();

    // 5. Load Stored Phenological Stages from Non-Volatile EEPROM
    for (uint8_t i = 0; i < DOCK_NUM_SLOTS; i++) {
        slot_stages[i] = EEPROM_LoadSlotStage(i);
    }
    GrowLights_SetStage(slot_stages[0]);

    // Startup Announcement
    SSD1306_SetCursor(1, 10);
    SSD1306_WriteString("C.A.M.P.I. 2560");
    SSD1306_SetCursor(3, 15);
    SSD1306_WriteString("System Online");
    Buzzer_Beep(100);
    Timer_DelayMs(1000);
    SSD1306_Clear();

    uint32_t last_sensor_time = 0;
    uint32_t last_telemetry_time = 0;
    uint32_t last_oled_time = 0;

    // Main Non-Blocking Superloop
    while (1) {
        uint32_t now = Millis();

        // --- Fast Periodic Subsystem Updates ---
        Encoder_Update(now);
        FloatSwitch_Update(now);
        DockSensor_Update(now);
        Buzzer_Update(now);
        Pollinator_Update(now);

        // --- Rotary Encoder Menu Navigation ---
        int16_t delta = Encoder_GetDelta();
        if (delta != 0) {
            if (delta > 0) {
                selected_menu_slot = (selected_menu_slot + 1) % DOCK_NUM_SLOTS;
            } else {
                selected_menu_slot = (selected_menu_slot == 0) ? (DOCK_NUM_SLOTS - 1) : (selected_menu_slot - 1);
            }
        }

        // Change growth stage on encoder pushbutton click
        if (Encoder_HasBeenClicked()) {
            slot_stages[selected_menu_slot] = (slot_stages[selected_menu_slot] + 1) % 3;
            EEPROM_SaveSlotStage(selected_menu_slot, slot_stages[selected_menu_slot]);
            GrowLights_SetStage(slot_stages[selected_menu_slot]);
            Buzzer_Beep(50);
        }

        // Acoustic validation for 5-second magnetic dock insertion
        for (uint8_t i = 0; i < DOCK_NUM_SLOTS; i++) {
            if (DockSensor_HasJustValidated(i)) {
                Buzzer_Beep(150); // Beep upon confirmed dock lock
            }
        }

        // Safety Alarm if tanks confirmed dry
        if (FloatSwitch_IsDryRunLocked()) {
            Buzzer_SetAlarm(true);
        } else {
            Buzzer_SetAlarm(false);
        }

        // --- Sensor Readout & Climate Control Loop (every 2s) ---
        if (Timer_HasElapsed(last_sensor_time, SENSOR_POLL_INTERVAL_MS)) {
            last_sensor_time = now;

            // Read ambient climate
            dht22_data_t dht;
            if (DHT22_Read(&dht) == DHT22_OK) {
                env_climate = dht;
            }

            // Execute Safety Interlocked Climate Control & Shading
            ClimateControl_Update(env_climate.temperature, now);
            ServoRoof_CheckThermalProtection(env_climate.temperature);
        }

        // --- Asynchronous USART1 Telemetry & Command Loop (every 3s) ---
        if (Timer_HasElapsed(last_telemetry_time, TELEMETRY_INTERVAL_MS)) {
            last_telemetry_time = now;

            char telemetry[128];
            snprintf(telemetry, sizeof(telemetry),
                     "CAMPI:T=%.1f,H=%.1f,TANK=%u,M=[%u,%u,%u,%u,%u],STG=%u\n",
                     env_climate.temperature, env_climate.humidity,
                     FloatSwitch_IsDryRunLocked() ? 0 : 1,
                     Moisture_GetPercentage(0), Moisture_GetPercentage(1),
                     Moisture_GetPercentage(2), Moisture_GetPercentage(3),
                     Moisture_GetPercentage(4),
                     slot_stages[selected_menu_slot]);
            USART1_TransmitString(telemetry);
        }

        // Parse incoming asynchronous commands from ESP32 without blocking
        char rx_cmd[64];
        if (USART1_ReadLine(rx_cmd, sizeof(rx_cmd))) {
            if (strncmp(rx_cmd, "POLLINATE", 9) == 0) {
                Pollinator_TriggerAll(env_climate.humidity, 3000);
            } else if (strncmp(rx_cmd, "SHADE_ON", 8) == 0) {
                ServoRoof_SetShading(true);
            } else if (strncmp(rx_cmd, "SHADE_OFF", 9) == 0) {
                ServoRoof_SetShading(false);
            }
        }

        // --- OLED Display Refresh (every 500ms) ---
        if (Timer_HasElapsed(last_oled_time, OLED_REFRESH_INTERVAL_MS)) {
            last_oled_time = now;
            update_oled_display();
        }
    }

    return 0;
}
