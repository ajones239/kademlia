#include "node.h"

#include "serializer.h"

#include "network.h"

#include <netdb.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <uuid/uuid.h>

#define MAX_PORT 65535
#define MIN_PORT 1025
#define QLEN 32

kademlia_node *kademlia_node_create(char *host, int port) {
    kademlia_node *n = malloc(sizeof(kademlia_node));
    uuid_generate_random(n->id);
    n->host = malloc(strlen(host) + 1);
    strcpy(n->host, host);
    n->port = port;
    n->lastSeen = 0;
    time(&(n->lastSeen));
    pthread_t t;
    pthread_create(&t, NULL, (void *)kademlia_node_listen, (void *)n);
    return n;
}

void kademlia_node_destroy(kademlia_node *n) {
    free(n->host);
    // free peers
    free(n);
    return;
}

void kademlia_node_listen(void *t)
{
    kademlia_node *n = (kademlia_node *)t;
    int csock, ssock = passiveTCPWithPort(n->port, QLEN);
    if (ssock == -1) {
        perror("Error creating listening socket");
        return;
    }
    while (1)
    {
        struct sockaddr_in *client = malloc(sizeof(struct sockaddr_in));
        socklen_t size = sizeof(*client);
        if ((csock = accept(ssock, (struct sockaddr *)client, &size)) == -1) {
            perror("Error accepting connection");
            continue;
        }
        client_sock_vars *vars = malloc(sizeof(client_sock_vars));
        vars->n = n;
        vars->client = client;
        vars->sock = csock;
        pthread_t t;
        pthread_attr_t attr;
        pthread_attr_setdetachstate(&attr, 1);
        pthread_create(&t, &attr, (void *)kademlia_deserialize_message, (void *)vars);
    }
}
