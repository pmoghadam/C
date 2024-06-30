
// gcc 01-netinfo.c -lpcap

// man pcap_findalldevs
// man sockaddr
// man htonl
// man inet_ntop

/*
    From: /usr/include/pcap/pcap.h

typedef struct pcap_if pcap_if_t;

struct pcap_if {
    struct pcap_if *next;
    char *name;
    char *description;
    struct pcap_addr *addresses;
    bpf_u_int32 flags;
};

typedef struct pcap_addr pcap_addr_t;

struct pcap_addr {
    struct pcap_addr *next;
    struct sockaddr *addr;
    struct sockaddr *netmask;
    struct sockaddr *broadaddr;
    struct sockaddr *dstaddr;
};

*/

#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <pcap/pcap.h>
#include <arpa/inet.h>

void print_ip(struct sockaddr *sap)
{
   switch (sap->sa_family) {

       case AF_INET: {

           struct in_addr addr;

           char ip_str[INET_ADDRSTRLEN];
           addr =  ((struct sockaddr_in *)sap)->sin_addr;
           inet_ntop(AF_INET, &addr, ip_str, INET_ADDRSTRLEN);

           printf("%s", ip_str);
           break;
       }

       case AF_INET6: {

           struct in6_addr addr;

           char ip_str[INET6_ADDRSTRLEN];
           addr = ((struct sockaddr_in6 *)sap)->sin6_addr;
           inet_ntop(AF_INET6, &addr, ip_str, INET6_ADDRSTRLEN);

           printf("%s", ip_str);
           break;
       }

       default:
           break;
   }
}

int cidr_prefix(struct sockaddr *netmaskp)
{
    int prefix;

    switch (netmaskp->sa_family) {

        case AF_INET: {
            struct in_addr addr;
            addr = ((struct sockaddr_in *)netmaskp)->sin_addr;
            uint32_t n = htonl((uint32_t)addr.s_addr);
            for (prefix = 0; n > 0; prefix++)
                n = n << 1;
            break;
        }

        case AF_INET6: {
            struct in6_addr *addr;
            addr = &(((struct sockaddr_in6 *)netmaskp)->sin6_addr);
            prefix = 0;
            for (int i = 0; i < 16; i++) {
                unsigned char c = *((char *)addr + i);
                if (c == 0)
                    break;
                while (c > 0) {
                    c = c << 1;
                    prefix++;
                }
            }
            break;
        }

        default:
            break;
    }

    return prefix;
}

void show_ip_addresses(pcap_addr_t *a)
{
    while (a != NULL) {
        if (a->addr->sa_family == AF_INET || a->addr->sa_family == AF_INET6) {

            printf("\tIPv%d: ", (a->addr->sa_family == AF_INET)?4:6);
            print_ip(a->addr);

            if (a->netmask != NULL) {
                printf("/%d  Netmask: ", cidr_prefix(a->netmask));
                print_ip(a->netmask);
            }

            printf("\n");
        }

        a = a->next;
    }
}

void read_flags(bpf_u_int32 f)
{
    printf("\tFlags: ");

    if (f & PCAP_IF_LOOPBACK)
        printf("Loopback ");

    if (f & PCAP_IF_UP)
       printf("Up ");
    else
       printf("Down ");

    if (f & PCAP_IF_RUNNING)
       printf("Running ");
    else
       printf("Not-running ");

    if (f & PCAP_IF_WIRELESS)
       printf("Wireless ");

    bpf_u_int32 status = f & PCAP_IF_CONNECTION_STATUS;

    if (status == PCAP_IF_CONNECTION_STATUS_UNKNOWN)
       printf("Unknown ");

    if (status == PCAP_IF_CONNECTION_STATUS_CONNECTED)
       printf("Connected ");

    if (status == PCAP_IF_CONNECTION_STATUS_DISCONNECTED)
       printf("Disconnected ");

    if (status == PCAP_IF_CONNECTION_STATUS_NOT_APPLICABLE)
       printf("Not-applicable ");

    printf("\n");
}

void network_info(void)
{
    char errbuf[PCAP_ERRBUF_SIZE];
    pcap_if_t *alldevsp, *devp;

    if ( (pcap_findalldevs(&alldevsp, errbuf)) == PCAP_ERROR) {
        printf("Error pcap_findalldevs() : %s\n", errbuf);
        exit(EXIT_FAILURE);
    }

    for(devp = alldevsp; devp != NULL; devp = devp->next) {
        printf("Interface: %s\n", devp->name);

        printf("\tDescription: %s\n",
                (devp->description == NULL)?" ":devp->description);

        read_flags(devp->flags);

        show_ip_addresses(devp->addresses);

        printf("\n");
    }

    pcap_freealldevs(alldevsp);
}

int main()
{
    network_info();
    return 0;
}
