#include <stdio.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>

#include "network.h"

int connectTCP(char *host, char *svc) {
    int port = getPortFromSvc(svc, "tcp");
    if (port == -1) return -1;
    return connectSock(host, port, "tcp");
}

int connectUDP(char *host, char *svc) {
    int port = getPortFromSvc(svc, "udp");
    if (port == -1) return -1;
    return connectSock(host, port, "udp");
}

int connectTCPWithPort(char *host, int port) {
    return connectSock(host, htons(port), "tcp");
}

int connectUDPWithPort(char *host, int port) {
    return connectSock(host, htons(port), "udp");
}

int connectSock(char *hostStr, int port, char *transportProto)
{
    struct hostent *host;
    struct protoent *proto;
    struct sockaddr_in addr;
    int sock, type;

    memset(&addr, 0, sizeof addr);
    addr.sin_family = AF_INET;
    addr.sin_port = port;
    if ((host = gethostbyname(hostStr)))
        memcpy(&addr.sin_addr, host->h_addr_list, host->h_length);
    else {
        return -1;
    }
    if ((proto = getprotobyname(transportProto)) == 0) {
        return -1;
    }
    if (strcmp(transportProto, "udp") == 0)
        type = SOCK_DGRAM;
    else type = SOCK_STREAM;
    sock = socket(AF_INET, type, proto->p_proto);
    if (sock < 0) {
        return -1;
    }
    if (connect(sock, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
        return -1;
    }
    return sock;
}
