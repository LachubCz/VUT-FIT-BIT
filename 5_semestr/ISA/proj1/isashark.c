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
#include <vector>
#include <string>

#include <stack>
#include <iostream>
using namespace std;

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

struct ETHHeader {
    uint8_t ether_dhost[ETHER_ADDR_LEN];
    uint8_t ether_shost[ETHER_ADDR_LEN];
    uint16_t ether_type;
};

struct PacketData {
	int Layer1;
	struct ETHHeader eptr;
	struct ADHeader adptr;
	struct QHeader qptr;
	int Layer2;
	struct IPv4Header ipv4ptr;
	struct IPv6Header ipv6ptr;
	int Layer3;
	struct TCPHeader tcpptr;
	struct UDPHeader udpptr;
	int PacketNumber;
	struct timeval ts;
	bpf_u_int32 len;
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

int GetNumberOfPackets(char *filename)
{
	int counter = 0;
	pcap_t *handle; //ukazatel na soubor s pakety
	const u_char *packet;
	char errbuf[256];
	struct pcap_pkthdr header;

	if ((handle = pcap_open_offline(filename,errbuf)) == NULL)
		ErrorFound(9);
	
	while ((packet = pcap_next(handle,&header)) != NULL)  //v header jsou hodnoty hlavicky paketu, v packetu je ukazatel na zacatek
	{
		counter++;
	}

	pcap_close(handle);
	return counter;
}

void printPacket(struct PacketData PacketList)
{
	char MacSrc[256];
	char MacDst[256];
	char printAbleIPv6src[INET6_ADDRSTRLEN];
	char printAbleIPv6dst[INET6_ADDRSTRLEN];

	switch (PacketList.Layer1)
	{
		case 0:  //Ethernet
		{
			strcpy(MacSrc, ether_ntoa((const struct ether_addr *)&PacketList.eptr.ether_shost));
			strcpy(MacDst, ether_ntoa((const struct ether_addr *)&PacketList.eptr.ether_dhost));
			CorrectMacAdress(MacSrc);
			CorrectMacAdress(MacDst);

			printf("%d: %ld%06ld %d | Ethernet: %s %s | ",
				PacketList.PacketNumber,
				PacketList.ts.tv_sec,
				PacketList.ts.tv_usec,
				PacketList.len,
				MacSrc,
				MacDst);
			break;
		}
		case 1:  //IEEE 802.1Q
		{
			strcpy(MacSrc, ether_ntoa((const struct ether_addr *)&PacketList.qptr.Q_shost));
			strcpy(MacDst, ether_ntoa((const struct ether_addr *)&PacketList.qptr.Q_dhost));
			CorrectMacAdress(MacSrc);
			CorrectMacAdress(MacDst);

			printf("%d: %ld%06ld %d | Ethernet: %s %s %d | ",
				PacketList.PacketNumber,
				PacketList.ts.tv_sec,
				PacketList.ts.tv_usec,
				PacketList.len,
				MacSrc,
				MacDst,
				ntohs(PacketList.qptr.Q_tpid2)); //tisknu spatne nhtons
			break;
		}
		case 2:  //IEEE 802.1ad
		{
			strcpy(MacSrc, ether_ntoa((const struct ether_addr *)&PacketList.adptr.AD_shost));
			strcpy(MacDst, ether_ntoa((const struct ether_addr *)&PacketList.adptr.AD_dhost));
			CorrectMacAdress(MacSrc);
			CorrectMacAdress(MacDst);

			printf("%d: %ld%06ld %d | Ethernet: %s %s %d %d | ",
				PacketList.PacketNumber,
				PacketList.ts.tv_sec,
				PacketList.ts.tv_usec,
				PacketList.len,
				MacSrc,
				MacDst,
				ntohs(PacketList.qptr.Q_tpid2),
				ntohs(PacketList.qptr.Q_tpid2));
			break;
		}
		default:
		{
			break;
		}
	}

	switch (PacketList.Layer2)
	{
		case 0:  //IPv4
		{
			printf("IPv4: %s ",
				inet_ntoa(PacketList.ipv4ptr.ip_src));
			printf("%s %d | ",
				inet_ntoa(PacketList.ipv4ptr.ip_dst),
				PacketList.ipv4ptr.ip_ttl);
			break;
		}
		case 1:  //IPv6
		{
			printf("IPv6: %s %s %d | ",
				inet_ntop(AF_INET6, &(PacketList.ipv6ptr.ip6_src), printAbleIPv6src, INET6_ADDRSTRLEN),
				inet_ntop(AF_INET6, &(PacketList.ipv6ptr.ip6_dst), printAbleIPv6dst, INET6_ADDRSTRLEN),
				PacketList.ipv6ptr.ip6_ctlun.ip6_un1.ip6_un1_hlim);
			break;
		}
		default:
		{
			break;
		}
	}

	switch (PacketList.Layer3)
	{
		case 0:  //ICMPv4
		{

		}
		case 1:  //TCP
		{
			printf("TCP: %d %d %u %u ",
				ntohs(PacketList.tcpptr.th_sport),
				ntohs(PacketList.tcpptr.th_dport),
				PacketList.tcpptr.th_seq & 0xff,
				PacketList.tcpptr.th_ack & 0xff);
			if (PacketList.tcpptr.th_flags & TH_CWR)
        		printf("C");
    		else
    			printf(".");
    		if (PacketList.tcpptr.th_flags & TH_ECE)
        		printf("E");
    		else
    			printf(".");
    		if (PacketList.tcpptr.th_flags & TH_URG)
        		printf("U");
    		else
    			printf(".");
    		if (PacketList.tcpptr.th_flags & TH_ACK)
        		printf("A");
    		else
    			printf(".");
    		if (PacketList.tcpptr.th_flags & TH_PUSH)
        		printf("P");
    		else
    			printf(".");
    		if (PacketList.tcpptr.th_flags & TH_RST)
        		printf("R");
    		else
    			printf(".");
    		if (PacketList.tcpptr.th_flags & TH_SYN)
        		printf("S");
    		else
    			printf(".");
    		if (PacketList.tcpptr.th_flags & TH_FIN)
        		printf("F");
    		else
    			printf(".");
    		printf("\n");
			break;
		}
		case 2:  //UDP
		{
			printf("UDP: %d %d\n", 
				ntohs(PacketList.udpptr.uh_sport),
				ntohs(PacketList.udpptr.uh_dport));
			break;
		}
		case 3:  //ICMPv6
		{
			break;
		}
		default: 
		{
			break;
		}
	}
}

int GetMax(std::vector<PacketData> PacketList, int NumberOfPackets)
{
	int Max = 0;
	int Order = 0;
	int i;
	for (i = 0; i < NumberOfPackets; i++)
	{
		if (i == 0)
		{
			Max = PacketList.at(i).len;
			Order = i;
		}
		if (PacketList.at(i).len >= Max)
		{
			Max = PacketList.at(i).len;
			Order = i;
		}
	}
	return Order;
}

int main (int argc, char *argv[])
{


	bool hflag = false;
	int c;
	int index;
	int lvalue = -2;
	char avalue[256] = "";
	int svalue = -2;
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
			if (strcmp(optarg, "packets") == 0)
			{
				svalue = 0;
			}
			if (strcmp(optarg, "bytes") == 0)
			{
				svalue = 1;
			}
			//strcpy(svalue, optarg);
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

	if (hflag == true && strcmp(avalue, "") == 0 && svalue == -2 && strcmp(fvalue, "") == 0 && lvalue == -2)
	{
		PrintHelp();
	}
	else
	{
		if (hflag == true && (strcmp(avalue, "") != 0 || svalue != -2 || strcmp(fvalue, "") != 0 || lvalue != -2))
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
  	struct ETHHeader *eptr;
  	struct ADHeader *adptr;
	struct QHeader *qptr;
	struct IPv4Header *ipv4ptr;  //struktura pro IPv4 hlavicku
	struct IPv6Header *ipv6ptr;  //struktura pro IPv6 hlavicku
	const struct TCPHeader *tcpptr;    // pointer to the beginning of TCP header
	const struct UDPHeader *udpptr;    // pointer to the beginning of UDP header

	int PacketNumber = 0;

	if ((handle = pcap_open_offline(filename,errbuf)) == NULL)
		ErrorFound(9);
	
	int NumberOfPackets = GetNumberOfPackets(filename);
	
	std::vector<PacketData>  PacketList(NumberOfPackets);

	struct PacketData *ahoj;
	while ((packet = pcap_next(handle,&header)) != NULL)  //v header jsou hodnoty hlavicky paketu, v packetu je ukazatel na zacatek
	{
		int EthTypeSize = 0;
		int IpSize = 0;
		PacketNumber++;
		eptr = (struct ETHHeader *) packet;

		switch (ntohs(eptr->ether_type))
		{
			case 0x0800: //ETHERTYPE_IP IPV4
			{
				EthTypeSize = 14;
				//IpSize = ipv4ptr->ip_hl*4;
				IpSize = 20;

				PacketList[PacketNumber - 1].eptr = *eptr;
				PacketList[PacketNumber - 1].Layer1 = 0;
				
				break;
			}
			case 0x86DD: //ETHERTYPE_IP IPV6
			{
				EthTypeSize = 14;
				IpSize = 40;

				PacketList[PacketNumber - 1].eptr = *eptr;
				PacketList[PacketNumber - 1].Layer1 = 0;

				break;
			}
			case 0x8100: //IEEE 802.1Q
			{
				EthTypeSize = 18;
				qptr = (struct QHeader *) packet;

				PacketList[PacketNumber - 1].qptr = *qptr;
				PacketList[PacketNumber - 1].Layer1 = 1;

				switch (ntohs(qptr->Q_ether_type))
				{
					case 0x0800:  //IPv4
					{
						//IpSize = ipv4ptr->ip_hl*4;
						IpSize = 20;
						break;
					}
					case 0x86DD:  //IPv6
					{
						IpSize = 40;
						break;
					}
					default:  //not IPv6 nor IPv4 OSETRIT
					{
						break;
					}
				}

				break;
			}
			case 0x88a8: //IEEE 802.1ad
			{
				EthTypeSize = 22;
				adptr = (struct ADHeader *) packet;

				PacketList[PacketNumber - 1].adptr = *adptr;
				PacketList[PacketNumber - 1].Layer1 = 2;

				switch (ntohs(adptr->AD_ether_type))
				{
					case 0x0800:  //IPv4
					{
						//IpSize = ipv4ptr->ip_hl*4;
						IpSize = 20;
						break;
					}
					case 0x86DD:  //IPv6
					{
						IpSize = 40;
						break;
					}
					default:  //not IPv6 nor IPv4 OSETRIT
					{
						break;
					}
				}

				break;	
			}
		}

		if (ntohs(eptr->ether_type) == 0x0800 || (ntohs(eptr->ether_type) == 0x8100 && ntohs(qptr->Q_ether_type) == 0x0800) || (ntohs(eptr->ether_type) == 0x88a8 && ntohs(adptr->AD_ether_type) == 0x0800))
		{
			
			ipv4ptr = (struct IPv4Header*) (packet+EthTypeSize);

			PacketList[PacketNumber - 1].ipv4ptr = *ipv4ptr;
			PacketList[PacketNumber - 1].Layer2 = 0;

			switch(ipv4ptr->ip_p)
			{
				case 1:  //ICMP protocol
				{
					break;
				}
				case 6:  //TCP protocol
				{
					tcpptr = (struct TCPHeader *) (packet+EthTypeSize+IpSize);

					PacketList[PacketNumber - 1].tcpptr = *tcpptr;
					PacketList[PacketNumber - 1].Layer3 = 1;

					break;
				}
				case 17: //UDP protocol
				{
					udpptr = (struct UDPHeader*) (packet+EthTypeSize+IpSize);

					PacketList[PacketNumber - 1].udpptr = *udpptr;
					PacketList[PacketNumber - 1].Layer3 = 2;  

					break;
				}
				case 58:  //ICMPv6
				{
					break;
				}
				default:  //Nor TCP nor UDP nor ICMPv4 nor ICMPv6
				{
					break;
				}
			}		}

		if (ntohs(eptr->ether_type) == 0x86DD || (ntohs(eptr->ether_type) == 0x8100 && ntohs(qptr->Q_ether_type) == 0x86DD) || (ntohs(eptr->ether_type) == 0x88a8 && ntohs(adptr->AD_ether_type) == 0x86DD))
		{
			ipv6ptr = (struct IPv6Header*) (packet+EthTypeSize);

			PacketList[PacketNumber - 1].ipv6ptr = *ipv6ptr;
			PacketList[PacketNumber - 1].Layer2 = 1;

			switch (ipv6ptr->ip6_ctlun.ip6_un1.ip6_un1_nxt)
			{
				case 1:  //ICMP protocol
				{
					break;
				}
				case 6:  //TCP protocol
				{
					tcpptr = (struct TCPHeader *) (packet+EthTypeSize+IpSize);

					PacketList[PacketNumber - 1].tcpptr = *tcpptr;
					PacketList[PacketNumber - 1].Layer3 = 1;

					break;
				}
				case 17: //UDP protocol
				{
					udpptr = (struct UDPHeader *) (packet+EthTypeSize+IpSize);

					PacketList[PacketNumber - 1].udpptr = *udpptr;
					PacketList[PacketNumber - 1].Layer3 = 2;  

					break;
				}
				case 58:  //ICMPv6
				{
					break;
				}
				default:  //Nor TCP nor UDP nor ICMPv4 nor ICMPv6
				{
					break;
				}
			}
		}

		PacketList[PacketNumber - 1].ts = header.ts;
		PacketList[PacketNumber - 1].len = header.len;
		PacketList[PacketNumber - 1].PacketNumber = PacketNumber;
	}

	//razeni
	switch (svalue)
	{
		case -2:
		{
			for (int i = 0; i < NumberOfPackets; i++)
			{
				printPacket(PacketList.at(i));
			}
			break;
		}
		case 0:  //packets
		{
			break;
		}
		case 1:  //bytes
		{
			int Temp;
			for (int i = 0; i < NumberOfPackets; i++)
			{
				Temp = GetMax(PacketList, NumberOfPackets);
				printPacket(PacketList.at(Temp));
				PacketList[Temp].len = 0;
				//uvolneni zdroju
			}
			break;
		}
		default:
		{
			break;
		}
	}

	pcap_close(handle);

	return 0;
}