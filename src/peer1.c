#include "kademlia.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <string.h>

int main(int argc, char **argv) {
    kademlia_init();
    
    kademlia_node *self;
    self = kademlia_node_create("172.18.0.2", IPPROTO_TCP);
    printf("%s\n", self->self.host);

    sleep(4);

//    if (kademlia_network_bootstrap("127.0.0.1") == -1) {
//        fprintf(stderr, "failed pinging remote host");
//        return -1;
//    }

    uuid_t k;
    uuid_generate_random(k);

    char *data = malloc(5);
    memcpy(data, "test", 5);
    data[4] = 0;

    printf("storing data...\n");
    if (kademlia_network_store(k, data, 5) != 0)
        printf("FAIL\n");
    free(data);

    char guid[UUID_STR_LEN];
    uuid_unparse(k, guid);
    printf("%s\n", guid);

//    printf("getting data...\n%s\n", kademlia_network_fetch(k));

    while (1) {
        printf("%d\n", n->peerCount);
        sleep(3);
    }

    kademlia_node_destroy();
    kademlia_destroy();
}
