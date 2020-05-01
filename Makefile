CC = gcc
CFLAGS =  -Wall
BIN = nuke

SRC = src/main.c \
      src/nuke.c

OBJS = src/main.o \
       src/nuke.o

$(BIN): $(OBJS)
	$(CC) $(OBJS) $(CFLAGS) -o $(BIN)

.PHONY: clean
.SUFFIXES: .o .c

.c.o:
	$(CC) -Iinclude -c $< -o $@ $(CFLAGS)

install: $(BIN)
	cp $(BIN) /usr/bin/

clean:
	rm -f $(OBJS) $(BIN)
