#include "kademlia.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char **argv) {
    kademlia_init();
    kademlia_node *self;
    self = kademlia_node_create("127.0.0.1", IPPROTO_TCP);
    printf("%s\n", self->addr.host);
    sleep(10);
    free(self);
}
