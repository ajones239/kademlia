#ifndef _KADEMLIA_RPC_H_RPCGEN
#define _KADEMLIA_RPC_H_RPCGEN

#include <rpc/rpc.h>
#include <pthread.h>
#include <uuid/uuid.h>


#ifdef __cplusplus
extern "C" {
#endif

typedef unsigned char *kademlia_id_t;

typedef char *host_t;

typedef struct {
	kademlia_id_t id;
	host_t host;
	unsigned long proto;
}kademlia_ping_t;

typedef struct {
    u_int data_len;
    char *data_val;
}kademlia_data;

typedef struct {
    u_int ids_len;
    kademlia_id_t *ids_val;
}kademlia_internal_id;

typedef struct {
    u_int hosts_len;
    host_t *hosts_val;
}kademlia_internal_host;

typedef struct {
    u_int protos_len;
    unsigned long *protos_val;
}kademlia_internal_protos;

typedef struct {
	kademlia_id_t key;
    kademlia_data data;
}kademlia_store_t;

typedef struct {
    kademlia_internal_id *ids;
    kademlia_internal_host *hosts;
    kademlia_internal_protos protos;
	int numNodes;
}kademlia_find_node_t;

typedef struct {
    kademlia_internal_id *ids;
    kademlia_internal_host *hosts;
    kademlia_internal_protos protos;
	int numNodes;
    kademlia_data data;
} kademlia_find_value_t;

extern kademlia_find_node_t *find_node;
extern kademlia_find_value_t *find_value;
extern pthread_mutex_t kademlia_rpc_lock;

#define MESSAGE_PROG 0x20000001
#define MESSAGE_VERS 1

extern int RES_CDE_SUC;
extern int RES_CDE_FLR;

int kademlia_send_ping(char *rhost);
int kademlia_send_find_node(uuid_t id, char *rhost);

#if defined(__STDC__) || defined(__cplusplus)
extern void kademlia_svc_run (void *t);
#define kademlia_ping 1
extern  int * kademlia_ping_1(kademlia_ping_t *, CLIENT *);
extern  int * kademlia_ping_1_svc(kademlia_ping_t *, struct svc_req *);
#define kademlia_store 2
extern  int * kademlia_store_1(kademlia_store_t *, CLIENT *);
extern  int * kademlia_store_1_svc(kademlia_store_t *, struct svc_req *);
#define kademlia_find_node 3
extern  kademlia_find_node_t * kademlia_find_node_1(kademlia_id_t *, CLIENT *);
extern  kademlia_find_node_t * kademlia_find_node_1_svc(kademlia_id_t *, struct svc_req *);
#define kademlia_find_value 4
extern  kademlia_find_value_t * kademlia_find_value_1(kademlia_id_t *, CLIENT *);
extern  kademlia_find_value_t * kademlia_find_value_1_svc(kademlia_id_t *, struct svc_req *);
extern int message_prog_1_freeresult (SVCXPRT *, xdrproc_t, caddr_t);

#else /* K&R C */
#define kademlia_ping 1
extern  int * kademlia_ping_1();
extern  int * kademlia_ping_1_svc();
#define kademlia_store 2
extern  int * kademlia_store_1();
extern  int * kademlia_store_1_svc();
#define kademlia_find_node 3
extern  kademlia_find_node_t * kademlia_find_node_1();
extern  kademlia_find_node_t * kademlia_find_node_1_svc();
#define kademlia_find_value 4
extern  kademlia_find_value_t * kademlia_find_value_1();
extern  kademlia_find_value_t * kademlia_find_value_1_svc();
extern int message_prog_1_freeresult ();
#endif /* K&R C */

/* the xdr functions */

#if defined(__STDC__) || defined(__cplusplus)
extern  bool_t xdr_kademlia_id_t (XDR *, kademlia_id_t*);
extern  bool_t xdr_host_t (XDR *, host_t*);
extern  bool_t xdr_kademlia_ping_t (XDR *, kademlia_ping_t*);
extern  bool_t xdr_kademlia_store_t (XDR *, kademlia_store_t*);
extern  bool_t xdr_kademlia_find_node_t (XDR *, kademlia_find_node_t*);
extern  bool_t xdr_kademlia_find_value_t (XDR *, kademlia_find_value_t*);

#else /* K&R C */
extern bool_t xdr_kademlia_id_t ();
extern bool_t xdr_host_t ();
extern bool_t xdr_kademlia_ping_t ();
extern bool_t xdr_kademlia_store_t ();
extern bool_t xdr_kademlia_find_node_t ();
extern bool_t xdr_kademlia_find_value_t ();

#endif /* K&R C */

#ifdef __cplusplus
}
#endif

#endif /* !_KADEMLIA_RPC_H_RPCGEN */
