#ifndef SSD1306_H
#define SSD1306_H

#include <stdint.h>

/**
 * @file ssd1306.h
 * @brief SSD1306 / SH1106 OLED Display I2C Driver for ATmega2560 (C.A.M.P.I. HMI).
 * 
 * Supports 128x64 OLED display over hardware I2C bus (address 0x3C).
 * Includes embedded ASCII 5x7 font in PROGMEM flash for text rendering.
 */

#define SSD1306_I2C_ADDR   0x3C /**< Standard 7-bit I2C address */
#define SSD1306_WIDTH      128
#define SSD1306_HEIGHT     64

/**
 * @brief Initializes the OLED display and clears screen buffer.
 */
void SSD1306_Init(void);

/**
 * @brief Clears all pages of the display (blanks screen).
 */
void SSD1306_Clear(void);

/**
 * @brief Sets the cursor position for text rendering.
 * 
 * @param line Page/line number (0 to 7 for 128x64).
 * @param col Column index (0 to 127).
 */
void SSD1306_SetCursor(uint8_t line, uint8_t col);

/**
 * @brief Writes a single character to the OLED using 5x7 font.
 * 
 * @param c ASCII character.
 */
void SSD1306_WriteChar(char c);

/**
 * @brief Writes a null-terminated string to the OLED.
 * 
 * @param str Pointer to string.
 */
void SSD1306_WriteString(const char* str);

/**
 * @brief Writes a formatted integer.
 * 
 * @param val Integer value.
 */
void SSD1306_WriteInt(int16_t val);

/**
 * @brief Draws a horizontal progress bar (0 - 100%).
 * 
 * @param line Page line (0 to 7).
 * @param col Starting column (0 to 127).
 * @param width Width in pixels.
 * @param percentage Fill percentage (0 to 100).
 */
void SSD1306_DrawProgressBar(uint8_t line, uint8_t col, uint8_t width, uint8_t percentage);

#endif // SSD1306_H
