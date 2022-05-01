#ifndef _KADEMLIA_RPC_H_RPCGEN
#define _KADEMLIA_RPC_H_RPCGEN

#include <rpc/rpc.h>


#ifdef __cplusplus
extern "C" {
#endif

typedef unsigned char *kademlia_id_t;

typedef char *host_t;

struct kademlia_ping_t {
	kademlia_id_t id;
	host_t host;
	unsigned long proto;
};
typedef struct kademlia_ping_t kademlia_ping_t;

struct kademlia_store_t {
	kademlia_id_t key;
	struct {
		u_int data_len;
		char *data_val;
	} data;
};
typedef struct kademlia_store_t kademlia_store_t;

struct kademlia_find_node_t {
	struct {
		u_int ids_len;
		kademlia_id_t *ids_val;
	} ids;
	struct {
		u_int hosts_len;
		host_t *hosts_val;
	} hosts;
	struct {
		u_int protos_len;
		unsigned long *protos_val;
	} protos;
	int numNodes;
};
typedef struct kademlia_find_node_t kademlia_find_node_t;

struct kademlia_find_value_t {
	struct {
		u_int ids_len;
		kademlia_id_t *ids_val;
	} ids;
	struct {
		u_int hosts_len;
		host_t *hosts_val;
	} hosts;
	struct {
		u_int protos_len;
		unsigned long *protos_val;
	} protos;
	int numNodes;
	struct {
		u_int data_len;
		char *data_val;
	} data;
};
typedef struct kademlia_find_value_t kademlia_find_value_t;

#define MESSAGE_PROG 0x20000001
#define MESSAGE_VERS 1

extern const int RES_CDE_SUC;
extern const int RES_CDE_FLR;

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
