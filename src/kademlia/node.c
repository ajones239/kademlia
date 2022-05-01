#include "kademlia.h"
/* #include "node.h" */

/* #include "message.h" */
/* #include "rpc/kademlia_rpc.h" */
/* #include "serializer.h" */


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

kademlia_node *n;

kademlia_node *kademlia_node_create(char *host, unsigned long proto) {
    n = malloc(sizeof(kademlia_node));

    uuid_generate_random(n->id);

    n->addr.host = malloc(strlen(host) + 1);
    strcpy(n->addr.host, host);
    switch (proto)
    {
        case IPPROTO_TCP:
            n->addr.tsp_tcp = 1;
            break;
        case IPPROTO_UDP:
            n->addr.tsp_udp = 1;
            break;
        default:
            n->addr.tsp_tcp = 1;           
            n->addr.tsp_udp = 1;           
            break;
    }

    time(&(n->lastSeen));

    n->peerCount = 0;
    n->maxPeerCount = KADEMLIA_DEFAULT_MAX_PEERS;
    n->peers = calloc(KADEMLIA_DEFAULT_MAX_PEERS, sizeof(kademlia_node *));

    kademlia_peer_add(n);

    pthread_t t;
    pthread_create(&t, NULL, (void *)kademlia_svc_run, (void *)n);

    return n;
}

void kademlia_node_destroy() {
    free(n->addr.host);
    free(n->peers);
    free(n);
    return;
}

void kademlia_peer_add(kademlia_node *p) {
    if (n->peerCount >= n->maxPeerCount) {
        kademlia_node **newPeers;
        n->maxPeerCount *= 2;
        newPeers = calloc(n->maxPeerCount, sizeof(kademlia_node *));
        for (int i = 0; i < n->peerCount; i++)
            newPeers[i] = n->peers[i];
        free(n->peers);
        n->peers = newPeers;
    }
    n->peers[n->peerCount] = p;
    n->peerCount++;
    kademlia_peer_sort(n);
}

void kademlia_peer_sort() {
    int j, i = 1;
    kademlia_node *x;
    while (i < n->peerCount)
    {
        x = n->peers[i];
        j = i - 1;
        while (j >= 0 && (uuid_compare((n->peers[j])->id, x->id) > 0)) { 
            n->peers[j + 1] = n->peers[j];
            j--;
        }
        n->peers[j + 1] = x;
        i++;
    }
}

int kademlia_peer_contains(uuid_t id) {
    for (int i = 0; i < n->peerCount; i++) {
        if (uuid_compare(id, (n->peers[i])->id) == 0)
            return 1;
    }
    return 0;
}
