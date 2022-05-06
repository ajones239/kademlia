#include "kademlia.h"

kademlia_find_node_t *find_node;
kademlia_find_value_t *find_value;
pthread_mutex_t kademlia_rpc_lock;

void kademlia_init()
{
    pthread_mutex_init(&kademlia_rpc_lock, NULL);

    find_node = malloc(sizeof(kademlia_find_node_t));
    find_node->ids = malloc(sizeof(kademlia_internal_id) * K);
    find_node->hosts = malloc(sizeof(kademlia_internal_host) * K);
    find_node->protos.protos_val = malloc(sizeof(unsigned long) * K);

    find_value = malloc(sizeof(kademlia_find_value_t));
    find_value->ids = malloc(sizeof(kademlia_internal_id) * K);
    find_value->hosts = malloc(sizeof(kademlia_internal_host) * K);
    find_value->protos.protos_val = malloc(sizeof(unsigned long) * K);


    for (int i = 0; i < K; i++) {
        find_node->ids[i].ids_len = UUID_BYTELEN;
        find_node->ids[i].ids_val = malloc(UUID_BYTELEN);
        find_value->ids[i].ids_len = UUID_BYTELEN;
        find_value->ids[i].ids_val = malloc(UUID_BYTELEN);
        find_node->hosts[i].hosts_val = NULL;
        find_value->hosts[i].hosts_val = NULL;
    }

    find_value->data.data_val = NULL;
}

void kademlia_destroy()
{
    if (find_value->data.data_val)
        free(find_value->data.data_val);

    for (int i = 0; i < K; i++) {
        if (find_node->hosts[i].hosts_val)
            free(find_node->hosts[i].hosts_val);
        if (find_value->hosts[i].hosts_val)
            free(find_value->hosts[i].hosts_val);
        free(find_node->ids[i].ids_val);
        free(find_value->ids[i].ids_val);
    }

    free(find_value->protos.protos_val);
    free(find_value->hosts);
    free(find_value->ids);
    free(find_value);

    free(find_node->protos.protos_val);
    free(find_node->hosts);
    free(find_node->ids);
    free(find_node);
} 

void err_exit(char *msg) {
    perror(msg);
    exit(1);
}
