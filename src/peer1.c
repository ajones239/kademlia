#include "kademlia.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char **argv) {
    kademlia_init();
    
    kademlia_node *self;
    self = kademlia_node_create("10.0.1.1", 0);
    printf("%s\n", self->self.host);

    while (1);

    kademlia_node_destroy();
    kademlia_destroy();
}
