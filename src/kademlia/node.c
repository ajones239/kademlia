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

#define UUID_BYTELEN 16

kademlia_node *n;

kademlia_node *kademlia_node_create(char *host, unsigned long proto) {
    n = malloc(sizeof(kademlia_node));
    uuid_generate_random(n->self.id);

    n->self.host = malloc(strlen(host) + 1);
    strcpy(n->self.host, host);
    n->self.host[strlen(host)] = 0;
    switch (proto)
    {
        case IPPROTO_TCP:
            n->self.tsp_tcp = 1;
            break;
        case IPPROTO_UDP:
            n->self.tsp_udp = 1;
            break;
        default:
            n->self.tsp_tcp = 1;           
            n->self.tsp_udp = 1;           
            break;
    }

    time(&(n->self.lastSeen));
    n->peerCount = 0;
    kademlia_peer_add(&(n->self));

    pthread_t t;
    pthread_create(&t, NULL, (void *)kademlia_svc_run, (void *)n);

    return n;
}

void kademlia_node_destroy() {
    for (int i = 0; i < M; i++) {
        for (int j = 0; j < (n->kbuckets[i]).count; j++) {
            free((n->kbuckets[i]).peers[j]->host);
            free((n->kbuckets[i]).peers[j]);
        }
    }
    free(n->self.host);
    free(n);
    return;
}

void kademlia_uuid_setbit(uuid_t id, int i) {
    char x = 0x01;
    int index = i / CHAR_BIT;
    int shifts = CHAR_BIT - (i % CHAR_BIT) - 1;
    for (int j = 0; j < shifts; j++)
        x = x << 1;
    ((char *)id)[index] |= x;
}

void kademlia_distance(uuid_t n1, uuid_t n2, uuid_t ret) {
    for (int i = 0; i < UUID_BYTELEN; i++)
        ((char *)ret)[i] =  ((char *)n1)[i] ^ ((char *)n2)[i];
}

void kademlia_peer_add(kademlia_peer *p)
{
    uuid_t distance;
    kademlia_distance(p->id, n->self.id, distance);
    int index;
    for (int i = 1; i < M; i++) {
        uuid_t t = {0};
        kademlia_uuid_setbit(t, i);
        if (uuid_compare(distance, t) <= 0) {              // 128 kbuckets, indexed
            index = i - 1;                                 // from 0. bucket i holds
            break;                                         // peers from 2^i to 2^{i+1}
        } else if (i == M - 1)
            index = i;
    }
    kademlia_bucket *b = &(n->kbuckets[index]);
    b->peers[b->count] = p;
    b->count++;
}

int kademlia_peer_contains(uuid_t id) {
    for (int i = 0; i < M; i++) {
        for (int j = 0; j < n->kbuckets[i].count; j++) {
            if (uuid_compare(id, n->kbuckets[i].peers[j]->id) == 0)
                return 1;
            else
                continue;
        }
    }
    return 0;
}

void kademlia_peer_update(uuid_t id) {
    for (int i = 0; i < M; i++) 
    {
        for (int j = 0; j < n->kbuckets[i].count; j++)
        {
            if (uuid_compare(id, (n->kbuckets[i].peers[j])->id) == 0)
            {
                time(&(n->kbuckets[i].peers[j]->lastSeen));
                if (j == n->kbuckets[i].count - 1)
                    return;
                else {
                    kademlia_peer *p;
                    p = n->kbuckets[i].peers[j];
                    for (int k = j; k < n->kbuckets[i].count - 1; k++)
                        n->kbuckets[i].peers[k] = n->kbuckets[i].peers[k + 1];
                    n->kbuckets[i].peers[n->kbuckets[i].count - 1] = p;
                }
                return;
            }
        }
    }
}
