#include <stdio.h>
#include <netdb.h>

#include "network.h"

int getPortFromSvc(const char *svcStr, const char *transportProto) {
    struct servent *svc;
    if ((svc = getservbyname(svcStr, transportProto)))
        return svc->s_port;
    else {  // error, can't find service
        fprintf(stderr, "Error: invalid service name\n");
        return -1;
    }
}
