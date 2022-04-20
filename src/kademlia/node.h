#include <uuid/uuid.h>
typedef struct {
    uuid_t id;    
    char *host;
    int port;
    time_t lastSeen;
    struct node **peers;
}kademlia_node;

typedef struct {
    kademlia_node *n;
    struct sockaddr_in *client;
    int sock;
}client_sock_vars;

kademlia_node *kademlia_node_create(char *host, int port);

void kademlia_node_destroy(kademlia_node *n);

void kademlia_node_listen(void *t);
