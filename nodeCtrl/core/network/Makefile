CC=gcc
LD=ld
PLATFORM=LINUX
NM=nm
SIZE=size
RUNENV=$(shell uname -o)
INC = -I. -I.. -I../platform  -I../protocol -I../radios/nrfLegacy
ifeq ($(DBG), 1)
	CFLAGS = -g3
else
	CFLAGS = -Os
endif

ifeq ($(PLATFORM),MSP430)
	MCU=-mmcu=msp430g2553
	CC=msp430-gcc
endif

DEPS = network.h ../neighbor/neighbor.h ../routing/routing.h ../memory_map/memory_map.h 
OBJ = network.o ../memory_map/memory_map.o ../platform/serial_c.o
OBJ_ASM = $(foreach file, $(OBJ),$(basename $(file))_$(PACK)_$(PLATFORM).asm)

TEST = testing/test.o
TEST_OBJ = network_test
TEST_ASM = $(foreach file, $(TEST),$(basename $(file))_$(PLATFORM).asm) $(basename $(TEST))_$(PACK)_$(PLATFORM).asm

PACK = 0

%.o: %.c $(DEPS)
	$(CC) $(CFLAGS) -c -DPACK_STRUCT=$(PACK) -D$(PLATFORM) $(INC) -o $@ $<
	$(CC) $(CFLAGS) -S -DPACK_STRUCT=$(PACK) -D$(PLATFORM) $(INC) -o $(basename $@)_$(PACK)_$(PLATFORM).asm $< 

%.o: %.cpp $(DEPS)
	$(CC) $(CFLAGS) -c -D$(PLATFORM) $(INC) -o $@ $<
	$(CC) $(CFLAGS) -S -D$(PLATFORM) $(INC) -o $(basename $@)_$(PLATFORM).asm $<


pgrm: $(OBJ)
	@echo "built network"

test: $(TEST) $(OBJ)
	@echo "build test DBG=$(DBG)"
	@$(CC) $(CFLAGS) -o $(TEST_OBJ) $(TEST) $(OBJ)
	@$(SIZE) $(TEST_OBJ)
	$(NM) -S --size-sort $(TEST_OBJ) | grep " T [A-Za-z]" | awk '{print strtonum("0x"$$2),$$3,$$4}'

.PHONY: clean

clean:
	rm -f $(TEST_OBJ)* $(OBJ) $(TEST) $(TEST_SIZE) $(OBJ_ASM) $(TEST_ASM) *~
