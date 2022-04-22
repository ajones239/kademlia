#include "serializer.h"

#include "message.h"
#include "node.h"

#include <errno.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void kademlia_deserialize_message(void *v)
{
    client_sock_vars *vars = (client_sock_vars *)v;
    int r;
    uint16_t type;
    if ((r = read(vars->sock, (void *)&(type), KADEMLIA_MESSAGE_TYPE_S)) == -1) {
        perror("Error reading data from peer socket");
        free(vars->client);
        free(vars);
        return;
    }
    switch (type)
    {
        case PING:
            kademlia_message_recv_ping(vars->client);
            break;
        case STORE:
            kademlia_message_recv_store(vars->client); // + data
            break;
        case FIND_NODE:
            kademlia_message_recv_findnode(vars->client); // + data
            break;
        case FIND_VALUE:
            kademlia_message_recv_findvalue(vars->client); // + data
            break;
        default:
            fprintf(stderr, "Invalid message type, aborting message deserialization\n");
            break;
    }
    free(vars->client);
    free(vars);
}
