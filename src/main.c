#include "kademlia.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char **argv) {
    kademlia_node *self;
    self = kademlia_node_create("127.0.0.1", IPPROTO_TCP);
    printf("%s\n", self->addr.host);

    kademlia_ping_t var;
    var.id = self->id;
    var.host = self->addr.host;
    var.proto = IPPROTO_TCP;
    int *r;
    CLIENT *clnt;
    clnt = clnt_create(self->addr.host, MESSAGE_PROG, MESSAGE_VERS, "tcp");
    if (clnt == NULL) {
        perror("Error creating rpc client\n");
        return 1;
    }

    r = kademlia_ping_1(&var, clnt);
    printf("%d\n", *r);
    sleep(10);
    free(self);
}
