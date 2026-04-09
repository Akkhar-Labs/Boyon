# Boyon Build System
CC=gcc
CFLAGS=-Iinclude

all:
	$(CC) $(CFLAGS) src/*.c -o bin/boyon
