CC=gcc

DEPS = neighbor.h neighbor-config.h

OBJ = neighbor.o

PACK = 0

%.o: %.c $(DEPS)
	$(CC) -c -Os -DPACK_STRUCT=$(PACK) -o $@ $< 

pgrm: $(OBJ)
	$(CC) -o $@ $^

test: clean pgrm 

.PHONY: clean

clean:
	rm -f *.o *~ pgrm
