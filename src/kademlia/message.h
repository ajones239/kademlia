#include <netdb.h>
#include <stdint.h>

#define KADEMLIA_MAX_MESSAGE_S 1024

#define KADEMLIA_MESSAGE_TYPE_S 2
#define PING 0
#define STORE 1
#define FIND_NODE 2
#define FIND_VALUE 3

void kademlia_message_recv_ping(struct sockaddr_in *client);
void kademlia_message_recv_store(struct sockaddr_in *client); // + data
void kademlia_message_recv_findnode(struct sockaddr_in *client); // + data
void kademlia_message_recv_findvalue(struct sockaddr_in *client); // + data

