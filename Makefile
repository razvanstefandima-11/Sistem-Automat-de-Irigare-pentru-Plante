# ==============================================================================
# C.A.M.P.I. - Automated Control & Monitoring of Indoor Plants
# Precision Smart Farming System for Hydroponic Dwarf Tomatoes (Micro Tom)
# Master Unit: Microchip ATmega2560 (Bare-Metal C / avr-gcc)
# ==============================================================================

# Microcontroller & Frequency Settings
MCU   = atmega2560
F_CPU = 16000000UL

# Programmer Settings (Arduino Mega 2560 Bootloader uses STK500v2 / wiring protocol)
PROGRAMMER ?= wiring
PORT       ?= /dev/ttyACM0
BAUD       ?= 115200

# Board Selection (default: mega2560)
BOARD ?= mega2560

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
           -Ibsp \
           -Iutils \
           -Idrivers/gpio \
           -Idrivers/adc \
           -Idrivers/button \
           -Idrivers/buzzer \
           -Idrivers/eeprom \
           -Idrivers/i2c \
           -Idrivers/interrupt \
           -Idrivers/lcd \
           -Idrivers/led \
           -Idrivers/nivel_apa \
           -Idrivers/pompe \
           -Idrivers/pwm \
           -Idrivers/s_umiditate \
           -Idrivers/timer \
           -Idrivers/usart \
           -Idrivers/dht22 \
           -Idrivers/capacitive_moisture \
           -Idrivers/float_switch \
           -Idrivers/dosing_system \
           -Idrivers/thermal_ptc \
           -Idrivers/grow_lights

# Compiler Optimization & Diagnostics Flags
CFLAGS  = -mmcu=$(MCU) -DF_CPU=$(F_CPU) -Os -Wall -Wextra -std=gnu99
CFLAGS += -funsigned-char -funsigned-bitfields -fpack-struct -fshort-enums
CFLAGS += -ffunction-sections -fdata-sections
CFLAGS += $(INCLUDES)

# Linker Flags (Garbage collect unused sections to conserve flash)
LDFLAGS = -mmcu=$(MCU) -Wl,--gc-sections

# Board Configuration Definitions
ifeq ($(BOARD), mega2560)
    CFLAGS += -DBOARD_MEGA2560
else ifeq ($(BOARD), mega)
    CFLAGS += -DBOARD_MEGA2560
else ifeq ($(BOARD), nano)
    CFLAGS += -DBOARD_NANO
else ifeq ($(BOARD), uno)
    CFLAGS += -DBOARD_UNO
else
    $(error Invalid BOARD specified: '$(BOARD)'. Supported: mega2560, mega, nano, uno)
endif

# Source Files List
SRC = src/main.c \
      drivers/gpio/gpio.c \
      drivers/adc/adc.c \
      drivers/lcd/lcd.c \
      drivers/usart/usart.c \
      drivers/button/button.c \
      drivers/i2c/i2c_master.c \
      drivers/i2c/i2c_slave.c \
      drivers/buzzer/buzzer.c \
      drivers/led/led.c \
      drivers/nivel_apa/nivel_apa.c \
      drivers/s_umiditate/s_umiditate.c \
      drivers/pompe/pompe.c \
      drivers/interrupt/external_interrupt.c \
      drivers/timer/timer0.c \
      drivers/timer/timer1.c \
      drivers/timer/timer2.c \
      drivers/pwm/pwm.c \
      drivers/eeprom/eeprom.c \
      drivers/dht22/dht22.c \
      drivers/capacitive_moisture/capacitive_moisture.c \
      drivers/float_switch/float_switch.c \
      drivers/dosing_system/dosing_system.c \
      drivers/thermal_ptc/thermal_ptc.c \
      drivers/grow_lights/grow_lights.c \
      utils/delay.c

# Object Files Mapping
OBJ = $(patsubst %.c,$(OBJDIR)/%.o,$(SRC))

# Target Binary Names
TARGET = $(BINDIR)/main

# Default Build Target
all: $(TARGET).hex size

# Link Target ELF
$(TARGET).elf: $(OBJ)
	@mkdir -p $(BINDIR)
	@echo "Linking ELF target: $@"
	$(CC) $(LDFLAGS) -o $@ $^

# Generate Intel HEX for Flash Programmer
$(TARGET).hex: $(TARGET).elf
	@echo "Generating HEX: $@"
	$(OBJCOPY) -O ihex -R .eeprom $< $@

# Compile C Sources into Object Files
$(OBJDIR)/%.o: %.c
	@mkdir -p $(dir $@)
	@echo "Compiling: $<"
	$(CC) $(CFLAGS) -c -o $@ $<

# Display Memory Utilization (Flash & SRAM)
size: $(TARGET).elf
	@echo "\n================ Memory Utilization ================"
	@$(AVR_SIZE) --format=avr --mcu=$(MCU) $(TARGET).elf
	@echo "====================================================\n"

# Flash Firmware via avrdude
flash: $(TARGET).hex
	@echo "Flashing $< to $(MCU) on $(PORT) at $(BAUD) baud..."
	$(AVRDUDE) -v -p $(MCU) -c $(PROGRAMMER) -P $(PORT) -b $(BAUD) -D -U flash:w:$<:i

# Clean Build Artifacts
clean:
	@echo "Cleaning build artifacts..."
	rm -rf $(OBJDIR) $(BINDIR)

# Host Test Targets (using Native GCC)
test_gpio:
	@mkdir -p $(BINDIR)/test
	gcc -I. -Isrc -Idrivers/gpio -Idrivers/interrupt -Idrivers/timer -Idrivers/eeprom -Idrivers/adc -Ibsp -Iutils -Itest -Itest/mocks -DUNIT_TEST -o $(BINDIR)/test/test_gpio test/test_gpio.c test/mocks/registers.c
	@echo "Running GPIO Tests..."
	@./$(BINDIR)/test/test_gpio

test_pwm:
	@mkdir -p $(BINDIR)/test
	gcc -I. -Isrc -Idrivers/gpio -Idrivers/interrupt -Idrivers/timer -Idrivers/eeprom -Idrivers/adc -Ibsp -Iutils -Itest -Itest/mocks -DUNIT_TEST -o $(BINDIR)/test/test_pwm test/test_pwm.c drivers/timer/timer1.c drivers/timer/timer2.c test/mocks/registers.c
	@echo "Running PWM Tests..."
	@./$(BINDIR)/test/test_pwm

test_pwm_wrapper:
	@mkdir -p $(BINDIR)/test
	gcc -I. -Isrc -Idrivers/gpio -Idrivers/interrupt -Idrivers/timer -Idrivers/pwm -Idrivers/eeprom -Idrivers/adc -Ibsp -Iutils -Itest -Itest/mocks -DUNIT_TEST -o $(BINDIR)/test/test_pwm_wrapper test/test_pwm_wrapper.c drivers/timer/timer1.c drivers/timer/timer2.c drivers/pwm/pwm.c test/mocks/registers.c

test: test_gpio test_pwm test_pwm_wrapper
	@echo "All host tests passed successfully!"

.PHONY: all clean flash size test test_gpio test_pwm test_pwm_wrapper
