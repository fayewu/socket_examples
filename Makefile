CC = gcc
CFLAGS = -c
DEBUG_FLAGS = -g -Wall -DSWS_DEBUG
SRC = src/

TARGET = SWS
OBJ = main.o log.o utils.o socket.o config.o interaction.o

.PHONY: debug all clean

debug: CFLAGS += $(DEBUG_FLAGS)
debug: clean all


all: $(OBJ)
	$(CC) -o $(TARGET) $(OBJ) -lconfig
main.o: main.c
	$(CC) $(CFLAGS) main.c
log.o: log.c log.h
	$(CC) $(CFLAGS) log.c
utils.o: utils.c utils.h
	$(CC) $(CFLAGS) utils.c
config.o: config.c config.h
	$(CC) $(CFLAGS) config.c
socket.o: socket.c socket.h
	$(CC) $(CFLAGS) socket.c
interaction.o: interaction.c interaction.h
	$(CC) $(CFLAGS) interaction.c

clean:
	-rm *.o $(TARGET)
