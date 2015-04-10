CC=gcc
LD=ld
PLATFORM=LINUX
NM=nm
SIZE=size
RUNENV=$(shell uname -o)

INC = -I. -I..
ifeq ($(DBG), 1)
	CFLAGS = -g3 -DDBG=1
else
	CFLAGS = -Os
endif

ifeq ($(PLATFORM),MSP430)
ifeq ($(RUNENV),cygwin)
	MCU=-mmcu=msp430g2553
	CC=msp430-elf-gcc
	LD=msp430-elf-ld
	SIZE=msp430-elf-size
else
	MCU=-mmcu=msp430g2553
	CC=msp430-gcc
	LD=msp430-ld
	SIZE=msp430-size
endif
endif

DEPS = protocol.h
OBJ = protocol.o packet.o ../memory_map/memory_map.o
OBJ_ASM = $(foreach file, $(OBJ),$(basename $(file))_$(PLATFORM).asm)

TEST = testing/main.o
TEST_SIZE = testing/main_size.o
TEST_OBJ = protocol_test
TEST_ASM = $(foreach file, $(TEST),$(basename $(file))_$(PLATFORM).asm) $(basename $(TEST_SIZE))_$(PLATFORM).asm

%.o: %.c $(DEPS)
	$(CC) $(CFLAGS) -c -D$(PLATFORM) $(INC) -o $@ $<
	$(CC) $(CFLAGS) -S -D$(PLATFORM) $(INC) -o $(basename $@)_$(PLATFORM).asm $<

pgrm: $(OBJ)
	@echo "build protocol"
	$(SIZE) $(OBJ)
	$(NM) -S --size-sort $(OBJ) | grep " T [A-Za-z]" | awk '{print strtonum("0x"$$2),$$3,$$4}'

test: $(TEST) $(OBJ)
	@echo "build test DBG=$(DBG)"
	@$(CC) $(CFLAGS) -o $(TEST_OBJ) $(TEST) $(OBJ)
	@$(SIZE) $(TEST_OBJ)
	$(NM) -S --size-sort $(TEST_OBJ) | grep " T [A-Za-z]" | awk '{print strtonum("0x"$$2),$$3,$$4}'

test_size: $(TEST_SIZE) $(OBJ)
	@echo "build test size DBG=$(DBG) PLATFORM=$(PLATFORM)"
	$(CC) $(CFLAGS) -o $(TEST_OBJ) $(TEST_SIZE) $(OBJ)
	$(SIZE) $(TEST_OBJ)
	$(NM) -S --size-sort $(TEST_OBJ) | grep " T [A-Za-z]" | awk '{print strtonum("0x"$$2),$$3,$$4}'

.PHONY: clean

clean:
	rm -f $(TEST_OBJ)* $(OBJ) $(TEST) $(TEST_SIZE) $(OBJ_ASM) $(TEST_ASM) *~
