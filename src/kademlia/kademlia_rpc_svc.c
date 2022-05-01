/* #include "kademlia_rpc.h" */

#include "kademlia.h"

#include <stdio.h>
#include <stdlib.h>
#include <rpc/pmap_clnt.h>
#include <string.h>
#include <memory.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <uuid/uuid.h>

#ifndef SIG_PF
#define SIG_PF void(*)(int)
#endif

const int RES_CDE_SUC = 0;
const int RES_CDE_FLR = -1;

static void message_prog_1(struct svc_req *rqstp, register SVCXPRT *transp)
{
    union {
        kademlia_ping_t kademlia_ping_1_arg;
        kademlia_store_t kademlia_store_1_arg;
        kademlia_id_t kademlia_find_node_1_arg;
        kademlia_id_t kademlia_find_value_1_arg;
    }argument;
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
}

void kademlia_svc_run (void *t)
{
    register SVCXPRT *transp;
    pmap_unset (MESSAGE_PROG, MESSAGE_VERS);
    if (n->addr.tsp_udp)
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
    if (n->addr.tsp_tcp)
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
    if (kademlia_peer_contains(pt->id))
        kademlia_peer_update(pt->id);
    else
    {
        kademlia_node *new = malloc(sizeof(kademlia_node));
        memcpy(new->id, pt->id, sizeof(uuid_t));
        size_t hsize = strlen(pt->host) + 1;
        new->addr.host = malloc(hsize);
        snprintf(new->addr.host, hsize, "%s", pt->host);
        if (pt->proto == IPPROTO_TCP) {
            new->addr.tsp_tcp = 1;
            new->addr.tsp_udp = 0;
        } else if (pt->proto == IPPROTO_UDP) {
            new->addr.tsp_udp = 1;
            new->addr.tsp_tcp = 0;
        }
        time(&(new->lastSeen));
        kademlia_peer_add(new);
    }
    return &RES_CDE_SUC;
}

int *kademlia_store_1_svc(kademlia_store_t *st, struct svc_req *req) {
    int i = 0;
    return &i;
}

kademlia_find_node_t *kademlia_find_node_1_svc(kademlia_id_t *it, struct svc_req *req) {
    return NULL;
}

kademlia_find_value_t *kademlia_find_value_1_svc(kademlia_id_t *it, struct svc_req *req) {
    return NULL;
}


