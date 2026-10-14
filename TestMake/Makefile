# Variablen im makefile
CC      = gcc
OBJECTS = main.o test1.o test2.o
LIBS    = -lm
CFLAGS  = -c -Wall

test: $(OBJECTS)
	$(CC) -o $@ $(OBJECTS) $(LIBS)

main.o: main.c
	$(CC) $(CFLAGS) $*.c
test1.o: test1.c
	$(CC) $(CFLAGS) $*.c
test2.o: test2.c
	$(CC) $(CFLAGS) $*.c
	
clean:
	rm *.o test


