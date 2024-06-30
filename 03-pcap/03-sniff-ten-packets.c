
// gcc 03-sniff-ten-packets.c -lpcap

// man pcap_open_live
// man pcap_loop

/*
    From: /usr/include/pcap/pcap.h

struct pcap_pkthdr {
        struct timeval ts;
        bpf_u_int32 caplen;
        bpf_u_int32 len;
};

*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pcap/pcap.h>

void callback(u_char *user, const struct pcap_pkthdr *h, const u_char *bytes)
{
    static int count = 0;
    printf("Packet Count: %5d\t", ++count);
    printf("Recieved Packet Size: %5d\n", h->len);
}

int main()
{
    char *dev = "enp3s0";
    char errbuf[PCAP_ERRBUF_SIZE] = "";

    pcap_t *p;
    int snaplen = 1518;
    int promisc = 1;
    int to_ms = 1000;
    p = pcap_open_live(dev, snaplen, promisc, to_ms, errbuf);
    if (p == NULL) {
        printf("Error pcap_open_live() : %s\n", errbuf);
        exit(EXIT_FAILURE);
    }
    if (strlen(errbuf) != 0)
        printf("Warning pcap_open_live() : %s\n", errbuf);

    int cnt = 10;
    pcap_loop(p, cnt, callback, NULL);

    pcap_close(p);
    return 0;
}

