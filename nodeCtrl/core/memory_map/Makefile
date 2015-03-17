CC=gcc
PLATFORM=LINUX
SIZE=size
ifeq ($(PLATFORM),MSP430)
	MCU=-mmcu=msp430g2553
	CC=msp430-gcc
	SIZE=msp430-size
endif


DEPS = memory_map.h
OBJ = memory_map.o

%.o: %.c $(DEPS)
	$(CC) -c -Os -D$(PLATFORM) -o $@ $< 
	$(CC) -c -Os -D$(PLATFORM) -o $(basename $@)_$(PLATFORM).asm $< 

pgrm: $(OBJ)
	$(CC) -o $(basename $@) $^ $(CFLAGS)
	$(SIZE) $(basename $@)

.PHONY: clean

clean:
	rm -f *.o *~ *.asm
