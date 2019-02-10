MCU=attiny2313
F_CPU=12000000UL
#F_CPU=1000000UL
PROGRAMMER?=usbtiny


#fuses: avrdude -c usbtiny -p t2313 -U lfuse:w:0xde:m -U hfuse:w:0xdf:m -U efuse:w:0xff:m

TARGET=avr-amiga-adapter

OBJECTS=main.o usbdrv/usbdrv.o usbdrv/oddebug.o usbdrv/usbdrvasm.o


CC=avr-gcc
CFLAGS+=-Wall -Werror -Os -DF_CPU=$(F_CPU) -mmcu=$(MCU) -std=gnu11 -I. -gdwarf-2 -fverbose-asm -fshort-enums 
LDFLAGS+=-Wall -Os -DF_CPU=$(F_CPU) -mmcu=$(MCU) -std=gnu11 -I. -gdwarf-2 -fverbose-asm -fshort-enums

#CFLAGS+=-ffunction-sections -fdata-sections
#LDFLAGS+=-Wl,-gc-sections -Wl,-print-gc-sections
#LDFLAGS+=-Wl,-gc-sections

#CFLAGS+=-flto
#LDFLAGS+=-flto=4

OBJCOPY=avr-objcopy
OBJDUMP=avr-objdump
BIN_FORMAT=ihex

PART=$(MCU)

BAUD=115200
PROTOCOL=stk500v1
PART=$(MCU)
AVRDUDE=avrdude -v -v

RM=rm -f

DEPS=$(patsubst %.o, .deps/%.d, $(OBJECTS))

VPATH=$(LIBDIR)


.PHONY: all clean upload
.SUFFIXES:

all: $(TARGET).hex

-include $(DEPS)

clean:
	$(RM) $(OBJECTS) $(ASM) $(TARGET).elf $(TARGET).hex $(DEPS)

upload: $(TARGET).hex
ifeq ($(PROGRAMMER),arduino)	
	$(AVRDUDE) -p$(PART) -P$(PORT) -c$(PROGRAMMER) -b$(BAUD) -U flash:w:$<
else
#	$(AVRDUDE) -p$(PART) -c$(PROGRAMMER) -B1 -U flash:w:$<
	$(AVRDUDE) -p$(PART) -c$(PROGRAMMER) -U flash:w:$<
endif

%.hex: %.elf
	$(OBJCOPY) -O $(BIN_FORMAT) -R .eeprom $< $@

$(TARGET).elf: $(OBJECTS)
	$(CC) $(LDFLAGS) -o $@ $^

$(TARGET).lss: $(TARGET).elf
	$(OBJDUMP) -h -S $< > $@

%.o: %.s
	$(CC) $(CFLAGS) -c -s -o $@ $<

%.s: %.c Makefile
	$(CC) $(CFLAGS) -S -o $@ $<
	@$(CC) -MM $(CFLAGS) $< > .deps/$*.d

%.o: %.S
	$(CC) $(CFLAGS) -x assembler-with-cpp -c $< -o $@
	@$(CC) -MM $(CFLAGS) $< > .deps/$*.d
