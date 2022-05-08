#include "kademlia.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char **argv) {
    kademlia_init();
    
    kademlia_node *self;
    self = kademlia_node_create("172.18.0.3", 0);
    printf("%s\n", self->self.host);
    if (kademlia_network_bootstrap("172.18.0.2") != 0) 
        err_exit("error bootstrapping network");

    sleep(10);

    kademlia_node_destroy();
    kademlia_destroy();
}
