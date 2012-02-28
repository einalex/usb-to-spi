PRG            = spi_controller
OBJ            = spi_controller.o usart.o fifo.o spi.o
MCU_TARGET     = atmega328p
OPTIMIZE       = -Os

DEFS           =
LIBS           =

# You should not have to change anything below here.

CC              = avr-gcc

AVRDUDE		= avrdude
AVRDUDE_PORT	= /dev/ttyACM0
AVRDUDE_PART	= m328p
AVRDUDE_PROG	= stk500v1

CFLAGS        = -g -Wall $(OPTIMIZE) -mmcu=$(MCU_TARGET) $(DEFS)
#LDFLAGS       = -Wl,-Map,$(PRG).map -Wl,-u,vfscanf -lscanf_flt -lm -Wl,-u,vfprintf -lprintf_flt
LDFLAGS       = -Wl,-Map,$(PRG).map

OBJCOPY        = avr-objcopy
OBJDUMP        = avr-objdump

all: $(PRG).elf lst text eeprom

$(PRG).elf: $(OBJ)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^ $(LIBS)

clean:
	rm -rf *.o $(PRG).elf *.eps *.png *.pdf *.bak
	rm -rf *.lst *.map $(EXTRA_CLEAN_FILES)

lst:  $(PRG).lst

%.lst: %.elf
	$(OBJDUMP) -h -S $< > $@
	
# Rules for programming the chip

program: $(PRG).hex
	$(AVRDUDE) -p $(AVRDUDE_PART) -c $(AVRDUDE_PROG) -P $(AVRDUDE_PORT) -F -U flash:w:$(PRG).hex 
	

# Rules for building the .text rom images

text: hex bin srec

hex:  $(PRG).hex
bin:  $(PRG).bin
srec: $(PRG).srec

%.hex: %.elf
	$(OBJCOPY) -j .text -j .data -O ihex $< $@

%.srec: %.elf
	$(OBJCOPY) -j .text -j .data -O srec $< $@

%.bin: %.elf
	$(OBJCOPY) -j .text -j .data -O binary $< $@

# Rules for building the .eeprom rom images

eeprom: ehex ebin esrec

ehex:  $(PRG)_eeprom.hex
ebin:  $(PRG)_eeprom.bin
esrec: $(PRG)_eeprom.srec

%_eeprom.hex: %.elf
	$(OBJCOPY) -j .eeprom --change-section-lma .eeprom=0 -O ihex $< $@

%_eeprom.srec: %.elf
	$(OBJCOPY) -j .eeprom --change-section-lma .eeprom=0 -O srec $< $@

%_eeprom.bin: %.elf
	$(OBJCOPY) -j .eeprom --change-section-lma .eeprom=0 -O binary $< $@
