#include <linux/if_ether.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <pcap.h>
#include <stdint.h>
#include <stdio.h>

// Macro to extract the last octet of an IP address
#define LAST_OCTET(ip) ((ip) & 0xFF)
#define OCTET_COUNTS 256

int main(int argc, char *argv[]) {
  char errbuf[PCAP_ERRBUF_SIZE];
  pcap_t *handle;
  const unsigned char *packet;
  struct pcap_pkthdr header;
  struct iphdr *ip_header;
  // Array to store the count of each possible last octet
  int octet_counts[OCTET_COUNTS] = {0};
  uint8_t last_octet;
  uint32_t ip;

  if (argc != 2) {
    fprintf(stderr, "Usage: %s <pcap file>\n", argv[0]);
    return 1;
  }

  handle = pcap_open_offline(argv[1], errbuf);
  if (handle == NULL) {
    fprintf(stderr, "Error opening pcap file: %s\n", errbuf);
    return 1;
  }

  while ((packet = pcap_next(handle, &header)) != NULL) {
    ip_header = (struct iphdr *)(packet + sizeof(struct ethhdr));
    // Use ntohl to convert to host byte order, eg. big endian -> little endian
    // for intel machine
    ip = ntohl(ip_header->daddr);

    // For each packet, add 1 to the count of the last octet of the destination
    // IP address
    last_octet = LAST_OCTET(ip);
    octet_counts[last_octet]++;
  }

  // Print the count of each possible last octet
  for (int i = 0; i < OCTET_COUNTS; i++) {
    printf("Last octet %d: %d\n", i, octet_counts[i]);
  }

  pcap_close(handle);
  return 0;
}
