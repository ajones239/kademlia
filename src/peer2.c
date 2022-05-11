#include "kademlia.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main(int argc, char **argv) {
    kademlia_init();
    
    kademlia_node *self;
    self = kademlia_node_create("172.18.0.3", 0);
    printf("%s\n", self->self.host);

    sleep(2);

    if (kademlia_network_bootstrap("172.18.0.2") != 0) 
        err_exit("error bootstrapping network");

//    uuid_t k;
//    uuid_generate_random(k);
//    char *data = malloc(5);
//    memcpy(data, "test", 4);
//    data[4] = 0;
//    if (kademlia_network_store(k, data, 5) != 0)
//        printf("FAIL\n");

//    uuid_parse("b301a56c-0e48-4ac2-8de6-e52396e94e3c", k);
//    printf("getting data...\n%s\n", kademlia_network_fetch(k));


    sleep(10);

    kademlia_node_destroy();
    kademlia_destroy();
}
