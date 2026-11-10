# Makefile for Kommando-Interpreter (Uebung 6)

CC      = gcc
CFLAGS  = -std=c99 -Wall -Wextra -g
TARGET  = cmd
SRCS    = main.c CommandInterpreter.c CommandTable.c
OBJS    = $(SRCS:.c=.o)

# build the program
all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS)

# build one object file from one source file
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# header dependencies
main.o: main.c CommandInterpreter.h CommandTable.h
CommandInterpreter.o: CommandInterpreter.c CommandInterpreter.h CommandTable.h
CommandTable.o: CommandTable.c CommandTable.h CommandInterpreter.h

# build and start the program
run: $(TARGET)
	./$(TARGET)

# remove all built files
clean:
	rm -f $(OBJS) $(TARGET) $(TARGET).exe

.PHONY: all run clean
