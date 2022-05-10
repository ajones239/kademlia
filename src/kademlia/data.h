#ifndef KADEMLIA_DATA
#define KADEMLIA_DATA

#include <uuid/uuid.h>

typedef struct {
    uuid_t key;
    unsigned int len;
    char *data;
}kademlia_data_t;

void kademlia_data_add(uuid_t k, char *d, unsigned int dlen);

int kademlia_data_contains(uuid_t k);

char *kademlia_data_get(uuid_t k);

#endif
