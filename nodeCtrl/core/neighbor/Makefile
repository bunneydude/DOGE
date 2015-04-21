CC=gcc
LD=ld
PLATFORM=LINUX
NM=nm
SIZE=size

ifeq ($(DBG), 1)
	CFLAGS = -g3 -DDBG=1
else
	CFLAGS = -Os -DDBG=0
endif

ifeq ($(PLATFORM),MSP430)
	MCU=-mmcu=msp430g2553
ifeq ($(RUNENV),Cygwin)
	CC=msp430-elf-gcc
	LD=msp430-elf-ld
	NM=msp430-elf-nm
	SIZE=msp430-elf-size
else
	CC=msp430-gcc
	LD=msp430-ld
	SIZE=msp430-size
endif
endif

DEPS = neighbor.h neighbor-config.h

OBJ = neighbor.o
OBJ_ASM = $(foreach file, $(OBJ),$(basename $(file))_$(PLATFORM).asm)

PACK = 0

%.o: %.c $(DEPS)
	$(CC) $(CFLAGS) -c -DPACK_STRUCT=$(PACK) -D$(PLATFORM) $(INC) -o $@ $<
	$(CC) $(CFLAGS) -S -DPACK_STRUCT=$(PACK) -D$(PLATFORM) $(INC) -o $(basename $@)_$(PLATFORM).asm $<

pgrm: $(OBJ)
	$(CC) $(MCU) -o $@ $^
	size pgrm

test: clean pgrm 

.PHONY: clean

clean:
	rm -f *.o *~ pgrm
