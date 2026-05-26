# Makefile for ATmega328P Project

# Microcontroller Settings
MCU = atmega328p
F_CPU = 16000000UL

# Programmer Settings
PROGRAMMER = arduino
PORT = /dev/ttyUSB0
BAUD = 57600
#BAUD = 115200

# Board Selection (default to nano)
BOARD ?= nano

# Compiler Settings
CC = avr-gcc
OBJCOPY = avr-objcopy
AVRDUDE = avrdude

# Directories
OBJDIR = obj
BINDIR = bin

# Compiler Flags
CFLAGS = -mmcu=$(MCU) -DF_CPU=$(F_CPU) -Os -Wall -Wextra -std=gnu99
CFLAGS += -I. -Idrivers/gpio -Idrivers/button -Idrivers/interrupt -Idrivers/timer -Idrivers/eeprom -Idrivers/adc -Ibsp -Iutils

ifeq ($(BOARD), nano)
    CFLAGS += -DBOARD_NANO
else ifeq ($(BOARD), uno)
    CFLAGS += -DBOARD_UNO
else
    $(error Invalid BOARD specified. Use 'nano' or 'uno')
endif

# Source Files
SRC = src/main.c drivers/gpio/gpio.c drivers/adc/adc.c drivers/lcd/lcd.c drivers/button/button.c drivers/i2c/i2c_master.c drivers/i2c/i2c_slave.c drivers/buzzer/buzzer.c drivers/led/led.c drivers/nivel_apa/nivel_apa.c drivers/s_umiditate/s_umiditate.c drivers/pompe/pompe.c drivers/interrupt/external_interrupt.c drivers/timer/timer0.c drivers/timer/timer1.c drivers/timer/timer2.c drivers/pwm/pwm.c drivers/eeprom/eeprom.c utils/delay.c

# Object Files
# Replace .c extension with .o and prepend OBJDIR, keeping directory structure
OBJ = $(patsubst %.c,$(OBJDIR)/%.o,$(SRC))

# Target Name
TARGET = $(BINDIR)/main

# Rules
all: directories $(TARGET).hex

directories:
	@mkdir -p $(BINDIR)
	@mkdir -p $(OBJDIR)/src
	@mkdir -p $(OBJDIR)/drivers/gpio
	@mkdir -p $(OBJDIR)/drivers/interrupt
	@mkdir -p $(OBJDIR)/drivers/timer
	@mkdir -p $(OBJDIR)/drivers/eeprom
	@mkdir -p $(OBJDIR)/drivers/adc
	@mkdir -p $(OBJDIR)/utils

$(TARGET).elf: $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^

$(TARGET).hex: $(TARGET).elf
	$(OBJCOPY) -O ihex -R .eeprom $< $@

$(OBJDIR)/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c -o $@ $<

flash: $(TARGET).hex
	$(AVRDUDE) -v -p $(MCU) -c $(PROGRAMMER) -P $(PORT) -b $(BAUD) -D -U flash:w:$<:i

clean:
	rm -rf $(OBJDIR) $(BINDIR)

# Test Runner Rule (Host GCC)
# Compiles test_gpio.c + registers.c
test_gpio: directories
	@mkdir -p $(BINDIR)/test
	gcc -I. -Isrc -Idrivers/gpio -Idrivers/interrupt -Idrivers/timer -Idrivers/eeprom -Idrivers/adc -Ibsp -Iutils -Itest -Itest/mocks -DUNIT_TEST -o $(BINDIR)/test/test_gpio test/test_gpio.c test/mocks/registers.c
	@echo "Running GPIO Tests..."
	@./$(BINDIR)/test/test_gpio

test_pwm: directories
	@mkdir -p $(BINDIR)/test
	gcc -I. -Isrc -Idrivers/gpio -Idrivers/interrupt -Idrivers/timer -Idrivers/eeprom -Idrivers/adc -Ibsp -Iutils -Itest -Itest/mocks -DUNIT_TEST -o $(BINDIR)/test/test_pwm test/test_pwm.c drivers/timer/timer1.c drivers/timer/timer2.c test/mocks/registers.c
	@echo "Running PWM Tests..."
	@./$(BINDIR)/test/test_pwm

test_pwm_wrapper: directories
	@mkdir -p $(BINDIR)/test
	gcc -I. -Isrc -Idrivers/gpio -Idrivers/interrupt -Idrivers/timer -Idrivers/pwm -Idrivers/eeprom -Idrivers/adc -Ibsp -Iutils -Itest -Itest/mocks -DUNIT_TEST -o $(BINDIR)/test/test_pwm_wrapper test/test_pwm_wrapper.c drivers/timer/timer1.c drivers/timer/timer2.c drivers/pwm/pwm.c test/mocks/registers.c
	# Run all tests
test: test_gpio test_pwm test_pwm_wrapper
	@echo "All tests passed!"

# Code Coverage Target
coverage: directories
	@mkdir -p $(BINDIR)/coverage
	@echo "Compiling for Coverage..."
	# Compile objects separately to ensure .gcno files are named correctly
	gcc --coverage -O0 -c -I. -Isrc -Idrivers/gpio -Idrivers/interrupt -Idrivers/timer -Idrivers/eeprom -Idrivers/adc -Ibsp -Iutils -Itest -Itest/mocks -DUNIT_TEST -o $(BINDIR)/coverage/test_gpio.o test/test_gpio.c
	gcc --coverage -O0 -c -I. -Isrc -Itest/mocks -DUNIT_TEST -o $(BINDIR)/coverage/registers.o test/mocks/registers.c
	# Link
	gcc --coverage -O0 -o $(BINDIR)/coverage/test_gpio_cov $(BINDIR)/coverage/test_gpio.o $(BINDIR)/coverage/registers.o
	@echo "Running Tests to generate profile data..."
	@./$(BINDIR)/coverage/test_gpio_cov
	@echo "Generating GCOV reports..."
	# Run gcov on the object file
	gcov -o $(BINDIR)/coverage/test_gpio.o test/test_gpio.c
	# Move gcov files to coverage dir
	mv *.gcov $(BINDIR)/coverage/

# HTML Coverage Report (Requires lcov)
coverage-html: coverage
	@mkdir -p $(BINDIR)/coverage/html
	@echo "Capturing LCOV data..."
	lcov --capture --directory $(BINDIR)/coverage --base-directory . --output-file $(BINDIR)/coverage/coverage.info --ignore-errors unsupported
	@echo "Filtering unwanted files (system libraries, tests, mocks)..."
	lcov --remove $(BINDIR)/coverage/coverage.info '/usr/*' 'test/*' 'utils/*' --output-file $(BINDIR)/coverage/coverage_cleaned.info --ignore-errors unused,unsupported
	@echo "Generating HTML report..."
	genhtml $(BINDIR)/coverage/coverage_cleaned.info --output-directory $(BINDIR)/coverage/html --ignore-errors unsupported
	@echo "Report generated at $(BINDIR)/coverage/html/index.html"

.PHONY: all flash clean directories test coverage coverage-html