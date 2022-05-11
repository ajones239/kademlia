#include "kademlia.h"
#include <stdio.h>
#include <stdlib.h>
#include <rpc/pmap_clnt.h>
#include <string.h>
#include <memory.h>
#include <sys/socket.h>
#include <netinet/in.h>

#ifndef SIG_PF
#define SIG_PF void(*)(int)
#endif

static void message_prog_1(struct svc_req *rqstp, register SVCXPRT *transp)
{
	union {
		kademlia_ping_t kademlia_ping_1_arg;
		kademlia_store_t kademlia_store_1_arg;
		kademlia_id_t kademlia_find_node_1_arg;
		kademlia_id_t kademlia_find_value_1_arg;
	} argument;
	char *result;
	xdrproc_t _xdr_argument, _xdr_result;
	char *(*local)(char *, struct svc_req *);

	switch (rqstp->rq_proc)
    {
        case NULLPROC:
            (void) svc_sendreply (transp, (xdrproc_t) xdr_void, (char *)NULL);
            return;

        case kademlia_ping:
            _xdr_argument = (xdrproc_t) xdr_kademlia_ping_t;
            _xdr_result = (xdrproc_t) xdr_int;
            local = (char *(*)(char *, struct svc_req *)) kademlia_ping_1_svc;
            break;

        case kademlia_store:
            _xdr_argument = (xdrproc_t) xdr_kademlia_store_t;
            _xdr_result = (xdrproc_t) xdr_int;
            local = (char *(*)(char *, struct svc_req *)) kademlia_store_1_svc;
            break;

        case kademlia_find_node:
            _xdr_argument = (xdrproc_t) xdr_kademlia_id_t;
            _xdr_result = (xdrproc_t) xdr_kademlia_find_node_t;
            local = (char *(*)(char *, struct svc_req *)) kademlia_find_node_1_svc;
            break;

        case kademlia_find_value:
            _xdr_argument = (xdrproc_t) xdr_kademlia_id_t;
            _xdr_result = (xdrproc_t) xdr_kademlia_find_value_t;
            local = (char *(*)(char *, struct svc_req *)) kademlia_find_value_1_svc;
            break;

        default:
            svcerr_noproc (transp);
            return;
	}
	memset ((char *)&argument, 0, sizeof (argument));
	if (!svc_getargs (transp, (xdrproc_t) _xdr_argument, (caddr_t) &argument)) {
		svcerr_decode (transp);
		return;
	}
	result = (*local)((char *)&argument, rqstp);
	if (result != NULL && !svc_sendreply(transp, (xdrproc_t) _xdr_result, result)) {
		svcerr_systemerr (transp);
	}
	if (!svc_freeargs (transp, (xdrproc_t) _xdr_argument, (caddr_t) &argument)) {
		fprintf (stderr, "%s", "unable to free arguments");
		exit (1);
	}
	return;
}

void kademlia_svc_run (void *t)
{
    register SVCXPRT *transp;
    pmap_unset (MESSAGE_PROG, MESSAGE_VERS);
    if (n->self.tsp_udp)
    {
        transp = svcudp_create(RPC_ANYSOCK);
        if (transp == NULL) {
            fprintf (stderr, "%s", "cannot create udp service.");
            exit(1);
        }
        if (!svc_register(transp, MESSAGE_PROG, MESSAGE_VERS, message_prog_1, IPPROTO_UDP)) {
            fprintf (stderr, "%s", "unable to register (MESSAGE_PROG, MESSAGE_VERS, udp).");
            exit(1);
        }
    }
    if (n->self.tsp_tcp)
    {
        transp = svctcp_create(RPC_ANYSOCK, 0, 0);
        if (transp == NULL) {
            fprintf (stderr, "%s", "cannot create tcp service.");
            exit(1);
        }
        if (!svc_register(transp, MESSAGE_PROG, MESSAGE_VERS, message_prog_1, IPPROTO_TCP)) {
            fprintf (stderr, "%s", "unable to register (MESSAGE_PROG, MESSAGE_VERS, udp).");
            exit(1);
        }
    }
    svc_run();
}

int *kademlia_ping_1_svc(kademlia_ping_t *pt, struct svc_req *req) {
    if (kademlia_peer_update(pt->id) == -1)
    {
        kademlia_peer *new = malloc(sizeof(kademlia_peer));
        memcpy(new->id, pt->id, sizeof(uuid_t));
        size_t hsize = strlen(pt->host) + 1;
        new->host = malloc(hsize);
        snprintf(new->host, hsize, "%s", pt->host);
        if (pt->proto == IPPROTO_TCP) {
            new->tsp_tcp = 1;
            new->tsp_udp = 0;
        } else if (pt->proto == IPPROTO_UDP) {
            new->tsp_udp = 1;
            new->tsp_tcp = 0;
        }
        time(&(new->lastSeen));
        kademlia_peer_add(new);
    }
    return &RES_CDE_SUC;
}

int *kademlia_store_1_svc(kademlia_store_t *st, struct svc_req *req) {
    kademlia_data_add(st->key, st->data.data.data_val, st->data.data.data_len);
    return &RES_CDE_SUC;
}

void kademlia_find_node_add(kademlia_peer *p)
{
    pthread_mutex_lock(&kademlia_rpc_lock);
    memcpy(find_node->ids.ids_val[find_node->numNodes].id, p->id, sizeof(uuid_t));
    find_node->ids.ids_len++;

    if (find_node->hosts.hosts_val[find_node->numNodes].host)
        free(find_node->hosts.hosts_val[find_node->numNodes].host);
    find_node->hosts.hosts_val[find_node->numNodes].host = malloc(strlen(p->host) + 1);
    find_node->hosts.hosts_len++;
    strcpy(find_node->hosts.hosts_val[find_node->numNodes].host, p->host);
    
    find_node->protos.protos.protos_val[find_node->numNodes] = (p->tsp_tcp) ? IPPROTO_TCP : IPPROTO_UDP;
    find_node->numNodes++;
    pthread_mutex_unlock(&kademlia_rpc_lock);
}

kademlia_find_node_t *kademlia_find_node_1_svc(kademlia_id_t *id, struct svc_req *req)
{
    int nn;
    kademlia_peer *p;
    find_node->numNodes = 0;
    if (sem_wait(&(n->sem)) == -1) err_exit("sem_wait");
    if (n->peerCount < K) {
        nn = n->peerCount;
        for (int i = 0; i < M; i++) {
            for (int j = 0; j < n->kbuckets[i].count; j++) {
                p = n->kbuckets[i].peers[j];
                kademlia_find_node_add(p);
            }
        }
    } else {
        nn = K;
        if (kademlia_peer_contains(*id)) {
            p = kademlia_peer_get(*id);
            kademlia_find_node_add(p); 
        }
        while (find_node->numNodes < K) {
            p = kademlia_peer_next(p->id);
            kademlia_find_node_add(p);            
        }
    }
    if (sem_post(&(n->sem)) == -1) err_exit("sem_post");
    pthread_mutex_lock(&kademlia_rpc_lock);
    find_node->protos.protos.protos_len = nn;
    pthread_mutex_unlock(&kademlia_rpc_lock);
    return find_node;
}

void kademlia_find_value_add(kademlia_peer *p)
{
    pthread_mutex_lock(&kademlia_rpc_lock);
    memcpy(find_value->ids.ids_val[find_value->numNodes].id, p->id, sizeof(uuid_t));
    find_value->ids.ids_len++;

    if (find_value->hosts.hosts_val[find_value->numNodes].host)
        free(find_value->hosts.hosts_val[find_value->numNodes].host);
    find_value->hosts.hosts_val[find_value->numNodes].host = malloc(strlen(p->host) + 1);
    find_value->hosts.hosts_len++;
    strcpy(find_value->hosts.hosts_val[find_value->numNodes].host, p->host);
    
    find_value->protos.protos.protos_val[find_value->numNodes] = (p->tsp_tcp) ? IPPROTO_TCP : IPPROTO_UDP;
    find_value->numNodes++;
    pthread_mutex_unlock(&kademlia_rpc_lock);
}

kademlia_find_value_t *kademlia_find_value_1_svc(kademlia_id_t *id, struct svc_req *req)
{
    if (kademlia_data_contains(*id)) {
        kademlia_data_t *t = kademlia_data_get(*id);
        pthread_mutex_lock(&kademlia_rpc_lock);
        find_value->data.data.data_len = t->len;
        find_value->data.data.data_val = t->data;
        find_value->numNodes = 0;
        pthread_mutex_unlock(&kademlia_rpc_lock);
    } else {
        int nn;
        kademlia_peer *p;
        find_value->numNodes = 0;
        if (sem_wait(&(n->sem)) == -1) err_exit("sem_wait");
        if (n->peerCount < K) {
            nn = n->peerCount;
            for (int i = 0; i < M; i++) {
                for (int j = 0; j < n->kbuckets[i].count; j++) {
                    p = n->kbuckets[i].peers[j];
                    kademlia_find_value_add(p);
                }
            }
        } else {
            nn = K;
            if (kademlia_peer_contains(*id)) {
                p = kademlia_peer_get(*id);
                kademlia_find_value_add(p); 
            }
            while (find_value->numNodes < K) {
                p = kademlia_peer_next(p->id);
                kademlia_find_value_add(p);            
            }
        }
        if (sem_post(&(n->sem)) == -1) err_exit("sem_post");
        pthread_mutex_lock(&kademlia_rpc_lock);
        find_value->protos.protos.protos_len = nn;
        pthread_mutex_unlock(&kademlia_rpc_lock);
    }
    return find_value;
}
