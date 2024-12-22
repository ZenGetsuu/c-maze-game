CC = gcc
CFLAGS = -Wall -Werror -ansi -pedantic -std=c89

# Usage for conditional DARK mode for some extra difficulty and fun!
ifeq ($(DARK),1)
CFLAGS += -DDARK_MODE
endif

all: treasure

treasure: main.o map.o game.o undo.o map_utils.o
	$(CC) $(CFLAGS) -o treasure main.o map.o game.o undo.o map_utils.o

main.o: main.c
	$(CC) $(CFLAGS) -c main.c

map.o: map.c map.h
	$(CC) $(CFLAGS) -c map.c

game.o: game.c game.h
	$(CC) $(CFLAGS) -c game.c

undo.o: undo.c undo.h
	$(CC) $(CFLAGS) -c undo.c

map_utils.o: map_utils.c map_utils.h
	$(CC) $(CFLAGS) -c map_utils.c

clean:
	rm -f *.o treasure
