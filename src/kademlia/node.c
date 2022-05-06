#include "kademlia.h"
/* #include "node.h" */

/* #include "message.h" */
/* #include "rpc/kademlia_rpc.h" */
/* #include "serializer.h" */


#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <uuid/uuid.h>

kademlia_node *n;

kademlia_node *kademlia_node_create(char *host, unsigned long proto) {
    n = malloc(sizeof(kademlia_node));

    if (sem_init(&(n->sem), 0, 0) == -1) err_exit("Error initializaing semaphore");
    if (sem_post(&(n->sem)) == -1) err_exit("sem_post");
    if (sem_post(&(n->sem)) == -1) err_exit("sem_post");

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
    if (sem_wait(&(n->sem)) == -1) err_exit("sem_wait");
    if (b->count < K) {
        if (sem_wait(&(n->sem)) == -1) err_exit("sem_wait");
        b->peers[b->count] = p;
        b->count++;
        n->peerCount++;
        if (sem_post(&(n->sem)) == -1) err_exit("sem_post");
    }
    if (sem_post(&(n->sem)) == -1) err_exit("sem_post");
}

int kademlia_peer_contains(uuid_t id)
{
    for (int i = 0; i < M; i++) {
        if (sem_wait(&(n->sem)) == -1) err_exit("sem_wait");
        for (int j = 0; j < n->kbuckets[i].count; j++) {
            if (uuid_compare(id, n->kbuckets[i].peers[j]->id) == 0) {
                if (sem_post(&(n->sem)) == -1) err_exit("sem_post");
                return 1;
            }
            else
                continue;
        }
        if (sem_post(&(n->sem)) == -1) err_exit("sem_post");
    }
    return 0;
}

int kademlia_peer_update(uuid_t id)
{
    for (int i = 0; i < M; i++) 
    {
        if (sem_wait(&(n->sem)) == -1) err_exit("sem_wait");
        for (int j = 0; j < n->kbuckets[i].count; j++)
        {
            if (uuid_compare(id, (n->kbuckets[i].peers[j])->id) == 0)
            {
                if (sem_wait(&(n->sem)) == -1) err_exit("sem_wait");
                time(&(n->kbuckets[i].peers[j]->lastSeen));
                if (j == n->kbuckets[i].count - 1) {
                    if (sem_post(&(n->sem)) == -1) err_exit("sem_post");
                    if (sem_post(&(n->sem)) == -1) err_exit("sem_post");
                    return 0;
                }
                kademlia_peer *p;
                p = n->kbuckets[i].peers[j];
                for (int k = j; k < n->kbuckets[i].count - 1; k++)
                    n->kbuckets[i].peers[k] = n->kbuckets[i].peers[k + 1];
                n->kbuckets[i].peers[n->kbuckets[i].count - 1] = p;
                if (sem_post(&(n->sem)) == -1) err_exit("sem_post");
                if (sem_post(&(n->sem)) == -1) err_exit("sem_post");
                return 0;
            }
        }
        if (sem_post(&(n->sem)) == -1) err_exit("sem_post");
    }
    return -1;
}

kademlia_peer *kademlia_peer_next(uuid_t id) {
    int bindex, pindex;
    kademlia_peer *p;
    for (int i = 1; i < M; i++) {
        uuid_t t = {0};
        kademlia_uuid_setbit(t, i);
        if (uuid_compare(id, t) <= 0) {
            bindex = i - 1;
            break;
        } else if (i == M - 1)
            bindex = i;
    }
    if (sem_wait(&(n->sem)) == -1) err_exit("sem_wait");
    kademlia_bucket *b = &(n->kbuckets[bindex]);
    for (int i = 0; i < b->count; i++) {
        if (uuid_compare(id, b->peers[i]->id) == 0)
            pindex = i;
    }
    if (pindex != b->count - 1) {
        p = b->peers[pindex + 1];
    } else
    {
        while (1)
        {
            if (bindex == M - 1)
                bindex = 0;
            else
                bindex++;
            if (n->kbuckets[bindex].count == 0)
                continue;
            else {
                p = n->kbuckets[bindex].peers[0];
                break;
            }
        }
    }
    if (sem_post(&(n->sem)) == -1) err_exit("sem_post");
    return p;
}

kademlia_peer *kademlia_peer_get(uuid_t id)
{
    kademlia_peer *p;
    for (int i = 0; i < M; i++)
    {
        if (sem_wait(&(n->sem)) == -1) err_exit("sem_wait");
        for (int j = 0; j < n->kbuckets[i].count; j++) {
            if (uuid_compare(id, n->kbuckets[i].peers[j]->id) == 0) {
                p = n->kbuckets[i].peers[j];
                if (sem_post(&(n->sem)) == -1) err_exit("sem_post");
                return p;
            } else continue;
        }
        if (sem_post(&(n->sem)) == -1) err_exit("sem_post");
    }
    return NULL;
}
