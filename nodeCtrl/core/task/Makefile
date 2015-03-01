CC=gcc

DEPS = task_list.h

OBJ = task_list.o

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< 

pgrm: $(OBJ)
	echo "test"

.PHONY: clean

clean:
	rm -f *.o *~
