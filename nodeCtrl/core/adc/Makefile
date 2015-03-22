CC=gcc
PLATFORM=LINUX
SIZE=size

ifeq ($(PLATFORM),MSP430)
	MCU=-mmcu=msp430g2553
	CC=msp430-gcc
	SIZE=msp430-size
endif

DEPS = adc.h ../memory_map/memory_map.h

OBJ = adc.o

PACK = 0

%.o: %.c $(DEPS)
	$(CC) -c -Os -D$(PLATFORM) -o $@ $< 
	$(CC) -S -Os -D$(PLATFORM) -o $(basename $@)_$(PLATFORM).asm $< 

pgrm: $(OBJ)
	$(CC) $(MCU) -o $(basename $@)_$(PLATFORM) $^ $(CFLAGS)
	$(SIZE) $(basename $@)_$(PLATFORM)

test: clean pgrm 

.PHONY: clean

clean:
	rm -f *.o *~ pgrm
