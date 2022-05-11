#include "kademlia.h"
#include <memory.h>
#include <semaphore.h>

/* Default timeout can be changed using clnt_control() */
static struct timeval TIMEOUT = { 25, 0 };

int kademlia_send_ping(char *rhost)
{
    kademlia_ping_t var;
    char *tspStr;

    if (sem_wait(&(n->sem)) == -1) err_exit("sem_wait");
    var.id = n->self.id;
    var.host = malloc(strlen(n->self.host) + 1);
    strcpy(var.host, n->self.host);
    if (n->self.tsp_tcp) {
        var.proto = IPPROTO_TCP;
        tspStr = "tcp";
    } else {
        var.proto = IPPROTO_UDP;
        tspStr = "udp";
    }
    if (sem_post(&(n->sem)) == -1) err_exit("sem_post");

    CLIENT *clnt;
    clnt = clnt_create(rhost, MESSAGE_PROG, MESSAGE_VERS, tspStr);
    if (clnt == NULL) {
        fprintf(stderr, "error creating rpc client\n");
        return -1;
    }
    int *r = kademlia_ping_1(&var, clnt);
    free(var.host);
    free(clnt);

    return *r;
}

int *kademlia_ping_1(kademlia_ping_t *argp, CLIENT *clnt)
{
	static int clnt_res;
	memset((char *)&clnt_res, 0, sizeof(clnt_res));
	if (clnt_call (clnt, kademlia_ping,
            (xdrproc_t) xdr_kademlia_ping_t, (caddr_t) argp,
            (xdrproc_t) xdr_int, (caddr_t) &clnt_res,
            TIMEOUT) != RPC_SUCCESS) {
		return (NULL);
	}
	return (&clnt_res);
}

int kademlia_send_store(uuid_t k, char *d, unsigned int dlen, char *rhost)
{
    kademlia_store_t st;
    st.key = malloc(UUID_BYTELEN);
    uuid_copy(st.key, k);
    st.data.data.data_len = dlen;
    st.data.data.data_val = malloc(dlen);
    memcpy(st.data.data.data_val, d, dlen);

    char *tspStr;
    if (sem_wait(&(n->sem)) == -1) err_exit("sem_wait");
    if (n->self.tsp_tcp)
        tspStr = "tcp";
    else
        tspStr = "udp";
    if (sem_post(&(n->sem)) == -1) err_exit("sem_post");

    CLIENT *clnt;
    clnt = clnt_create(rhost, MESSAGE_PROG, MESSAGE_VERS, tspStr);
    if (clnt == NULL) {
        fprintf(stderr, "error creating rpc client\n");
        return -1;
    }
    int *r = kademlia_store_1(&st, clnt);
    free(st.key);
    free(st.data.data.data_val);
    free(clnt);

    return *r;
}

int *kademlia_store_1(kademlia_store_t *argp, CLIENT *clnt)
{
	static int clnt_res;
	memset((char *)&clnt_res, 0, sizeof(clnt_res));
	if (clnt_call (clnt, kademlia_store,
            (xdrproc_t) xdr_kademlia_store_t, (caddr_t) argp,
            (xdrproc_t) xdr_int, (caddr_t) &clnt_res,
            TIMEOUT) != RPC_SUCCESS) {
		return (NULL);
	}
	return (&clnt_res);
}

int kademlia_send_find_node(uuid_t id, char *rhost)
{
    unsigned char *idcpy = malloc(sizeof(uuid_t));
    memcpy(idcpy, id, sizeof(uuid_t));
    
    CLIENT *clnt;
    char *tspStr;
    if (sem_wait(&(n->sem)) == -1) err_exit("sem_wait");
    if (n->self.tsp_tcp)
        tspStr = "tcp";
    else
        tspStr = "udp";
    if (sem_post(&(n->sem)) == -1) err_exit("sem_post");
    clnt = clnt_create(rhost, MESSAGE_PROG, MESSAGE_VERS, tspStr);
    if (clnt == NULL) {
        fprintf(stderr, "error creating rpc client\n");
        return -1;
    }

    kademlia_find_node_t *t = kademlia_find_node_1(&idcpy, clnt);
    for (int i = 0; i < t->numNodes; i++)
    {
        kademlia_peer *p = malloc(sizeof(kademlia_peer));
        memcpy(p->id, t->ids.ids_val[i].id, sizeof(uuid_t));
        p->host = malloc(strlen(t->hosts.hosts_val[i].host) + 1);
        strcpy(p->host, t->hosts.hosts_val[i].host);
        p->tsp_tcp = (t->protos.protos.protos_val[i] == IPPROTO_TCP) ? 1 : 0;
        p->tsp_udp = (t->protos.protos.protos_val[i] == IPPROTO_UDP) ? 1 : 0;
        time(&(p->lastSeen));
        kademlia_peer_add(p);
    }
    free(clnt);
    free(idcpy);
    return 0;
}

kademlia_find_node_t *kademlia_find_node_1(kademlia_id_t *argp, CLIENT *clnt)
{
	static kademlia_find_node_t clnt_res;
	memset((char *)&clnt_res, 0, sizeof(clnt_res));
	if (clnt_call (clnt, kademlia_find_node,
            (xdrproc_t) xdr_kademlia_id_t, (caddr_t) argp,
            (xdrproc_t) xdr_kademlia_find_node_t, (caddr_t) &clnt_res,
            TIMEOUT) != RPC_SUCCESS) {
		return (NULL);
	}
	return (&clnt_res);
}

kademlia_data_t *kademlia_send_find_value(kademlia_id_t *k, char *rhost) {
    unsigned char *idcpy = malloc(sizeof(uuid_t));
    memcpy(idcpy, k, sizeof(uuid_t));
    kademlia_data_t *r = NULL;
    
    CLIENT *clnt;
    char *tspStr;
    if (sem_wait(&(n->sem)) == -1) err_exit("sem_wait");
    if (n->self.tsp_tcp)
        tspStr = "tcp";
    else
        tspStr = "udp";
    if (sem_post(&(n->sem)) == -1) err_exit("sem_post");
    clnt = clnt_create(rhost, MESSAGE_PROG, MESSAGE_VERS, tspStr);
    if (clnt == NULL) {
        fprintf(stderr, "error creating rpc client\n");
        return NULL;
    }

    kademlia_find_value_t *t = kademlia_find_value_1(&idcpy, clnt);
    if (t->numNodes == 0) {
        r = malloc(sizeof(kademlia_data_t));
        uuid_copy(r->key, *k);
        r->len = t->data.data.data_len;
        memcpy(r->data, t->data.data.data_val, r->len);
    } else
    {
        for (int i = 0; i < t->numNodes; i++)
        {
            kademlia_peer *p = malloc(sizeof(kademlia_peer));
            memcpy(p->id, t->ids.ids_val[i].id, sizeof(uuid_t));
            p->host = malloc(strlen(t->hosts.hosts_val[i].host) + 1);
            strcpy(p->host, t->hosts.hosts_val[i].host);
            p->tsp_tcp = (t->protos.protos.protos_val[i] == IPPROTO_TCP) ? 1 : 0;
            p->tsp_udp = (t->protos.protos.protos_val[i] == IPPROTO_UDP) ? 1 : 0;
            time(&(p->lastSeen));
            kademlia_peer_add(p);
        }
    } 
    free(clnt);
    free(idcpy);
    return r;
}

kademlia_find_value_t *kademlia_find_value_1(kademlia_id_t *argp, CLIENT *clnt)
{
	static kademlia_find_value_t clnt_res;
	memset((char *)&clnt_res, 0, sizeof(clnt_res));
	if (clnt_call (clnt, kademlia_find_value,
            (xdrproc_t) xdr_kademlia_id_t, (caddr_t) argp,
            (xdrproc_t) xdr_kademlia_find_value_t, (caddr_t) &clnt_res,
            TIMEOUT) != RPC_SUCCESS) {
		return (NULL);
	}
	return (&clnt_res);
}
