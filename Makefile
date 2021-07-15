CC=gcc
CFLAGS=-I.
DEPS=main.h filters.h
OBJ=main.o filters.o

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

find: $(OBJ) 
	$(CC) -o $@ $^ $(CFLAGS)
