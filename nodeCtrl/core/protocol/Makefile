CC=gcc
LD=ld
PLATFORM=LINUX
SIZE=size
ifeq ($(PLATFORM),MSP430)
	MCU=-mmcu=msp430g2553
	CC=msp430-gcc
	LD=msp430-ld
	SIZE=msp430-size
endif


DEPS = protocol.h
OBJ = protocol.o
TEST = testing/main.o ../memory_map/memory_map.o
TEST_ASM = $(foreach file, $(TEST), $(basename $(file))_$(PLATFORM).asm)
INC = -I. -I..


%.o: %.c $(DEPS)
	$(CC) -c -Os -D$(PLATFORM) $(INC) -o $@ $<
	$(CC) -S -Os -D$(PLATFORM) $(INC) -o $(basename $@)_$(PLATFORM).asm $<

pgrm: $(OBJ)
	@echo "build protocol"

test: $(TEST) $(OBJ)
	@echo "build test"
	$(CC) -o protocol_test $(TEST) $(OBJ)

.PHONY: clean

clean:
	rm -f $(TEST) $(TEST_ASM) *.o *~ *.asm
