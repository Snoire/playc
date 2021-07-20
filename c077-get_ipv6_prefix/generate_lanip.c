#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <unistd.h>
#include <arpa/inet.h>


int main(int argc, char *argv[])
{
    uint8_t netidmask[16];
    uint8_t hostidmask[16];
    uint8_t netid[16];
    uint8_t hostid[16];
    int prefixlen;

    inet_pton(AF_INET6, argv[1], netid);
    inet_pton(AF_INET6, argv[2], hostid);
    prefixlen = atoi(argv[3]);

    memset(netidmask, 0xff, sizeof(netidmask));
    memset(hostidmask, 0xff, sizeof(hostidmask));

    int index = (prefixlen / 8);
    int off = (prefixlen % 8);

    if (off) {
        netidmask[index] <<= (8 - off);
        hostidmask[index] >>= off;
        memset(&netidmask[index + 1], 0, 15 - index);
        memset(hostidmask, 0, index);
    } else {
        memset(&netidmask[index], 0, 16 - index);
        memset(hostidmask, 0, index);
    }


    uint8_t addr[16];

    for (int i = 0; i < 16; i++) {
        addr[i] = (netidmask[i] & netid[i]) | (hostidmask[i] & hostid[i]);
    }

    char ip[128];
    inet_ntop(AF_INET6, addr, ip, sizeof(ip));
    printf("result: %s\n", ip);

    return 0;
}

/*
 *                          netid                       hostid   prefixlen
 * ./a.out '2a01:d0:838e:f779:a94b:21da:74b4:6ed4' '2a01:1c::30' 65
 */
