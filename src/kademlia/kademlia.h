#include <uuid/uuid.h>

typedef struct kademlia_node {
    uuid_t id;    
    char *host;
    int port;
    struct node **peers;
}kademlia_node_t;

kademlia_node_t *kademlia_node_create(char *host, int port);
