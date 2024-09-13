# avrix make file system

# dir names
S = src
I = include
B = build

# drivers
IN_D   = uart
OUT_D  = uart
NULL_D = null
FS_D   = null

ENABLED_DRIVERS = uart

# device settings
DEVICE   = atmega328p
PLATFORM = arduino 
CLOCK    = 16000000
PORT     = /dev/ttyUSB0

# util path
ARDUINO_TOOLS_PATH = /home/gimura/arduino-1.8.15/hardware/tools/avr
AVRDUDE_EXE  = $(ARDUINO_TOOLS_PATH)/bin/avrdude
AVRDUDE_CONF = $(ARDUINO_TOOLS_PATH)/etc/avrdude.conf
C            = avr-gcc

# defines
ARCH    = -DF_CPU=$(CLOCK)

DRIVERS = -DIN_D=$(IN_D)   \
          -DOUT_D=$(OUT_D) \
		  -DNULL_D=$(NULL_D)

DEFINES = $(ARCH) $(DRIVERS)

# util commands
AVRDUDE = $(AVRDUDE_EXE) -C $(AVRDUDE_CONF) -v -V -p $(DEVICE) -c $(PLATFORM) -P $(PORT) -b115200 -D
COMPILE = $(C) -Wall -Os -g                \
    	  $(DEFINES)                       \
		  -mmcu=$(DEVICE) -std=c99 -I $(I) \

# command execution

# util lables
all: clean kernel.hex

flash: kernel.hex
	$(AVRDUDE) kernel.hex 

clean: kernel_clean
#	FIXME: fix build directory
#	rm -f -r $(B)
#	mkdir $(B)

debug: kernel.elf
	simavr -g -m $(DEVICE) kernel.elf

# kernel compilation
# standart drivers
INTERFACE_DRIVERS = $(S)/drivers/std/in/d_$(IN_D).o     \
                    $(S)/drivers/std/out/d_$(OUT_D).o   \
					$(S)/drivers/std/null/d_$(NULL_D).o \
					$(S)/drivers/std/fs/d_$(NULL_D).o   \

ENABLED_DRIVERS_OBJ = $(foreach var,$(ENABLED_DRIVERS),$(S)/drivers/d_$(var).o)

# objects for compilation
KERNEL_OBJ = $(S)/k_main.o          \
			 $(INTERFACE_DRIVERS)   \
			 $(ENABLED_DRIVERS_OBJ)

kernel.hex: kernel.elf
	rm -f kernel.hex
	avr-objcopy -j .text -j .data -O ihex kernel.elf kernel.hex
	avr-size --format=avr --mcu=$(DEVICE) kernel.elf

kernel.elf: $(KERNEL_OBJ)
	$(COMPILE) -o kernel.elf $(KERNEL_OBJ)

kernel_clean:
	rm -f kernel.hex kernel.elf $(KERNEL_OBJ)

# file compilation

# .c files
.c.o:
	$(COMPILE) -c $< -o $@

# .s files
.S.o:
	$(COMPILE) -x assembler-with-cpp -c $< -o $@

# what?
.c.s:
	$(COMPILE) -S $< -o $@