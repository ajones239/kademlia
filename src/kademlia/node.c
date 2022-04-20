#include "kademlia.h"

#include <stdlib.h>
#include <time.h>
#include <uuid/uuid.h>

#define MAX_PORT 65535
#define MIN_PORT 1025

kademlia_node *kademlia_node_create(char *host, int port) {
    kademlia_node *n = malloc(sizeof(kademlia_node));
    uuid_generate_random(n->id);
    n->host = host;
    n->port = port;
    time(&(n->lastSeen));
    return n;
}

void kademlia_node_listen(void *n)
{
    kademlia_node *self = (kademlia_node *)n;
    
}
