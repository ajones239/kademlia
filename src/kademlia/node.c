#include "kademlia.h"

#include <netdb.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <uuid/uuid.h>

kademlia_node *n;

kademlia_node *kademlia_node_create(char *host, unsigned long proto) {
    n = malloc(sizeof(kademlia_node));

    if (sem_init(&(n->sem), 0, 2) == -1) err_exit("error initializaing semaphore");

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

    n->dataCount = 0;
    for (int i = 0; i < MAX_DATA_COUNT; i++)
        n->data[i] = NULL;

    return n;
}

void kademlia_node_destroy()
{
    for (int i = 0; i < M; i++) {
        for (int j = 0; j < (n->kbuckets[i]).count; j++) {
            free((n->kbuckets[i]).peers[j]->host);
            free((n->kbuckets[i]).peers[j]);
        }
    }
    free(n->self.host);

    for (int i = 0; i < MAX_DATA_COUNT; i++) {
        if (n->data[i] != NULL) {
            free((n->data[i])->data);
            free(n->data[i]);
        }
    }

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
    if (kademlia_peer_contains(p->id)) return;
    uuid_t distance;
    kademlia_distance(p->id, n->self.id, distance);
    int index;
    for (int i = 1; i < M; i++) {
        uuid_t t;
        memset(t, 0, sizeof(uuid_t));
        kademlia_uuid_setbit(t, i);
        if (uuid_compare(t, distance) <= 0) {              // 128 kbuckets, indexed
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
    for (int i = 0; i < M; i++)
    {
        if (sem_wait(&(n->sem)) == -1) err_exit("sem_wait");
        for (int j = 0; j < n->kbuckets[i].count; j++)
        {
            if (uuid_compare(id, n->kbuckets[i].peers[j]->id) == 0) {
                if (sem_post(&(n->sem)) == -1) err_exit("sem_post");
                return 1;
            } else continue;
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

kademlia_peer *kademlia_peer_next(uuid_t id)
{
    int bindex = -1, pindex = -1;
    if (kademlia_peer_contains(id))
    {
        for (int i = 0; i < M; i++)
        {
            for (int j = 0; j < n->kbuckets[i].count; j++) {
                if (uuid_compare(id, (n->kbuckets[i].peers[j])->id) == 0) {
                    bindex = i;
                    pindex = j;
                    break;
                }
            }
            if (bindex != -1)
                break;
        }
    } else
    {
        pindex = 0;
        for (int i = 1; i < M; i++)
        {
            uuid_t t;
            memset(t, 0, sizeof(uuid_t));
            kademlia_uuid_setbit(t, i);
            if (uuid_compare(t, id) <= 0) {
                bindex = i - 1;
                break;
            } else if (i == M - 1)
                bindex = i;
        }
    }
    char done = 0;
    kademlia_peer *p;
    while (!done)
    {
        if (sem_wait(&(n->sem)) == -1) err_exit("sem_wait");
        kademlia_bucket *b = &(n->kbuckets[bindex]);
        if (b->count == 0) {
            if (bindex == M - 1)
                bindex = 0;
            else
                bindex++;
        } else {
            done = 1;
            p = n->kbuckets[bindex].peers[pindex];
        }
        if (sem_post(&(n->sem)) == -1) err_exit("sem_post");
    }

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

int kademlia_network_bootstrap(char *rhost) {
    if (kademlia_send_find_node(n->self.id, rhost) == -1) {
        fprintf(stderr, "failed getting peers from remote host");
        return -1;
    }
    if (kademlia_send_ping(rhost) == -1) {
        fprintf(stderr, "failed pinging remote host");
        return -1;
    }
    return 0;
}

int kademlia_network_store(uuid_t key, char *data, unsigned int dlen) {
    kademlia_peer *p;
    if (kademlia_peer_contains(key))
        p = kademlia_peer_get(key);
    else
        p = kademlia_peer_next(key);
    return kademlia_send_store(key, data, dlen, p->host);
}

char *kademlia_network_fetch(uuid_t key) {
    if (kademlia_data_contains(key))
        return kademlia_data_get(key)->data;
    kademlia_peer *p = NULL;
    for (int i = 0; i < M; i++) {
        for (int j = 0; j < n->kbuckets[i].count; j++) {
            p = n->kbuckets[i].peers[j];
            kademlia_data_t *dt = kademlia_send_find_value(&key, p->host);
            if (dt != NULL)
                return dt->data;
        }
    }
    return NULL;
}

