#include "kademlia.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char **argv) {
    kademlia_node *self;
    self = kademlia_node_create("127.0.0.1", 5555);
    printf("%s\t%d\n", self->host, self->port);
    sleep(10);
    free(self);
}
