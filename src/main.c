#include "kademlia.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char **argv) {
    kademlia_init();
    
    kademlia_node *self;
    self = kademlia_node_create("127.0.0.1", 0);
    printf("%s\n", self->self.host);
    sleep(1);
    int r = kademlia_send_ping("127.0.0.1");

    printf("%d\n", r);
    sleep(10);

    kademlia_node_destroy();
    kademlia_destroy();
}
