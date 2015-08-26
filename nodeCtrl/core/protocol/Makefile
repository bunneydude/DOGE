CC=gcc
LD=ld
PLATFORM=LINUX
NM=nm
SIZE=size
RUNENV=$(shell uname -o)
#MSP430LDSCRIPT=c:/ti/gcc/bin/../lib/gcc/msp430-elf/4.9.1/../../../../msp430-elf/lib/msp430.ld
#LD_INC= -Lc:/ti/gcc/lib/gcc/msp430-elf/4.9.1 -Lc:/ti/gcc/lib/gcc -Lc:/ti/gcc/lib/gcc/msp430-elf/4.9.1/../../../../msp430-elf/lib


INC = -I. -I.. -I../platform -I../radios/nrfLegacy
ifeq ($(DBG), 1)
	CFLAGS = -g3
else
	CFLAGS = -Os
	#LDFLAGS = --verbose --start-group -lc -lgcc -lcrt -lnosys --end-group --script $(MSP430LDSCRIPT)
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

DEPS = protocol.h
OBJ = protocol.o packet.o ../memory_map/memory_map.o ../network/network.o ../platform/serial_c.o ../platform/static_routes.o ../platform/doge_app.o
OBJ_ASM = $(foreach file, $(OBJ),$(basename $(file))_$(PLATFORM).asm)

TEST = testing/main.o
TEST_SIZE = testing/main_size.o
TEST_OBJ = protocol_test
TEST_ASM = $(foreach file, $(TEST),$(basename $(file))_$(PLATFORM).asm) $(basename $(TEST_SIZE))_$(PLATFORM).asm

%.o: %.c $(DEPS)
	$(CC) $(CFLAGS) -c -D$(PLATFORM) $(INC) -o $@ $<
	$(CC) $(CFLAGS) -S -D$(PLATFORM) $(INC) -o $(basename $@)_$(PLATFORM).asm $<

%.o: %.cpp $(DEPS)
	$(CC) $(CFLAGS) -c -D$(PLATFORM) $(INC) -o $@ $<
	$(CC) $(CFLAGS) -S -D$(PLATFORM) $(INC) -o $(basename $@)_$(PLATFORM).asm $<

pgrm: $(OBJ)
	@echo "build protocol"
	$(SIZE) $(OBJ)
	$(NM) -S --size-sort $(OBJ) | grep " T [A-Za-z]" | awk '{print strtonum("0x"$$2),$$3,$$4}'

test: $(TEST) $(OBJ)
	@echo "build test DBG=$(DBG)"
	@#$(LD) -o $(TEST_OBJ) $(TEST) $(OBJ) $(LD_INC) $(LDFLAGS)
	@$(CC) $(CFLAGS) -o $(TEST_OBJ) $(TEST) $(OBJ)
	@$(SIZE) $(TEST_OBJ)
	$(NM) -S --size-sort $(TEST_OBJ) | grep " T [A-Za-z]" | awk '{print strtonum("0x"$$2),$$3,$$4}'

test_size: $(TEST_SIZE) $(OBJ)
	@echo "build test size DBG=$(DBG) PLATFORM=$(PLATFORM)"
	$(CC) $(CFLAGS) -o  $(TEST_OBJ) $(TEST_SIZE) $(OBJ) -Wl,-Map,$(TEST_OBJ).map
	$(SIZE) $(TEST_OBJ)
	$(NM) -S --size-sort $(TEST_OBJ) | grep " T [A-Za-z]" | awk '{print strtonum("0x"$$2),$$3,$$4}'

.PHONY: clean

clean:
	rm -f $(TEST_OBJ)* $(OBJ) $(TEST) $(TEST_SIZE) $(OBJ_ASM) $(TEST_ASM) *~
