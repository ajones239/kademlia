#include "message.h"

#include <netdb.h>

void kademlia_message_recv_ping(struct sockaddr_in *client) {
}

void kademlia_message_recv_store(struct sockaddr_in *client) {} // + data

void kademlia_message_recv_findnode(struct sockaddr_in *client) {} // + data

void kademlia_message_recv_findvalue(struct sockaddr_in *client) {} // + data
