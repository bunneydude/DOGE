CC=gcc
PLATFORM=LINUX

ifeq ($(PLATFORM),MSP430)
	MCU=-mmcu=msp430g2553
	CC=msp430-gcc
endif

DEPS = nrf24l01p.h nrf24l01p-const.h

OBJ = nrf24l01p.o

%.o: %.c $(DEPS)
	$(CC) -c -Os -D$(PLATFORM) -o $@ $< 
	$(CC) -S -Os -D$(PLATFORM) -o $(basename $@)_$(PLATFORM).asm $< 

pgrm: $(OBJ)
	echo "built radio"

test: clean pgrm 

.PHONY: clean

clean:
	rm -f *.o *~ pgrm
