#include "serializer.h"

#include "message.h"
#include "node.h"

#include <errno.h>
#include <netdb.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <unistd.h>

void kademlia_deserialize_message(void *v)
{
    client_sock_vars *vars = (client_sock_vars *)v;
    uint16_t type = ntohs(*((uint16_t *)&(vars->data[0])));
    uuid_t id;
    if (uuid_parse(&(vars->data[2]), id) == -1) {
        fprintf(stderr, "Error invalid UUID, aborting message deserialization\n");
        return;
    }

    switch (type)
    {
        case PING:
            sendto(vars->sock, &type, KADEMLIA_MESSAGE_TYPE_S, 0, (struct sockaddr *)vars->client, sizeof(vars->client));
            if (vars->proto == KADEMLIA_PROTO_TCP)
                close(vars->sock);
            printf("got ping\n");
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
    free(vars->data);
    free(vars);
}
