#include "kademlia.h"

kademlia_find_node_t *find_node;
kademlia_find_value_t *find_value;
pthread_mutex_t kademlia_rpc_lock;

int RES_CDE_SUC = 0;
int RES_CDE_FLR = 1;

void kademlia_init()
{
    pthread_mutex_init(&kademlia_rpc_lock, NULL);

    find_node = malloc(sizeof(kademlia_find_node_t));
    find_node->ids.ids_val = malloc(sizeof(kademlia_internal_id) * K);
    find_node->hosts.hosts_val = malloc(sizeof(kademlia_internal_host) * K);
    find_node->protos.protos.protos_val = malloc(sizeof(unsigned long) * K);

    find_value = malloc(sizeof(kademlia_find_value_t));
    find_value->ids.ids_val = malloc(sizeof(kademlia_internal_id) * K);
    find_value->hosts.hosts_val = malloc(sizeof(kademlia_internal_host) * K);
    find_value->protos.protos.protos_val = malloc(sizeof(unsigned long) * K);


    for (int i = 0; i < K; i++) {
//        find_node->ids.ids_len = K;
        find_node->ids.ids_val[i].id = malloc(UUID_BYTELEN);
//        find_value->ids.ids_len = K;
        find_value->ids.ids_val[i].id = malloc(UUID_BYTELEN);
        find_node->hosts.hosts_val[i].host = NULL;
        find_value->hosts.hosts_val[i].host = NULL;
    }

    find_value->data.data.data_val = NULL;
}

void kademlia_destroy()
{
    if (find_value->data.data.data_val)
        free(find_value->data.data.data_val);

    for (int i = 0; i < K; i++) {
        if (find_node->hosts.hosts_val[i].host)
            free(find_node->hosts.hosts_val[i].host);
        if (find_value->hosts.hosts_val[i].host)
            free(find_value->hosts.hosts_val[i].host);
        free(find_node->ids.ids_val[i].id);
        free(find_value->ids.ids_val[i].id);
    }

    free(find_value->protos.protos.protos_val);
    free(find_value->hosts.hosts_val);
    free(find_value->ids.ids_val);
    free(find_value);

    free(find_node->protos.protos.protos_val);
    free(find_node->hosts.hosts_val);
    free(find_node->ids.ids_val);
    free(find_node);
} 

void err_exit(char *msg) {
    perror(msg);
    exit(1);
}
