CC=gcc

DEPS = neighbor.h neighbor-config.h

OBJ = neighbor.o

%.o: %.c $(DEPS)
	$(CC) -c -Os -o $@ $< 

pgrm: $(OBJ)
	$(CC) -o $@ $^

.PHONY: clean

clean:
	rm -f *.o *~ pgrm
