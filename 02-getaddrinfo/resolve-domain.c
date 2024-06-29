// Filename: resolve-domain.c
// Description: using getaddrinfo() to resolve domain names
// Compile: gcc -std=c99 -Wall -Wextra -Werror -pedantic -o resolve-domain resolve-domain.c

/* Output:
www.yahoo.com A 212.82.116.206
www.yahoo.com A 212.82.116.204
www.yahoo.com AAAA 2a00:1288:80:807::1
www.yahoo.com AAAA 2a00:1288:80:807::2
*/

#define _POSIX_C_SOURCE 200112L

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>

int main()
{
    char *node = "www.yahoo.com";

    struct addrinfo hints;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = 0;

    struct addrinfo *res;
    int errcode = getaddrinfo(node, NULL, &hints, &res);
    if (errcode != 0) {
        fprintf(stderr, "getaddrinfo() error: %s\n", gai_strerror(errcode));
        exit(EXIT_FAILURE);
    }

    for (struct addrinfo *p = res; p != NULL; p = p->ai_next) {

            if (p->ai_family != AF_INET && p->ai_family != AF_INET6) {
                printf("Unknown address familty: %d\n", p->ai_family);
                continue;
            }

            char *rtype;
            void *addr;
            if (p->ai_family == AF_INET) {
                rtype = "A";
                addr = &(((struct sockaddr_in *)(p->ai_addr))->sin_addr);
            } else {
                rtype = "AAAA";
                addr = &(((struct sockaddr_in6 *)(p->ai_addr))->sin6_addr);
            }

            char ipstr[INET6_ADDRSTRLEN];
            inet_ntop(p->ai_family, addr, ipstr, sizeof(ipstr));
            printf("%s %s %s\n", node, rtype, ipstr);
    }

    freeaddrinfo(res);

    return 0;
}
