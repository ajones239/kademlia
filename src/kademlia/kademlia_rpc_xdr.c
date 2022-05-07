#include "kademlia_rpc.h"

bool_t xdr_kademlia_id_t (XDR *xdrs, kademlia_id_t *objp) {
    register int32_t *buf;
    if (!xdr_string (xdrs, (char **)objp, ~0))
        return FALSE;
    return TRUE;
}

bool_t xdr_host_t (XDR *xdrs, host_t *objp) {
    register int32_t *buf;
    if (!xdr_string (xdrs, objp, ~0))
        return FALSE;
    return TRUE;
}

bool_t xdr_kademlia_ping_t (XDR *xdrs, kademlia_ping_t *objp) {
    register int32_t *buf;
    if (!xdr_kademlia_id_t (xdrs, &objp->id))
        return FALSE;
    if (!xdr_host_t (xdrs, &objp->host))
        return FALSE;
    if (!xdr_u_long (xdrs, &objp->proto))
        return FALSE;
    return TRUE;
}

bool_t xdr_kademlia_store_t (XDR *xdrs, kademlia_store_t *objp) {
    register int32_t *buf;
    if (!xdr_kademlia_id_t (xdrs, &objp->key))
        return FALSE;
    if (!xdr_array (xdrs, (char **)&(objp->data.data_val), (u_int *) &(objp->data.data_len), ~0, sizeof(char), (xdrproc_t)xdr_char))
        return FALSE;
    return TRUE;
}

bool_t xdr_kademlia_find_node_t (XDR *xdrs, kademlia_find_node_t *objp)
{
    register int32_t *buf;
    if (!xdr_array (xdrs, (char **)&(objp->ids->ids_val), (u_int *) &(objp->ids->ids_len), ~0, sizeof(kademlia_id_t), (xdrproc_t)xdr_kademlia_id_t))
        return FALSE;
    if (!xdr_array (xdrs, (char **)&(objp->hosts->hosts_val), (u_int *) &(objp->hosts->hosts_len), ~0, sizeof(host_t), (xdrproc_t)xdr_host_t))
        return FALSE;
    if (!xdr_array (xdrs, (char **)&(objp->protos.protos_val), (u_int *) &(objp->protos.protos_len), ~0, sizeof(int), (xdrproc_t)xdr_u_long))
        return FALSE;
    if (!xdr_int (xdrs, &objp->numNodes))
        return FALSE;
    return TRUE;
}

bool_t xdr_kademlia_find_value_t (XDR *xdrs, kademlia_find_value_t *objp)
{
    register int32_t *buf;
    if (!xdr_array (xdrs, (char **)&(objp->ids->ids_val), (u_int *) &(objp->ids->ids_len), ~0, sizeof(kademlia_id_t), (xdrproc_t)xdr_kademlia_id_t))
        return FALSE;
    if (!xdr_array (xdrs, (char **)&(objp->hosts->hosts_val), (u_int *) &(objp->hosts->hosts_len), ~0, sizeof(host_t), (xdrproc_t)xdr_host_t))
        return FALSE;
    if (!xdr_array (xdrs, (char **)&(objp->protos.protos_val), (u_int *) &(objp->protos.protos_len), ~0, sizeof(int), (xdrproc_t)xdr_u_long))
        return FALSE;
    if (!xdr_int (xdrs, &objp->numNodes))
        return FALSE;
    if (!xdr_array (xdrs, (char **)&(objp->data.data_val), (u_int *) &(objp->data.data_len), ~0, sizeof(char), (xdrproc_t)xdr_char))
        return FALSE;
    return TRUE;
}
