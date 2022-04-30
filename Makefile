COMMON_FLAGS = -Wall -O3 -march=native -pipe -fomit-frame-pointer -ftree-vectorize -std=c99
# graphite
GRAPHITE_FLAGS = -fgraphite-identity -floop-interchange -ftree-loop-distribution -floop-strip-mine 
# lto
LTO_FLAGS = -flto=25 -fuse-linker-plugin -fno-fat-lto-objects
CFLAGS = $(COMMON_FLAGS) $(GRAPHITE_FLAGS) $(LTO_FLAGS)
CC = gcc $(CFLAGS)

all: setup libnetwork libkademlia test 

setup:
	mkdir -p build/kademlia/rpc
	mkdir -p build/network

libnetwork:
	$(CC) -c src/network/client.c -o build/network/client.o
	$(CC) -c src/network/server.c -o build/network/server.o
	$(CC) -c src/network/util.c -o build/network/util.o
	ar rs lib/libnetwork.a build/network/client.o build/network/server.o build/network/util.o

libkademlia:
	$(CC) -I /usr/include/tirpc -I ./src/network -c src/kademlia/kademlia.c -o build/kademlia/kademlia.o
	$(CC) -I /usr/include/tirpc -I ./src/network -c src/kademlia/node.c -o build/kademlia/node.o
	$(CC) -I /usr/include/tirpc -c src/kademlia/message.c -o build/kademlia/message.o
#	$(CC) -c src/kademlia/serializer.c -o build/kademlia/serializer.o
	$(CC) -I /usr/include/tirpc -c src/kademlia/rpc/kademlia_rpc_clnt.c -o build/kademlia/rpc/kademlia_rpc_clnt.o
	$(CC) -I /usr/include/tirpc -c src/kademlia/rpc/kademlia_rpc_xdr.c -o build/kademlia/rpc/kademlia_rpc_xdr.o
	$(CC) -I /usr/include/tirpc -I ./src/kademlia -c src/kademlia/rpc/kademlia_rpc_svc.c -o build/kademlia/rpc/kademlia_rpc_svc.o
	ar rs lib/libkademlia.a build/kademlia/kademlia.o build/kademlia/node.o build/kademlia/message.o build/kademlia/rpc/kademlia_rpc_clnt.o build/kademlia/rpc/kademlia_rpc_svc.o build/kademlia/rpc/kademlia_rpc_xdr.o

test:
	$(CC) -I ./src/kademlia -c src/main.c -o build/main.o
	$(CC) build/main.o lib/libkademlia.a lib/libnetwork.a -o test -luuid -pthread -ltirpc

clean:
	rm -rf build
	rm lib/*
	rm test

