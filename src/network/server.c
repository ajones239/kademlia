#include <stdio.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>

#include "network.h"

int passiveUDP(const char *svc) {
    int port = getPortFromSvc(svc, "udp");
    if (port == -1) return -1;
    return passiveSock(port, "udp", 0);
}

int passiveTCP(const char *svc, int qlen) {
    int port = getPortFromSvc(svc, "tcp");
    if (port == -1) return -1;
    return passiveSock(port, "tcp", qlen);
}

int passiveUDPWithPort(const int port) {
    return passiveSock(htons(port), "udp", 0);
}

int passiveTCPWithPort(const int port, int qlen) {
    return passiveSock(htons(port), "tcp", qlen);
}

int passiveSock(const int port, const char *transportProto, int qlen)
{
    struct protoent *proto;
    struct sockaddr_in addr;
    int sock, type;

    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY; // bind to all interfaces
    addr.sin_port = port;
    if ((proto = getprotobyname(transportProto)) == 0) {
        fprintf(stderr, "Error: failed to get protocol entry\n");
        return -1;
    }
    if (strcmp(transportProto, "udp") == 0)
        type = SOCK_DGRAM;
    else type = SOCK_STREAM;
    sock = socket(AF_INET, type, proto->p_proto);
    if (sock < 0) {
        fprintf(stderr, "Error: failed to create socket\n");
        return -1;
    }
    if (bind(sock, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
        fprintf(stderr, "Error: failed to bind to port %d\n", ntohs((unsigned short)port));
        return -1;
    }
    if (type == SOCK_STREAM && listen(sock, qlen) < 0) {
        fprintf(stderr, "Error: failed to listen on port %d\n", ntohs((unsigned short)port));
        return -1;
    }
    return sock;
}
