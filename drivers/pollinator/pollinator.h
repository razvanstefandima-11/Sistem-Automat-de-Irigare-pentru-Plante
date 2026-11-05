#ifndef POLLINATOR_H
#define POLLINATOR_H

#include <stdint.h>
#include <stdbool.h>

/**
 * @file pollinator.h
 * @brief Automated Electronic Pollination System Driver for C.A.M.P.I.
 * 
 * Controls 5 coin haptic vibration micro-motors located in the dock bases.
 * Triggered daily during the flowering/fruiting stage to induce acoustic/mechanical
 * buzz-pollination in dwarf tomatoes (Micro Tom).
 * 
 * Safety Conditioning (Section 4.2 of Specifications):
 *  - Pollination is permitted ONLY when DHT22 relative humidity is in optimal range: 50% - 70%.
 */

#define POLLINATOR_NUM_MOTORS 5
#define POLLINATOR_DEFAULT_BURST_MS 3000UL /**< 3-second mechanical vibration shock */

/**
 * @brief Initializes GPIO pins for the 5 haptic coin vibration motors.
 */
void Pollinator_Init(void);

/**
 * @brief Initiates a timed vibration shock for a specific dock.
 * Gated by ambient humidity check (50% - 70% RH).
 * 
 * @param slot_idx Index of dock (0 to 4).
 * @param ambient_humidity Current humidity from DHT22 in %.
 * @param burst_duration_ms Duration of vibration in ms.
 * @return true if shock initiated, false if rejected due to sub-optimal humidity or invalid slot.
 */
bool Pollinator_TriggerSlot(uint8_t slot_idx, float ambient_humidity, uint16_t burst_duration_ms);

/**
 * @brief Initiates a timed vibration shock across all active flowering docks.
 * 
 * @param ambient_humidity Current humidity from DHT22 in %.
 * @param burst_duration_ms Duration in ms.
 * @return true if initiated, false if rejected due to humidity.
 */
bool Pollinator_TriggerAll(float ambient_humidity, uint16_t burst_duration_ms);

/**
 * @brief Non-blocking state update for active vibration timers.
 * 
 * @param current_millis Current timestamp from Millis().
 */
void Pollinator_Update(uint32_t current_millis);

/**
 * @brief Immediately stops all vibration motors.
 */
void Pollinator_StopAll(void);

/**
 * @brief Checks if any pollination motor is currently vibrating.
 * 
 * @return true if vibrating, false if idle.
 */
bool Pollinator_IsActive(void);

#endif // POLLINATOR_H
