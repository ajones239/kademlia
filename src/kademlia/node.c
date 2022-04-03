#include <stdlib.h>
#include <time.h>
#include <uuid/uuid.h>

#include "kademlia.h"

#define MAX_PORT 65535
#define MIN_PORT 1025

kademlia_node_t *kademlia_node_create(char *host, int port) {
    kademlia_node_t *n = malloc(sizeof(n));
    uuid_generate_random(n->id);
    n->host = host;
    n->port = port;
    time(&(n->lastSeen));
    return n;
}

