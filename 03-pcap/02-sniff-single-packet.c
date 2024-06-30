
// gcc 02-sniff-single-packet.c -lpcap

// man pcap_open_live
// man pcap_next

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

int main()
{
    char *dev = "enp3s0";
    char errbuf[PCAP_ERRBUF_SIZE] = "";

    int promisc = 1;
    int to_ms = 1000;       // packet buffer timeout
    int snaplen = BUFSIZ;   // snapshot length
    pcap_t *handle;
    handle = pcap_open_live(dev, snaplen, promisc, to_ms, errbuf);
    if (handle == NULL) {
        printf("Error pcap_open_live() : %s\n", errbuf);
        exit(EXIT_FAILURE);
    }
    if (strlen(errbuf) != 0) {
        printf("Warning pcap_open_live() : %s\n", errbuf);
    }

    const u_char *packet_data;
    struct pcap_pkthdr header;
    packet_data = pcap_next(handle, &header);
    if (packet_data == NULL) {
        printf("Error pcap_next(): No packet can be read.\n");
        exit(EXIT_FAILURE);
    }

    printf("Packet capture length: %d\n", header.caplen);
    printf("Packet total length %d\n", header.len);

    pcap_close(handle);
    return 0;
}
