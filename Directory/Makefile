CC = gcc
CFLAGS = -Wall -Wextra

TARGET = directory
OBJECTS = main.o directory.o

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) -o $@ $^

main.o: main.c directory.h
	$(CC) $(CFLAGS) -c $< -o $@

directory.o: directory.c directory.h
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJECTS) $(TARGET)
