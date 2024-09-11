# avrix make file system

# dir names
S = src
I = include
B = build

# drivers
TTY_D   = hd44780
VIDEO_D = hd44780
SOUND_D = null

# device settings
DEVICE   = atmega328p
PLATFORM = arduino 
CLOCK    = 16000000
PORT     = /dev/ttyUSB0

# util path
AVRDUDE_PATH = /home/gimura/arduino-1.8.15/hardware/tools/avr
AVRDUDE_EXE  = $(AVRDUDE_PATH)/bin/avrdude
AVRDUDE_CONF = $(AVRDUDE_PATH)/etc/avrdude.conf
C            = avr-gcc

# util commands
AVRDUDE = $(AVRDUDE_EXE) -C $(AVRDUDE_CONF) -v -V -p $(DEVICE) -c $(PLATFORM) -P $(PORT) -b115200 -D
COMPILE = $(C) -Wall -Os -g                                                          \
          -DF_CPU=$(CLOCK) -DTTY_D=$(TTY_D) -DSOUND_D=$(SOUND_D) -DVIDEO_D=$(VIDEO_D) \
		  -mmcu=$(DEVICE) -std=c99 -I $(I)                                           \

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
# drivers objects
KERNEL_DRIVERS = $(S)/drivers/sound/d_$(SOUND_D).o \
				 $(S)/drivers/tty/d_$(TTY_D).o   \
				 $(S)/drivers/video/d_$(VIDEO_D).o \

# objects for compilation
KERNEL_OBJ = $(S)/k_main.o    \
			 $(KERNEL_DRIVERS)

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