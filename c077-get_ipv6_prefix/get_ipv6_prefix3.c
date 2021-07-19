#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <endian.h>
#include <arpa/inet.h>

int main(int argc, char **argv)
{
    int prefix_len = atoi(argv[2]), mask_len = 128 - prefix_len;
    struct in6_addr ip6_network;

    inet_pton(AF_INET6, argv[1], &ip6_network);

    printf("ipv6:\n");
    for (int i = 0; i < 16; i++) { /* 网络字节序，所以从 0 开始输出刚好可以从高位打印 */
        printf("%.2X ", ip6_network.__in6_u.__u6_addr8[i]);
    }
    printf("\n");

    /* get prefix */
    struct in6_addr ip6_prefix = ip6_network;
#if 0
    int a = mask_len / 8;
    int b = mask_len % 8;

    /* 根据 mask 的长度，把右边全部置 0，剩下的就是 prefix */
    for (int i = 15; i > 15-a; i--) {
        ip6_prefix.__in6_u.__u6_addr8[i] = 0;           /* 以字节为单位置 0 */
    }
    for (int i = 0; i < b; i++) {
        ip6_prefix.__in6_u.__u6_addr8[15-a] &= ~(1<<i); /* 可能还多一些不足一个字节的，0 '与'置 0 */
    }

#else
    if (mask_len < 64) {
        /* 网络字节序转换成主机字节序 */
        uint64_t ip2 = be64toh(*(uint64_t *) &ip6_prefix.__in6_u.__u6_addr32[2]);

        uint64_t mask = 0LL;
        for (int i = 0; i < mask_len; i++) {
            mask |= 1LL << i;
        }
        mask = ~mask;
        ip2 &= mask;

        /* 再转回来，毕竟 ip6_prefix 需要网络字节序。。 */
        ip2 = htobe64(ip2);
        memcpy(&ip6_prefix.__in6_u.__u6_addr32[2], &ip2, sizeof(ip2));
    }
#endif

    char prefix[INET6_ADDRSTRLEN] = "";
    inet_ntop(AF_INET6, &ip6_prefix, prefix, INET6_ADDRSTRLEN);

    printf("prefix:\n");
    printf("%s\n", prefix);
    return 0;
}


/*
 * ./get_prefix '2a01:d0:838e:f700:194b:21da:74b4:6ed4' 64
 */
