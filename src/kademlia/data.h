#include <uuid/uuid.h>

void kademlia_data_add(uuid_t k, char *d, unsigned int dlen);

int kademlia_data_contains(uuid_t k);

char *kademlia_data_get(uuid_t k);
