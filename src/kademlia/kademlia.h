#include <time.h>
#include <uuid/uuid.h>

typedef struct kademlia_node {
    uuid_t id;    
    char *host;
    int port;
    time_t lastSeen;
    struct node **peers;
}kademlia_node_t;

enum kademlia_message_t{PING, STORE, FIND_NODE, FIND_VALUE};

kademlia_node_t *kademlia_node_create(char *host, int port);
