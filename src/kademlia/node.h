#ifndef KADEMLIA_NODE
#define KADEMLIA_NODE

#include <pthread.h>
#include <semaphore.h>
#include <uuid/uuid.h>
#include "conf.h"
#include "data.h"

typedef struct {
    uuid_t id;    
    char *host;
    char tsp_tcp;
    char tsp_udp;
    time_t lastSeen;
}kademlia_peer;

typedef struct {
    int count;
    kademlia_peer *peers[K];
}kademlia_bucket;

typedef struct kademlia_node_t {
    kademlia_peer self;
    kademlia_bucket kbuckets[M];
    int peerCount;

    int dataCount;
    kademlia_data_t *data[MAX_DATA_COUNT];
    
    sem_t sem;
}kademlia_node;

kademlia_node *kademlia_node_create(char *host, unsigned long proto);

void kademlia_node_destroy();

void kademlia_peer_add(kademlia_peer *p);

int kademlia_peer_contains(uuid_t id);

int kademlia_peer_update(uuid_t id);

kademlia_peer *kademlia_peer_next(uuid_t id);

kademlia_peer *kademlia_peer_get(uuid_t id);

int kademlia_network_bootstrap(char *rhost);

int kademlia_network_store(uuid_t key, char *data, unsigned int dlen);

char *kademlia_network_fetch(uuid_t key);
#endif
