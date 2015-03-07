CC=gcc
PLATFORM=LINUX

ifeq ($(PLATFORM),MSP430)
	MCU=-mmcu=msp430g2553
	CC=msp430-gcc
endif


DEPS = task_list.h

OBJ = task_list.o

%.o: %.c $(DEPS)
	$(CC) -c -Os -D$(PLATFORM) -o $@ $< 
	$(CC) -c -Os -D$(PLATFORM) -o $(basename $@)_$(PLATFORM).asm $< 

pgrm: $(OBJ)
	echo "task_list built"

.PHONY: clean

clean:
	rm -f *.o *~ *.asm
