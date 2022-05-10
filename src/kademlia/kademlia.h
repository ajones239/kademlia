#include "data.h"
#include "node.h"
#include "kademlia_rpc.h"
#include "conf.h"

#define UUID_BYTELEN 16

extern kademlia_node *n;

void kademlia_init();
void kademlia_destroy();

void err_exit(char *msg);
