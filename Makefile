# ==============================================================================
# C.A.M.P.I. - Automated Control & Monitoring of Indoor Plants
# Precision Smart Farming System for Hydroponic Dwarf Tomatoes (Micro Tom)
# Master Controller: Microchip ATmega2560 (16MHz, Bare-Metal C / avr-gcc)
# ==============================================================================

# Target Microcontroller & Frequency
MCU   = atmega2560
F_CPU = 16000000UL

# Programmer Settings (Arduino Mega 2560 uses wiring/STK500v2 protocol)
PROGRAMMER ?= wiring
PORT       ?= /dev/ttyACM0
BAUD       ?= 115200

# Toolchain Binaries
CC       = avr-gcc
OBJCOPY  = avr-objcopy
OBJDUMP  = avr-objdump
AVR_SIZE = avr-size
AVRDUDE  = avrdude

# Output Directories
OBJDIR = obj
BINDIR = bin

# Include Directories
INCLUDES = -I. \
           -Isrc \
           -Iutils \
           -Idrivers/gpio \
           -Idrivers/timer \
           -Idrivers/adc \
           -Idrivers/usart \
           -Idrivers/dht22 \
           -Idrivers/moisture \
           -Idrivers/float_switch \
           -Idrivers/dock_sensor \
           -Idrivers/pumps \
           -Idrivers/valves \
           -Idrivers/climate_control \
           -Idrivers/grow_lights \
           -Idrivers/servo_roof \
           -Idrivers/pollinator \
           -Idrivers/i2c \
           -Idrivers/ssd1306 \
           -Idrivers/encoder \
           -Idrivers/buzzer \
           -Idrivers/eeprom

# Compiler Optimization & Diagnostic Flags
CFLAGS  = -mmcu=$(MCU) -DF_CPU=$(F_CPU) -Os -Wall -Wextra -std=gnu99
CFLAGS += -funsigned-char -funsigned-bitfields -fpack-struct -fshort-enums
CFLAGS += -ffunction-sections -fdata-sections
CFLAGS += $(INCLUDES)

# Linker Flags (Dead code elimination and math library)
LDFLAGS = -mmcu=$(MCU) -Wl,--gc-sections -lm

# Source Files List
SRC = src/main.c \
      drivers/gpio/gpio.c \
      drivers/timer/timer.c \
      drivers/adc/adc.c \
      drivers/usart/usart.c \
      drivers/dht22/dht22.c \
      drivers/moisture/moisture.c \
      drivers/float_switch/float_switch.c \
      drivers/dock_sensor/dock_sensor.c \
      drivers/pumps/pumps.c \
      drivers/valves/valves.c \
      drivers/climate_control/climate_control.c \
      drivers/grow_lights/grow_lights.c \
      drivers/servo_roof/servo_roof.c \
      drivers/pollinator/pollinator.c \
      drivers/i2c/i2c.c \
      drivers/ssd1306/ssd1306.c \
      drivers/encoder/encoder.c \
      drivers/buzzer/buzzer.c \
      drivers/eeprom/eeprom.c \
      utils/delay.c

# Object Files Mapping
OBJ = $(patsubst %.c,$(OBJDIR)/%.o,$(SRC))

# Target Binary
TARGET = $(BINDIR)/main

# Default Build Target
all: $(TARGET).hex size

# Link Executable ELF
$(TARGET).elf: $(OBJ)
	@mkdir -p $(BINDIR)
	@echo "Linking target ELF: $@"
	$(CC) $(LDFLAGS) -o $@ $^

# Generate Intel HEX
$(TARGET).hex: $(TARGET).elf
	@echo "Generating HEX: $@"
	$(OBJCOPY) -O ihex -R .eeprom $< $@

# Compile C Source Files
$(OBJDIR)/%.o: %.c
	@mkdir -p $(dir $@)
	@echo "Compiling: $<"
	$(CC) $(CFLAGS) -c -o $@ $<

# Display Memory Footprint
size: $(TARGET).elf
	@echo "\n================ Memory Utilization ================"
	@$(AVR_SIZE) --format=avr --mcu=$(MCU) $(TARGET).elf
	@echo "====================================================\n"

# Flash Target via avrdude
flash: $(TARGET).hex
	@echo "Flashing $< to $(MCU) via $(PROGRAMMER) on $(PORT)..."
	$(AVRDUDE) -v -p $(MCU) -c $(PROGRAMMER) -P $(PORT) -b $(BAUD) -D -U flash:w:$<:i

# Clean Build Artifacts
clean:
	@echo "Cleaning build directory..."
	rm -rf $(OBJDIR) $(BINDIR)

.PHONY: all clean flash size
