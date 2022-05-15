# Kademlia - A C library for distributed hash tables.

## Overview of Kademlia
Kademlia is a distributed hash table for peer-to-peer networks. It provides a transparent and distributed key-value data storage layer to peers in the network. Each node has an ID (128 bit UUID in this implementation) which is also used for keys. Data with keys closest to a specific node ID are stored on that node where distance is defined as the XOR of two IDs/keys. Many implementations include data replication, asynchronous messages, and resistence to certain types of denial of service attacks. For specifics on how Kademlia works, please refer to the <a href="https://cs.brown.edu/courses/csci2950-g/papers/kademlia.pdf">whitepaper</a>.

## Maturity of Project
This project is a work in progress and has not yet reached the level of maturity where it can be used in production code. Basic node creation and routing, as well as data storage and fetching, is supported. Currently this project does not support UDP as a transport protocol (UDP and asynchronous messages are coming soon), data replication, node deletion, or denial of service resistance.

## Dependencies
Only tested on Debian and Gentoo.

### Runtime
- UUID C library (uuid/uuid.h). Install package uuid-dev on Debian, included by default on Gentoo.
- RPC library (tirpc/rpc/rpc/h), rpcbind. Install rpcbind on Debian and net-nds/rpcbind on Gentoo.

### Build
- clang or gcc (default is gcc)
- make

## Build instructions
1. Install dependencies.
2. git clone https://github.com/arjones239/kademlia.git
3. cd kademlia
4. make

libkademlia.a is now in lib folder. Demo program (kpeer) is in working directory.

## API
- Include kademlia.h header file.
- Before running any kademlia code, call kademlia_init(void)
- To create a peer, run kademlia_peer_create(char *host, unsigned long proto).
- Now you can call the functions
&nbsp; &nbsp; * kademlia_network_bootstrap(char *rhost)
&nbsp; &nbsp; * kademlia_network_store(uuid_t key, char *data, unsigned int dlen)
&nbsp; &nbsp; * kademlia_network_fetch(uuid_t key)
- To cleanup the node run kademlia_node_destroy()
- To cleanup the kademlia environment, run kademlia_destroy()

