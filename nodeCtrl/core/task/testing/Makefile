IDIR =..
CC=gcc
PLATFORM=LINUX
CFLAGS= ../task_list.o

ifeq ($(PLATFORM),MSP430)
	MCU=-mmcu=msp430g2553
	CC=msp430-gcc
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
	cd ..; make pgrm PLATFORM=$(PLATFORM)
	$(CC) $(MCU) -o $(basename $@)_$(PLATFORM) $^ $(CFLAGS)
	size $(basename $@)_$(PLATFORM)


all: clean pgrm

.PHONY: clean
clean:
	rm -f *.o *~ *.asm
