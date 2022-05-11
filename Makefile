DEMOFLAGS = -std=gnu99 
LIBFLAGS = -std=c99
CC = clang -g -Wall -pipe $(CFLAGS)

all: setup libkademlia demo 

setup:
	mkdir -p build/kademlia
	mkdir -p build/network
	mkdir -p lib

libkademlia:
	$(CC) $(LIBFLAGS) -I /usr/include/tirpc -c src/kademlia/kademlia.c -o build/kademlia/kademlia.o
	$(CC) $(LIBFLAGS) -I /usr/include/tirpc -c src/kademlia/node.c -o build/kademlia/node.o
	$(CC) $(LIBFLAGS) -I /usr/include/tirpc -c src/kademlia/data.c -o build/kademlia/data.o
	$(CC) $(LIBFLAGS) -I /usr/include/tirpc -c src/kademlia/kademlia_rpc_clnt.c -o build/kademlia/kademlia_rpc_clnt.o
	$(CC) $(LIBFLAGS) -I /usr/include/tirpc -c src/kademlia/kademlia_rpc_xdr.c -o build/kademlia/kademlia_rpc_xdr.o
	$(CC) $(LIBFLAGS) -I /usr/include/tirpc -c src/kademlia/kademlia_rpc_svc.c -o build/kademlia/kademlia_rpc_svc.o
	ar rs lib/libkademlia.a build/kademlia/kademlia.o build/kademlia/node.o build/kademlia/data.o build/kademlia/kademlia_rpc_clnt.o build/kademlia/kademlia_rpc_svc.o build/kademlia/kademlia_rpc_xdr.o

demo:
	$(CC) $(DEMOFLAGS) -I /usr/include/tirpc -I ./src/kademlia -c src/kpeer.c -o build/kpeer.o
	$(CC) $(DEMOFLAGS) build/kpeer.o lib/libkademlia.a -o kpeer -luuid -pthread -ltirpc

clean:
	rm -rf build
	rm -rf lib
	rm kpeer

