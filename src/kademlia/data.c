#include "kademlia.h"

#include <string.h>

void kademlia_data_add(uuid_t k, char *d, unsigned int dlen)
{
    unsigned int l = (dlen <= MAX_DATA_LEN) ? dlen : MAX_DATA_LEN;
    if (sem_wait(&(n->sem)) == -1) err_exit("sem_wait");
    if (sem_wait(&(n->sem)) == -1) err_exit("sem_wait");
    n->data[n->dataCount] = malloc(sizeof(kademlia_data_t));
    uuid_copy((n->data[n->dataCount])->key, k);
    (n->data[n->dataCount])->len = l;
    (n->data[n->dataCount])->data = malloc(l);
    memcpy((n->data[n->dataCount])->data, d, l);
    n->dataCount++;
    if (sem_post(&(n->sem)) == -1) err_exit("sem_post");
    if (sem_post(&(n->sem)) == -1) err_exit("sem_post");
}

int kademlia_data_contains(uuid_t k)
{
    int r = 0;
    if (sem_wait(&(n->sem)) == -1) err_exit("sem_wait");   
    for (int i = 0; i < n->dataCount; i++) {
        if (uuid_compare(k, (n->data[i])->key) == 0) {
            r = 1;
            break;
        } else continue;
    }
    if (sem_post(&(n->sem)) == -1) err_exit("sem_post");
    return r;
}

char *kademlia_data_get(uuid_t k)
{
    char *r = NULL;
    if (sem_wait(&(n->sem)) == -1) err_exit("sem_wait");   
    for (int i = 0; i < n->dataCount; i++) {
        if (uuid_compare(k, (n->data[i])->key) == 0) {
            r = (n->data[i])->data;
            break;
        } else continue;
    }
    if (sem_post(&(n->sem)) == -1) err_exit("sem_post");
    return r;
}
