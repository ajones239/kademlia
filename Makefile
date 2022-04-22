COMMON_FLAGS = -O3 -march=native -pipe -fomit-frame-pointer -ftree-vectorize
# graphite
GRAPHITE_FLAGS = -fgraphite-identity -floop-interchange -ftree-loop-distribution -floop-strip-mine 
# lto
LTO_FLAGS = -flto=25 -fuse-linker-plugin -fno-fat-lto-objects
CFLAGS = $(COMMON_FLAGS) $(GRAPHITE_FLAGS) $(LTO_FLAGS)
CC = gcc -Wall -pthread $(CFLAGS)

all: libnetwork libkademlia test 

libnetwork:
	$(CC) -c src/network/client.c -o build/client.o
	$(CC) -c src/network/server.c -o build/server.o
	$(CC) -c src/network/util.c -o build/util.o
	ar rs lib/libnetwork.a build/client.o build/server.o build/util.o

libkademlia:
	$(CC) -I ./src/network -c src/kademlia/node.c -o build/node.o
	$(CC) -c src/kademlia/message.c -o build/message.o
	$(CC) -c src/kademlia/serializer.c -o build/serializer.o
	ar rs lib/libkademlia.a build/node.o build/message.o build/serializer.o

test:
	$(CC) -I ./src/kademlia -c src/main.c -o build/main.o
	$(CC) build/main.o lib/libkademlia.a lib/libnetwork.a -luuid -o test

clean:
	rm build/*
	rm lib/*
	rm test

