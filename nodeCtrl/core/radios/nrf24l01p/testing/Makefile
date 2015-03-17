IDIR =..
CC=gcc
NM=nm
SIZE=size

PLATFORM=LINUX
CFLAGS= ../nrf24l01p.o

DEPS = test.h
OBJ = test.o

ifeq ($(PLATFORM),MSP430)
	MCU=-mmcu=msp430g2553
	CC=msp430-gcc
	NM=msp430-nm
	SIZE=msp430-size
	OBJ += platform-msp430.o
	DEPS += platform-msp430.h
else
	OBJ += platform-edison.o
	DEPS += platform-edison.h
endif



%.o: %.c $(DEPS)
	$(CC) -c -Os -D$(PLATFORM) -o $@ $< -I$(IDIR)
	$(CC) -S -Os -D$(PLATFORM) -o $(basename $@)_$(PLATFORM).asm $<  -I$(IDIR)


pgrm: $(OBJ)
	cd ..; make clean
	cd ..; make pgrm PLATFORM=$(PLATFORM)
	$(CC) $(MCU) -o $(basename $@)_$(PLATFORM) $^ $(CFLAGS)
	$(SIZE) $(basename $@)_$(PLATFORM)
	$(NM) -S $(basename $@)_$(PLATFORM) | grep " T [a-z]" | awk '{print strtonum("0x"$$2),$$3,$$4}'

all: clean pgrm

.PHONY: clean
clean:
	rm -f *.o *~ *.asm
	rm -f pgrm_LINUX
	rm -f pgrm_MSP430
