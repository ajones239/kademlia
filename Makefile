COMMON_FLAGS = -O3 -march=native -pipe -fomit-frame-pointer -ftree-vectorize
# graphite
GRAPHITE_FLAGS = -fgraphite-identity -floop-interchange -ftree-loop-distribution -floop-strip-mine 
# lto
LTO_FLAGS = -flto=25 -fuse-linker-plugin -fno-fat-lto-objects
CFLAGS = $(COMMON_FLAGS) $(GRAPHITE_FLAGS) $(LTO_FLAGS)
CC = gcc -Wall -pthread $(CFLAGS)

clean: lib/libnetwork.a lib/libkademlia.a test
	rm lib/*
	rm test

libnetwork:
	$(CC) -c src/network/client.c -o client.o
	$(CC) -c src/network/server.c -o server.o
	$(CC) -c src/network/util.c -o util.o
	ar rs lib/libnetwork.a client.o server.o util.o
	rm *.o

libkademlia:
	$(CC) -c src/kademlia/node.c -o node.o
	ar rs lib/libkademlia.a node.o 
	rm *.o

test:
	$(CC) -I ./src/kademlia -c src/main.c -o main.o
	$(CC) lib/libkademlia.a main.o -o test
	rm *.o

all: libnetwork libkademlia test 

.PHONY: lib
