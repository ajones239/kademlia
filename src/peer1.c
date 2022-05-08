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


    kademlia_ping_t var;

    var.id = self->self.id;
    var.host = self->self.host;
    var.proto = IPPROTO_TCP;

    CLIENT *clnt;
    clnt = clnt_create("127.0.0.1", MESSAGE_PROG, MESSAGE_VERS, "tcp");
    if (clnt == NULL) {
        fprintf(stderr, "error creating rpc client\n");
        return 1;
    }
    int *r;
    r = kademlia_ping_1(&var, clnt);
    if (r == NULL) {
        fprintf(stderr, "error pinging");
        return 1;
    } else printf("%d\n", *r);
    free(var.host);
    free(clnt);







    if (kademlia_send_ping("127.0.0.1") == -1) {
        fprintf(stderr, "failed pinging remote host");
        return -1;
    }
    if (kademlia_send_find_node(self->self.id, "127.0.0.1") == -1) {
        fprintf(stderr, "failed getting peers from remote host");
        return -1;
    }

    while (1);

    kademlia_node_destroy();
    kademlia_destroy();
}
