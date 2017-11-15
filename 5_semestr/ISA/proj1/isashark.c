#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include <unistd.h>
#include <string.h>
#include <pcap/pcap.h>
#include <netinet/ether.h>
#include <arpa/inet.h>
#include <inttypes.h>

#ifndef PCAP_ERRBUF_SIZE
#define PCAP_ERRBUF_SIZE (256)
#endif

struct TCPHeader {
    u_short th_sport;	/* source port */
    u_short th_dport;	/* destination port */
    u_int th_seq;		/* sequence number */
    u_int th_ack;		/* acknowledgement number */
    u_char th_offx2;	/* data offset, rsvd */
#define TH_OFF(th)	(((th)->th_offx2 & 0xf0) >> 4)
    u_char th_flags;
#define TH_FIN 0x01
#define TH_SYN 0x02
#define TH_RST 0x04
#define TH_PUSH 0x08
#define TH_ACK 0x10
#define TH_URG 0x20
#define TH_ECE 0x40
#define TH_CWR 0x80
#define TH_FLAGS (TH_FIN|TH_SYN|TH_RST|TH_ACK|TH_URG|TH_ECE|TH_CWR)
    u_short th_win;		/* window */
    u_short th_sum;		/* checksum */
    u_short th_urp;		/* urgent pointer */
};

struct UDPHeader {
    u_short	uh_sport;		/* source port */
    u_short	uh_dport;		/* destination port */
    short	uh_ulen;		/* udp length */
    u_short	uh_sum;			/* udp checksum */
};

struct IPv6Header {
	union {
		struct ip6_hdrctl {
			u_int32_t ip6_un1_flow;	/* 20 bits of flow-ID */
			u_int16_t ip6_un1_plen;	/* payload length */
			u_int8_t  ip6_un1_nxt;	/* next header */
			u_int8_t  ip6_un1_hlim;	/* hop limit */
		} ip6_un1;
		u_int8_t ip6_un2_vfc;	/* 4 bits version, top 4 bits class */
	} ip6_ctlun;
	struct in6_addr ip6_src;	/* source address */
	struct in6_addr ip6_dst;	/* destination address */
};

struct IPv4Header {
	#if BYTE_ORDER == LITTLE_ENDIAN
		u_char  ip_hl:4,				/* header length */
		ip_v:4;				 /* version */
	#endif
	#if BYTE_ORDER == BIG_ENDIAN
		u_char  ip_v:4,				 /* version */
		ip_hl:4;				/* header length */
	#endif
	u_char ip_tos;			/* type of service */
	u_short ip_len;			/* total length */
	u_short ip_id;			/* identification */
	u_short ip_off;			/* fragment offset field */
	#define	IP_RF 0x8000			/* reserved fragment flag */
	#define	IP_DF 0x4000			/* dont fragment flag */
	#define	IP_MF 0x2000			/* more fragments flag */
	#define	IP_OFFMASK 0x1fff		/* mask for fragmenting bits */
	/*(flags_fragmentOffset & 0x4000) != 0*/
	u_char ip_ttl;			/* time to live */
	u_char ip_p;			/* protocol */
	u_short ip_sum;			/* checksum */
	struct in_addr ip_src; /* source address*/
	struct in_addr ip_dst; /* source and dest address */
};

struct ADHeader {
	uint8_t AD_dhost[ETHER_ADDR_LEN];
	uint8_t AD_shost[ETHER_ADDR_LEN];
	uint16_t AD_tpid;
	uint16_t AD_tci;
	uint16_t AD_tpid2;
	uint16_t AD_tci2;
	uint16_t AD_ether_type;
};

struct QHeader {
	uint8_t Q_dhost[ETHER_ADDR_LEN];
	uint8_t Q_shost[ETHER_ADDR_LEN];
	uint16_t Q_tpid1;
	uint16_t Q_tpid2;
	uint16_t Q_ether_type;
};

struct EthernetHeader {
    u_char ether_dhost[ETHER_ADDR_LEN]; /* Destination host address */
    u_char ether_shost[ETHER_ADDR_LEN]; /* Source host address */
    u_short ether_type; /* IP? ARP? RARP? list of ethertypes https://en.wikipedia.org/wiki/EtherType */
};

void PrintHelp()
{
	printf("This line is every help you will ever get.\n");
	exit(0);
}

void ErrorFound(int i)
{
	switch(i)
	{
		case 0:
			fprintf (stderr, "Non valid argument -l.\n");
			break;
		case 1:
			fprintf (stderr, "Option -a requires an argument.\n");
			break;
		case 2:
			fprintf (stderr, "Option -s requires an argument.\n");
			break;
		case 3:
			fprintf (stderr, "Option -l requires an argument.\n");
			break;
		case 4:
			fprintf (stderr, "Option -f requires an argument.\n");
			break;
		case 5:
			fprintf (stderr, "Unknown option.\n");
			break;
		case 6:
			fprintf (stderr, "Unknown option character.\n");
			break;
		case 7:
			printf ("Non-option argument.\n");
			break;
		case 8:
			fprintf (stderr, "Non valid argument combination.\n");
			break;	
		case 9:
			fprintf (stderr, "Can't open file for reading.\n");
			break;
	}
	exit(1);
}

int CharToInt (char *str)
{
	int length = strlen(str);
	int number = 0;
	int i;
	for (i=0; i<length; i++)
	{
		if (!isdigit(str[i]))
		{
			return -1;
		}
		if (str[i] == '-')
		{
			return -1;
		}
		number = number * 10;
		number = number + (((int) str[i]) - 48);
	}

	return number;
}

int CorrectMacAdress(char *str)
{
	char Temp[256];
	strcpy(Temp, str);
	char Temp2[256];
	char ReconstructedMacAdress[256];
	int i = 0;
	int counter = 0;
	int e = 0;
	while(i < (strlen(Temp) + 1))
	{
		if (Temp[i] != ':' && strlen(Temp) > i)
		{
			Temp2[counter] = Temp[i];
			counter++;
		}
		else
		{
			if (counter == 2)
			{
				if (i< strlen(Temp))
				{
					ReconstructedMacAdress[e] = Temp2[0];
					e++;
					ReconstructedMacAdress[e] = Temp2[1];
					e++;
					ReconstructedMacAdress[e] = ':';
					e++;
				}
				else
				{
					ReconstructedMacAdress[e] = Temp2[0];
					e++;
					ReconstructedMacAdress[e] = Temp2[1];
					e++;
					ReconstructedMacAdress[e] = '\0';
					e++;
				}
				counter = 0;
			}
			else
			{
				if (i < strlen(Temp))
				{
					ReconstructedMacAdress[e] = '0';
					e++;
					ReconstructedMacAdress[e] = Temp2[0];
					e++;
					ReconstructedMacAdress[e] = ':';
					e++;
				}
				else
				{
					ReconstructedMacAdress[e] = '0';
					e++;
					ReconstructedMacAdress[e] = Temp2[0];
					e++;
					ReconstructedMacAdress[e] = '\0';
					e++;
				}
				counter = 0;
			}
		}
		i++;
	}
	strcpy(str,ReconstructedMacAdress);
	return 0;
}

int main (int argc, char *argv[])
{
	bool hflag = false;
	int c;
	int index;
	int lvalue = -2;
	char avalue[256] = "";
	char svalue[256] = "";
	char fvalue[256] = "";
	char filename[256] = "";
	
	opterr = 0;

	while ((c = getopt (argc, argv, "ha:s:l:f:")) != -1)
	switch (c)
		{
			case 'h':
				hflag = true;
				break;
			case 'a':
				strcpy(avalue, optarg);
				break;
			case 's':
				strcpy(svalue, optarg);
				break;
			case 'l':
				lvalue = CharToInt(optarg);
				if (lvalue < 0)
					ErrorFound(0);
				break;
			case 'f':
				strcpy(fvalue, optarg);
				break;
			case '?':
				if (optopt == 'a')
					ErrorFound(1);
				else if (optopt == 's')
					ErrorFound(2);
				else if (optopt == 'l')
					ErrorFound(3);
				else if (optopt == 'f')
					ErrorFound(4);
				else if (isprint (optopt))
					ErrorFound(5);
				else
					ErrorFound(6);
			default:
				break;
			}


	for (index = optind; index < argc; index++)
	{
		//printf ("Non-option argument %s\n", argv[index]);
		strcpy(filename, argv[index]);
		//ErrorFound(7);
	}

	if (hflag == true && strcmp(avalue, "") == 0 && strcmp(svalue, "") == 0 && strcmp(fvalue, "") == 0 && lvalue == -2)
	{
		PrintHelp();
	}
	else
	{
		if (hflag == true && (strcmp(avalue, "") != 0 || strcmp(svalue, "") == 0 || strcmp(fvalue, "") == 0 || lvalue == -2))
		{
			ErrorFound(8);
		}
	}

	//printf("%s\n", filename);
	//printf ("hflag = %d, avalue = %s, svalue = %s, lvalue = %d, fvalue = %s\n", hflag, avalue, svalue, lvalue, fvalue);

	pcap_t *handle; //ukazatel na soubor s pakety
	const u_char *packet;
	char errbuf[256];
	struct pcap_pkthdr header;

	//nepopsane struktury
	const struct tcphdr *my_tcp;
  	const struct udphdr *my_udp;
  	struct ether_header *eptr;
  	struct ADHeader *adptr;
	struct QHeader *qptr;
	struct IPv4Header *ipv4ptr;  //struktura pro IPv4 hlavicku
	struct IPv6Header *ipv6ptr;  //struktura pro IPv6 hlavicku
	const struct TCPHeader *tcpptr;    // pointer to the beginning of TCP header
	const struct UDPHeader *udpptr;    // pointer to the beginning of UDP header
	char printAbleIPv4src[INET6_ADDRSTRLEN];
	char printAbleIPv4dst[INET6_ADDRSTRLEN];
	char printAbleIPv6src[INET6_ADDRSTRLEN];
	char printAbleIPv6dst[INET6_ADDRSTRLEN];
	int PacketNumber = 0;


	if ((handle = pcap_open_offline(filename,errbuf)) == NULL)
		ErrorFound(9);
	
	while ((packet = pcap_next(handle,&header)) != NULL)  //v header jsou hodnoty hlavicky paketu, v packetu je ukazatel na zacatek
	{
		int EthTypeSize = 0;
		int IpSize = 0;
		PacketNumber++;
		eptr = (struct ether_header *) packet;

		switch (ntohs(eptr->ether_type))
		{
			case 0x0800: //ETHERTYPE_IP IPV4
			{
				EthTypeSize = 14;
				printf("| Ethernet %s %s\n",ether_ntoa((const struct ether_addr *)&eptr->ether_shost), ether_ntoa((const struct ether_addr *)&eptr->ether_dhost));
				ipv4ptr = (struct IPv4Header*) (packet+EthTypeSize);
				switch (ipv4ptr->ip_p)
				{
					case 1:  //ICMP protocol
					{
						printf("ICMP\n");
						break;
					}
					case 6:  //TCP protocol
					{
						printf("TCP\n");
						break;
					}
					case 17: //UDP protocol
					{
						printf("UDP\n");
						break;
					}
					default:
					{
						printf("%x - Nor TCP nor UDP.\n", ipv4ptr->ip_p);
						break;
					}
				}
				break;
			}
			case 0x86DD: //ETHERTYPE_IP IPV6
			{
				EthTypeSize = 14;
				printf("| Ethernet %s %s\n",ether_ntoa((const struct ether_addr *)&eptr->ether_shost), ether_ntoa((const struct ether_addr *)&eptr->ether_dhost));
				ipv6ptr = (struct IPv6Header*) (packet+EthTypeSize);
				switch (ipv6ptr->ip6_ctlun.ip6_un1.ip6_un1_nxt)
				{
					case 6:  //TCP protocol
					{
						printf("TCP\n");
						break;
					}
					case 17: //UDP protocol
					{
						printf("UDP\n");
						break;
					}
					case 58:  //ICMPv6
					{
						printf("ICMPv6\n");
						break;
					}
					default:
					{
						printf("%x - Nor TCP nor UDP.\n", ipv6ptr->ip6_ctlun.ip6_un1.ip6_un1_nxt);
						break;
					}
				}
				break;
			}
			case 0x8100: //IEEE 802.1Q
			{
				EthTypeSize = 18;
				//printf("| Ethernet %s %s\n",ether_ntoa((const struct ether_addr *)&eptr->ether_shost), ether_ntoa((const struct ether_addr *)&eptr->ether_dhost));
				qptr = (struct QHeader *) packet;

				if (ntohs(qptr->Q_ether_type) == 0x0800) //IPv4
				{
					ipv4ptr = (struct IPv4Header*) (packet+EthTypeSize);
					IpSize = ipv4ptr->ip_hl*4;

					switch (ipv4ptr->ip_p)
					{
						case 1:  //ICMP protocol
						{
							printf("ICMP\n");
							break;
						}
						case 6:  //TCP protocol
						{
							printf("TCP\n");
							break;
						}
						case 17: //UDP protocol
						{
							printf("UDP\n");
							break;
						}
						default:
						{
							printf("%x - Nor TCP nor UDP.\n", ipv4ptr->ip_p);
							break;
						}
					}
				}
				else 
				{
					if (ntohs(qptr->Q_ether_type) == 0x86DD) //IPv6
					{
						ipv6ptr = (struct IPv6Header*) (packet+EthTypeSize);
						IpSize = 40;  //checknout co je ve skutecnosti payload len

						switch (ipv6ptr->ip6_ctlun.ip6_un1.ip6_un1_nxt)
						{
							case 6:  //TCP protocol
							{
								printf("TCP\n");
								break;
							}
							case 17: //UDP protocol
							{
								char MacSrc[256];
								char MacDst[256];
								strcpy(MacSrc, ether_ntoa((const struct ether_addr *)&eptr->ether_shost));
								strcpy(MacDst, ether_ntoa((const struct ether_addr *)&eptr->ether_dhost));
								CorrectMacAdress(MacSrc);
								CorrectMacAdress(MacDst);
								udpptr = (struct UDPHeader *) (packet+EthTypeSize+IpSize);
								printf("%d: %lu%lu %d | Ethernet: %s %s %d | IPv6: %s %s %d | UDP: %d %d\n",
								PacketNumber,
								header.ts.tv_sec,
								header.ts.tv_usec,
								header.len,
								MacSrc,
								MacDst,
								ntohs(qptr->Q_tpid2),
								inet_ntop(AF_INET6, &(ipv6ptr->ip6_src), printAbleIPv6src, INET6_ADDRSTRLEN),
								inet_ntop(AF_INET6, &(ipv6ptr->ip6_dst), printAbleIPv6dst, INET6_ADDRSTRLEN),
								ipv6ptr->ip6_ctlun.ip6_un1.ip6_un1_hlim,
								ntohs(udpptr->uh_sport),
								ntohs(udpptr->uh_dport));
								//printf("UDP\n");
								break;
							}
							case 58:  //ICMPv6
							{
								printf("ICMPv6\n");
								break;
							}
							default:
							{
								printf("%x - Nor TCP nor UDP.\n", ipv6ptr->ip6_ctlun.ip6_un1.ip6_un1_nxt);
								break;
							}
						}
					}
					else
					{
						printf("Nor IPv4 nor IPv6.\n");
					}
				}
				break;
			}
			case 0x88a8: //IEEE 802.1ad
			{
				EthTypeSize = 22;
				printf("| Ethernet %s %s\n",ether_ntoa((const struct ether_addr *)&eptr->ether_shost), ether_ntoa((const struct ether_addr *)&eptr->ether_dhost));
				adptr = (struct ADHeader *) packet;
				
				if (ntohs(adptr->AD_ether_type) == 0x0800) //IPv4
				{
					ipv4ptr = (struct IPv4Header*) (packet+EthTypeSize);
					switch (ipv4ptr->ip_p)
					{
						case 1:  //ICMP protocol
						{
							printf("ICMP\n");
							break;
						}
						case 6:  //TCP protocol
						{
							printf("TCP\n");
							break;
						}
						case 17: //UDP protocol
						{
							printf("UDP\n");
							break;
						}
						default:
						{
							printf("%x - Nor TCP nor UDP.\n", ipv4ptr->ip_p);
							break;
						}
					}
				}
				else 
				{
					if (ntohs(adptr->AD_ether_type) == 0x86DD) //IPv6
					{
						ipv6ptr = (struct IPv6Header*) (packet+EthTypeSize);
						switch (ipv6ptr->ip6_ctlun.ip6_un1.ip6_un1_nxt)
						{
							case 6:  //TCP protocol
							{
								printf("TCP\n");
								break;
							}
							case 17: //UDP protocol
							{
								printf("UDP\n");
								break;
							}
							case 58:  //ICMPv6
							{
								printf("ICMPv6\n");
								break;
							}
							default:
							{
								printf("%x - Nor TCP nor UDP.\n", ipv6ptr->ip6_ctlun.ip6_un1.ip6_un1_nxt);
								break;
							}
						}
					}
					else
					{
						printf("Nor IPv4 nor IPv6.\n");
					}
				}
			break;
			}
			default: //chyba osetrit
			{
				EthTypeSize = -1;
				break;
			}
		}
	}
	return 0;
}