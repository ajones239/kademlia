#include "kademlia.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <string.h>

int main(int argc, char **argv) {
    kademlia_init();
    
    kademlia_node *self;
    self = kademlia_node_create("127.0.0.1", IPPROTO_TCP);
    printf("%s\n", self->self.host);

    sleep(1);




    if (kademlia_network_bootstrap("127.0.0.1") == -1) {
        fprintf(stderr, "failed pinging remote host");
        return -1;
    }

    uuid_t k;
    uuid_generate_random(k);
    if (kademlia_send_store(k, "test", 4, "127.0.0.1") != 0)
        printf("FAIL\n");

    while (1);

    kademlia_node_destroy();
    kademlia_destroy();
}
