# Microcontroller settings
# Chip model
MCU = atmega328p
#Clock Frequeny variable
F_CPU = 16000000UL

# Programmer settings
PROGRAMMER = arduino
# USB Serial
PORT = /dev/ttyUSB0
# Baud rate for serial commnication
BAUD = 115200

# Compiler settings
CC = avr-gcc #compiler for AVR Micro controllers
OBJCOPY = avr-objcopy # tool to convert binary formats
#AVR Downloader/Uploader
AVRDUDE = avrdude
# Compiler flags: Tells the compiler to generate code specifically for the ATmega328P && Makes F_CPU available in your C code
CFLAGS = -mmcu=$(MCU) -DF_CPU=$(F_CPU) -Wall

# Target files
TARGET = main
SRC = main.c

all: hex flash

# hex rule: compile and generate hex file
hex: $(TARGET).hex

# Rule to create main.bin form main.c
$(TARGET).bin: $(SRC)
	$(CC) $(CFLAGS) -o $(TARGET).bin $(SRC)

# Rule to create main.hex from main.bin
$(TARGET).hex: $(TARGET).bin
	$(OBJCOPY) -O ihex -R .eeprom $(TARGET).bin $(TARGET).hex

# flash rule: upload hex to microcontroller
# flash:w:$(TARGET).hex:i => Flash memory to file to write, i; input format
flash: $(TARGET).hex
	 	$(AVRDUDE) -p $(MCU) -c $(PROGRAMMER)  -P $(PORT) -b $(BAUD) -U flash:w:$(TARGET).hex:i

# clean rule: delete generated files
clean:
	rm -f $(TARGET).hex $(TARGET).bin

.PHONY: hex flash clean