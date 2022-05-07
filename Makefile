# COMMON_FLAGS = -Wall -O3 -march=native -pipe -fomit-frame-pointer -ftree-vectorize -std=c99
# graphite
# GRAPHITE_FLAGS = -fgraphite-identity -floop-interchange -ftree-loop-distribution -floop-strip-mine 
# lto
# LTO_FLAGS = -flto=25 -fuse-linker-plugin -fno-fat-lto-objects
CFLAGS = $(COMMON_FLAGS) $(GRAPHITE_FLAGS) $(LTO_FLAGS)
CC = gcc -g -Wall -pipe -std=c99 $(CFLAGS)

all: setup libkademlia test 

setup:
	mkdir -p build/kademlia
	mkdir -p build/network
	mkdir -p lib

libkademlia:
	$(CC) -I /usr/include/rpc -c src/kademlia/kademlia.c -o build/kademlia/kademlia.o
	$(CC) -I /usr/include/rpc -c src/kademlia/node.c -o build/kademlia/node.o
	$(CC) -I /usr/include/rpc -c src/kademlia/kademlia_rpc_clnt.c -o build/kademlia/kademlia_rpc_clnt.o
	$(CC) -I /usr/include/rpc -c src/kademlia/kademlia_rpc_xdr.c -o build/kademlia/kademlia_rpc_xdr.o
	$(CC) -I /usr/include/rpc -c src/kademlia/kademlia_rpc_svc.c -o build/kademlia/kademlia_rpc_svc.o
	ar rs lib/libkademlia.a build/kademlia/kademlia.o build/kademlia/node.o build/kademlia/kademlia_rpc_clnt.o build/kademlia/kademlia_rpc_svc.o build/kademlia/kademlia_rpc_xdr.o

test:
	$(CC) -I /usr/include/rpc -I ./src/kademlia -c src/peer1.c -o build/peer1.o
	$(CC) -I /usr/include/rpc -I ./src/kademlia -c src/peer2.c -o build/peer2.o
	$(CC) build/peer1.o lib/libkademlia.a -o peer1 -luuid -pthread -ltirpc
	$(CC) build/peer2.o lib/libkademlia.a -o peer2 -luuid -pthread -ltirpc

clean:
	rm -rf build
	rm -rf lib
	rm peer1 peer2

