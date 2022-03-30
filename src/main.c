#include <stdio.h>

#include "kademlia.h"

int main(int argc, char **argv) {
    kademlia_node_t *self;
    self = kademlia_node_create("127.0.0.1", 5555);
    printf("%s\t%d\n", self->host, self->port);
}