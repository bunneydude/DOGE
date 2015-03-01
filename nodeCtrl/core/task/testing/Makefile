IDIR =..
CC=gcc
CFLAGS= ../task_list.o


DEPS = test.h

OBJ = test.o

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< -I$(IDIR)

pgrm: $(OBJ)
	cd ..; make
	$(CC) -o $@ $^ $(CFLAGS)

.PHONY: clean

clean:
	rm -f *.o *~
