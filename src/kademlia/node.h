#include <uuid/uuid.h>

#define KADEMLIA_PROTO_TYPE_S 1
#define KADEMLIA_PROTO_TCP 0
#define KADEMLIA_PROTO_UDP 1

#define KADEMLIA_DEFAULT_MAX_PEERS 128

typedef struct kademlia_node_t {
    uuid_t id;    

    char *host;
    int port;
    int proto;

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

kademlia_node *kademlia_node_create(char *host, int port, int proto);

void kademlia_node_destroy(kademlia_node *n);

void kademlia_node_listen(void *t);

void kademlia_peer_add(kademlia_node *n, kademlia_node *p);

void kademlia_peer_sort(kademlia_node *n);
