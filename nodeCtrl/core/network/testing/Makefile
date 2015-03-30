IDIR =..
CC=gcc
NM=nm
SIZE=size

PLATFORM=LINUX
PACK=0

CFLAGS= ../network.o ../../memory_map/memory_map.o

ifeq ($(PLATFORM),MSP430)
	MCU=-mmcu=msp430g2553
	CC=msp430-gcc
	NM=msp430-nm
	SIZE=msp430-size
else
	CFLAGS += -lrt
endif


DEPS = test.h

OBJ = test.o

%.o: %.c $(DEPS)
	$(CC) -c -Os -D$(PLATFORM) -o $@ $< -I$(IDIR)
	$(CC) -S -Os -D$(PLATFORM) -o $(basename $@)_$(PLATFORM).asm $<  -I$(IDIR)


pgrm: $(OBJ)
	cd ..; make clean
	cd ..; make pgrm PLATFORM=$(PLATFORM) PACK_STRUCT=$(PACK)
	$(CC) $(MCU) -o $(basename $@)_$(PLATFORM) $^ $(CFLAGS)
	$(SIZE) $(basename $@)_$(PLATFORM)
	$(NM) -S $(basename $@)_$(PLATFORM) | grep " T [a-z]" | awk '{print strtonum("0x"$$2),$$3,$$4}'

all: clean pgrm

.PHONY: clean
clean:
	rm -f *.o *~ *.asm
