#include "kademlia.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <string.h>

#define BUFSIZE 64

int main(int argc, char **argv) {
    int opt;
    char *host = NULL, *bootstrap = NULL, *tspStr = NULL;
    char udefhost = 0;

    while ((opt = getopt(argc, argv, ":hn:b:t:") )!= -1)
    {
        switch (opt)
        {
            case 'h':
                printf("%s [-h -n <hostname> -b <bootstrap host> -t <one of 'tcp', 'udp', or 'any' (defaults to any)>]\n", argv[0]);
                return 0;
            case 'n':
                host = optarg;
                udefhost = 1;
                break;
            case 'b':
                bootstrap = optarg;
                break;
            case 't':
                tspStr = optarg;
                break;
            case ':':
                fprintf(stderr, "option '-%c' requires an argument\n", optopt);
                return 1;
            case '?':
                fprintf(stderr, "unknown option '-%c'\n", optopt);
                return 1;
        }
    }

    if (host == NULL) {
        host = malloc(BUFSIZE);
        if (gethostname(host, BUFSIZE) == -1)
            err_exit("failed to get hostname");
    }
    unsigned long tsp = 0;
    if (tspStr == NULL || strcmp(tspStr, "any") == 0)
        ; // do nothing
    else if (strcmp(tspStr, "tcp") == 0)
        tsp = IPPROTO_TCP;
    else if (strcmp(tspStr, "udp") == 0)
        tsp = IPPROTO_UDP;
    else
        fprintf(stderr, "warning: unknown transport protocol, defaulting to 'any'\n");

    kademlia_init();
    kademlia_node *self = kademlia_node_create(host, tsp);

    if (bootstrap != NULL) {
        if (kademlia_network_bootstrap(bootstrap) == -1)
            err_exit("failed bootstraping into network\n");
    }

    char cmd, done = 0;
    char buf[BUFSIZE];
    uuid_t k;
    while (!done)
    {
        memset(buf, 0, BUFSIZE);
        printf("cmds:\np\tprint peers\nd\tprint data on this node\ns\tstore data in network\nf\tfetch data from network\nq\tquit\n\ncmd: ");
        cmd = getchar();
        while (getchar() != '\n');
        switch (cmd)
        {
            case 'p':
                printf("\nprinting known peers\n");
                for (int i = 0; i < M; i++) {
                    for (int j = 0; j < self->kbuckets[i].count; j++) {
                        kademlia_peer *p = self->kbuckets[i].peers[j];
                        uuid_unparse(p->id, buf);
                        printf("ID: %s\thost: %s\n", buf, p->host);
                    }
                }
                break;
            case 'd':
                printf("\nprinting data on this node\n");
                for (int i = 0; i < self->dataCount; i++) {
                    uuid_unparse((self->data[i])->key, buf);
                    printf("key: %s\tdata: %s", buf, (self->data[i])->data); // assumes data is null terminated string
                }
                break;
            case 's':
                printf("\nenter string to store: ");
                fgets(buf, BUFSIZE, stdin);
                uuid_generate_random(k);
                if (kademlia_network_store(k, buf, strlen(buf) + 1) != 0)
                    fprintf(stderr, "failed to store data\n");
                break;
            case 'f':
                printf("\nenter key to fetch: ");
                fgets(buf, BUFSIZE, stdin);
                for (int c = 0; c < BUFSIZE; c++) {
                    if (buf[c] == '\n')
                        buf[c] = 0;
                }
                if (uuid_parse(buf, k) == -1) {
                    fprintf(stderr, "error parsing UUID\n");
                    continue;
                }
                char *d = kademlia_network_fetch(k);
                if (d == NULL) {
                    fprintf(stderr, "error fetching data\n");
                    continue;
                }
                printf("data for key %s: %s\n", buf, d);
                break;
            case 'q':
                done = 1;
                break;
            default:
                fprintf(stderr, "unknown cmd: %c\n", cmd);
        }
        printf("\n");
    }
    
    if (!udefhost)
        free(host);

    kademlia_node_destroy();
    kademlia_destroy();
}
