#include <stdlib.h>
#include <uuid/uuid.h>

#include "kademlia.h"

#define MAX_PORT 65535
#define MIN_PORT 1025

int gen_random_port() {
    return (rand() % (MAX_PORT - MIN_PORT + 1)) + MIN_PORT;
}

kademlia_node_t *kademlia_node_create(char *host, int port) {
    kademlia_node_t *n = malloc(sizeof(n));
    uuid_generate(n->id);
    n->host = host;
    n->port = port;
    return n;
}

