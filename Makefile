CC = gcc
CFLAGS = -c
DEBUG_FLAGS = -g -Wall -DSWS_DEBUG
SRC = src/

TARGET = SWS
OBJ = main.o log.o error.o socket.o config.o 

.PHONY: debug all clean

debug: CFLAGS += $(DEBUG_FLAGS)
debug: clean all


all: $(OBJ)
	$(CC) -o $(TARGET) $(OBJ) -lconfig
main.o: main.c
	$(CC) $(CFLAGS) main.c
log.o: log.c log.h
	$(CC) $(CFLAGS) log.c
error.o: error.c error.h
	$(CC) $(CFLAGS) error.c
config.o: config.c config.h
	$(CC) $(CFLAGS) config.c
socket.o: socket.c socket.h
	$(CC) $(CFLAGS) socket.c

clean:
	-rm *.o $(TARGET)
