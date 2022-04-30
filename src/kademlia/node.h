#include <uuid/uuid.h>

#define KADEMLIA_DEFAULT_MAX_PEERS 128

#define MAX_ADDR_LEN 62

typedef struct {
    char *host;
    char tsp_tcp;
    char tsp_udp;
}kademlia_address;

typedef struct kademlia_node_t {
    uuid_t id;    

    kademlia_address addr;

    time_t lastSeen;

    int peerCount;
    int maxPeerCount;
    struct kademlia_node_t **peers;
}kademlia_node;

typedef struct {
    kademlia_node *n;
    char *data;
    struct sockaddr_in *client;
    int sock;
    int proto;
}client_sock_vars;

kademlia_node *kademlia_node_create(char *host, unsigned long proto);

void kademlia_node_destroy();

void kademlia_node_listen(void *t);

void kademlia_peer_add(kademlia_node *p);

void kademlia_peer_sort();
