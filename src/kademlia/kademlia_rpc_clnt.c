#include <memory.h> /* for memset */
#include "kademlia_rpc.h"

/* Default timeout can be changed using clnt_control() */
static struct timeval TIMEOUT = { 25, 0 };

int *
kademlia_ping_1(kademlia_ping_t *argp, CLIENT *clnt)
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

int *
kademlia_store_1(kademlia_store_t *argp, CLIENT *clnt)
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

kademlia_find_node_t *
kademlia_find_node_1(kademlia_id_t *argp, CLIENT *clnt)
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

kademlia_find_value_t *
kademlia_find_value_1(kademlia_id_t *argp, CLIENT *clnt)
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