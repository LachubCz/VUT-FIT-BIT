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

struct ICMPv6Header {
	u_int8_t	icmp6_type;	/* type field */
	u_int8_t	icmp6_code;	/* code field */
	u_int16_t	icmp6_cksum;	/* checksum field */
};

struct ICMPv4Header{
	u_int8_t icmp4_type;	   			/* ICMP type */
	u_int8_t icmp4_code;	   			/* ICMP code */
	u_int16_t icmp4_sum;   			/* ICMP Checksum */
	//u_int16_t id; 				/* ICMP id */
	//u_int16_t seq;				/* ICMP sequence number */
};

struct TCPHeader {
	u_short th_sport;	/* source port */
	u_short th_dport;	/* destination port */
	u_int th_seq;		/* sequence number */
	u_int th_ack;		/* acknowledgement number */
	u_char th_offx2;	/* data offset, rsvd */
	u_char th_flags;
#define TH_FIN 0x01
#define TH_SYN 0x02
#define TH_RST 0x04
#define TH_PUSH 0x08
#define TH_ACK 0x10
#define TH_URG 0x20
#define TH_ECE 0x40
#define TH_CWR 0x80
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

struct IPv6ExtHeader{
	uint8_t  ip6e_nxt;		/* next header.  */
	uint8_t  ip6e_len;
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
	struct ICMPv4Header icmpv4ptr;
	struct ICMPv6Header icmpv6ptr;
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

int GetNumberOfPackets(char *filename, char *fvalue)
{
	int counter = 0;
	pcap_t *handle; //ukazatel na soubor s pakety
	const u_char *packet;
	char errbuf[256];
	struct pcap_pkthdr header;
	struct bpf_program fp; // the compiled filter

	if ((handle = pcap_open_offline(filename,errbuf)) == NULL)
		ErrorFound(9);

	if (strcmp("", fvalue) != 0)
	{
		if (pcap_compile(handle,&fp, fvalue, 0, 0) == -1) // compile the filter
		{
			printf("chyba\n" );
		}
		if (pcap_setfilter(handle, &fp) == -1)
		{
			printf("chyba\n" );
		}
	}

	while ((packet = pcap_next(handle,&header)) != NULL)  //v header jsou hodnoty hlavicky paketu, v packetu je ukazatel na zacatek
	{
		counter++;
	}

	pcap_close(handle);
	return counter;
}

struct AggrData {
	uint8_t Aggr_shost[ETHER_ADDR_LEN];
	uint8_t Aggr_dhost[ETHER_ADDR_LEN];
   	struct in_addr Aggr_ip_src;
	struct in_addr Aggr_ip_dst;

	int IpType;

	struct in6_addr Aggr_ip6_src;
	struct in6_addr Aggr_ip6_dst;

	u_short	Aggr_sport;
	u_short	Aggr_dport;

	int NumberOfPackets;
	bpf_u_int32 len;
};

int GetMaxAggrLen(std::vector<AggrData> PacketList, int NumberOfPackets)
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

int GetMaxAggrPac(std::vector<AggrData> PacketList, int NumberOfPackets)
{
	int Max = 0;
	int Order = 0;
	int i;
	for (i = 0; i < NumberOfPackets; i++)
	{
		if (i == 0)
		{
			Max = PacketList.at(i).NumberOfPackets;
			Order = i;
		}
		if (PacketList.at(i).NumberOfPackets >= Max)
		{
			Max = PacketList.at(i).NumberOfPackets;
			Order = i;
		}
	}
	return Order;
}

void printPacketAggr(std::vector<PacketData> PacketList, int NumberOfPackets, int avalue, int svalue, unsigned int lvalue)
{
	int ItemsCount = 0;
	bool IsIn = false;
	int printed = 0;
	std::vector<AggrData> AggrPacketList(NumberOfPackets);
	switch(avalue)
	{
		case 0:
		{
			for (int i = 0; i < NumberOfPackets; i++)
			{
				switch(PacketList.at(i).Layer1)
				{
					case 0:
					{
						int e = 0;
						for (e = 0; e < ItemsCount; e++)
						{
							if (memcmp(AggrPacketList.at(e).Aggr_shost, PacketList.at(i).eptr.ether_shost, ETHER_ADDR_LEN) == 0)
							{	
								IsIn = true;

								AggrPacketList.at(e).NumberOfPackets++; 
								AggrPacketList.at(e).len += PacketList.at(i).len;

								break;
							}
							else
							{
								IsIn = false;
							}
						}
						if (IsIn == false)
						{
							memcpy(AggrPacketList.at(ItemsCount).Aggr_shost, PacketList.at(i).eptr.ether_shost, ETHER_ADDR_LEN);

							AggrPacketList.at(ItemsCount).NumberOfPackets = 1; 
							AggrPacketList.at(ItemsCount).len = PacketList.at(i).len;

							ItemsCount++;
						}

						IsIn = false;
						break;
					}
					case 1:
					{
						int e = 0;
						for (e = 0; e < ItemsCount; e++)
						{
							if (memcmp(AggrPacketList.at(e).Aggr_shost, PacketList.at(i).qptr.Q_shost, ETHER_ADDR_LEN) == 0)
							{
								IsIn = true;

								AggrPacketList.at(e).NumberOfPackets++; 
								AggrPacketList.at(e).len += PacketList.at(i).len;

								break;
							}
							else
							{
								IsIn = false;
							}
						}
						if (IsIn == false)
						{
							memcpy(AggrPacketList.at(ItemsCount).Aggr_shost, PacketList.at(i).qptr.Q_shost, ETHER_ADDR_LEN);

							AggrPacketList.at(ItemsCount).NumberOfPackets = 1; 
							AggrPacketList.at(ItemsCount).len = PacketList.at(i).len;

							ItemsCount++;
						}

						IsIn = false;
						break;
					}
					case 2:
					{
						int e = 0;
						for (e = 0; e < ItemsCount; e++)
						{
							if (memcmp(AggrPacketList.at(e).Aggr_shost, PacketList.at(i).adptr.AD_shost, ETHER_ADDR_LEN) == 0)
							{
								IsIn = true;

								AggrPacketList.at(e).NumberOfPackets++; 
								AggrPacketList.at(e).len += PacketList.at(i).len;

								break;
							}
							else
							{
								IsIn = false;
							}
						}
						if (IsIn == false)
						{
							memcpy(AggrPacketList.at(ItemsCount).Aggr_shost, PacketList.at(i).adptr.AD_shost, ETHER_ADDR_LEN);

							AggrPacketList.at(ItemsCount).NumberOfPackets = 1; 
							AggrPacketList.at(ItemsCount).len = PacketList.at(i).len;

							ItemsCount++;
						}
						
						IsIn = false;
						break;
					}
					default:
					{
						break;
					}
				}
			}

			switch(svalue)
			{
				case -2:
				{
					for (int i = 0; i < ItemsCount; i++)
					{
						if (printed < lvalue)
						{
							char MacSrc[256];
							strcpy(MacSrc, ether_ntoa((const struct ether_addr *)&AggrPacketList.at(i).Aggr_shost));
							CorrectMacAdress(MacSrc);
							printf("%s: %d %d\n", 
								MacSrc, 
								AggrPacketList.at(i).NumberOfPackets, 
								AggrPacketList.at(i).len);
							printed++;
						}
						else
						{
							break;
						}

					}
					break;
				}
				case 0:
				{
					int Temp;
					for (int i = 0; i < ItemsCount; i++)
					{
						if (printed < lvalue)
						{
							Temp = GetMaxAggrPac(AggrPacketList, ItemsCount);
							char MacSrc[256];
							strcpy(MacSrc, ether_ntoa((const struct ether_addr *)&AggrPacketList.at(Temp).Aggr_shost));
							CorrectMacAdress(MacSrc);
							printf("%s: %d %d\n", 
								MacSrc, 
								AggrPacketList.at(Temp).NumberOfPackets, 
								AggrPacketList.at(Temp).len);
							AggrPacketList[Temp].NumberOfPackets = 0;
							printed++;
						}
						else
						{
							break;
						}
					}
					break;
				}
				case 1:
				{
					int Temp;
					for (int i = 0; i < ItemsCount; i++)
					{
						if (printed < lvalue)
						{
							Temp = GetMaxAggrLen(AggrPacketList, ItemsCount);
							char MacSrc[256];
							strcpy(MacSrc, ether_ntoa((const struct ether_addr *)&AggrPacketList.at(Temp).Aggr_shost));
							CorrectMacAdress(MacSrc);
							printf("%s: %d %d\n", 
								MacSrc, 
								AggrPacketList.at(Temp).NumberOfPackets, 
								AggrPacketList.at(Temp).len);
							AggrPacketList[Temp].len = 0;
							printed++;
						}
						else
						{
							break;
						}
					}
					break;
				}
				default:
				{
					break;
				}
			}

			break;
		}
		case 1:
		{
			for (int i = 0; i < NumberOfPackets; i++)
			{
				switch(PacketList.at(i).Layer1)
				{
					case 0:
					{
						int e = 0;
						for (e = 0; e < ItemsCount; e++)
						{
							if (memcmp(AggrPacketList.at(e).Aggr_dhost, PacketList.at(i).eptr.ether_dhost, ETHER_ADDR_LEN) == 0)
							{	
								IsIn = true;

								AggrPacketList.at(e).NumberOfPackets++; 
								AggrPacketList.at(e).len += PacketList.at(i).len;
								
								break;
							}
							else
							{
								IsIn = false;
							}
						}
						if (IsIn == false)
						{
							memcpy(AggrPacketList.at(ItemsCount).Aggr_dhost, PacketList.at(i).eptr.ether_dhost, ETHER_ADDR_LEN);

							AggrPacketList.at(ItemsCount).NumberOfPackets = 1; 
							AggrPacketList.at(ItemsCount).len = PacketList.at(i).len;

							ItemsCount++;
						}

						IsIn = false;
						break;
					}
					case 1:
					{
						int e = 0;
						for (e = 0; e < ItemsCount; e++)
						{
							if (memcmp(AggrPacketList.at(e).Aggr_dhost, PacketList.at(i).qptr.Q_dhost, ETHER_ADDR_LEN) == 0)
							{
								IsIn = true;

								AggrPacketList.at(e).NumberOfPackets++; 
								AggrPacketList.at(e).len += PacketList.at(i).len;

								break;
							}
							else
							{
								IsIn = false;
							}
						}
						if (IsIn == false)
						{
							memcpy(AggrPacketList.at(ItemsCount).Aggr_dhost, PacketList.at(i).qptr.Q_dhost, ETHER_ADDR_LEN);

							AggrPacketList.at(ItemsCount).NumberOfPackets = 1; 
							AggrPacketList.at(ItemsCount).len = PacketList.at(i).len;

							ItemsCount++;
						}

						IsIn = false;
						break;
					}
					case 2:
					{
						int e = 0;
						for (e = 0; e < ItemsCount; e++)
						{
							if (memcmp(AggrPacketList.at(e).Aggr_dhost, PacketList.at(i).adptr.AD_dhost, ETHER_ADDR_LEN) == 0)
							{
								IsIn = true;

								AggrPacketList.at(e).NumberOfPackets++; 
								AggrPacketList.at(e).len += PacketList.at(i).len;

								break;
							}
							else
							{
								IsIn = false;
							}
						}
						if (IsIn == false)
						{
							memcpy(AggrPacketList.at(ItemsCount).Aggr_dhost, PacketList.at(i).adptr.AD_dhost, ETHER_ADDR_LEN);

							AggrPacketList.at(ItemsCount).NumberOfPackets = 1; 
							AggrPacketList.at(ItemsCount).len = PacketList.at(i).len;

							ItemsCount++;
						}
						
						IsIn = false;
						break;
					}
					default:
					{
						break;
					}
				}
			}

			switch(svalue)
			{
				case -2:
				{
					for (int i = 0; i < ItemsCount; i++)
					{
						if (printed < lvalue)
						{
							char MacDst[256];
							strcpy(MacDst, ether_ntoa((const struct ether_addr *)&AggrPacketList.at(i).Aggr_dhost));
							CorrectMacAdress(MacDst);
							printf("%s: %d %d\n", 
								MacDst, 
								AggrPacketList.at(i).NumberOfPackets, 
								AggrPacketList.at(i).len);
							printed++;
						}
						else
						{
							break;
						}
					}
					break;
				}
				case 0:
				{
					int Temp;
					for (int i = 0; i < ItemsCount; i++)
					{
						if (printed < lvalue)
						{
							Temp = GetMaxAggrPac(AggrPacketList, ItemsCount);
							char MacDst[256];
							strcpy(MacDst, ether_ntoa((const struct ether_addr *)&AggrPacketList.at(Temp).Aggr_dhost));
							CorrectMacAdress(MacDst);
							printf("%s: %d %d\n", 
								MacDst, 
								AggrPacketList.at(Temp).NumberOfPackets, 
								AggrPacketList.at(Temp).len);
							AggrPacketList[Temp].NumberOfPackets = 0;
							printed++;
						}
						else
						{
							break;
						}
					}
					break;
				}
				case 1:
				{
					int Temp;
					for (int i = 0; i < ItemsCount; i++)
					{
						if (printed < lvalue)
						{
							Temp = GetMaxAggrLen(AggrPacketList, ItemsCount);
							char MacDst[256];
							strcpy(MacDst, ether_ntoa((const struct ether_addr *)&AggrPacketList.at(Temp).Aggr_dhost));
							CorrectMacAdress(MacDst);
							printf("%s: %d %d\n", 
								MacDst, 
								AggrPacketList.at(Temp).NumberOfPackets, 
								AggrPacketList.at(Temp).len);
							AggrPacketList[Temp].len = 0;
							printed++;
						}
						else
						{
							break;
						}
					}
					break;
				}
				default:
				{
					break;
				}
			}
			break;
		} 
		case 2:
		{
			for (int i = 0; i < NumberOfPackets; i++)
			{
				switch (PacketList.at(i).Layer2)
				{
					case 0:
					{
						char IPv4Src[256];
						strcpy(IPv4Src,inet_ntoa(PacketList.at(i).ipv4ptr.ip_src));

						int e = 0;
						for (e = 0; e < ItemsCount; e++)
						{
							if (strcmp(inet_ntoa(AggrPacketList.at(e).Aggr_ip_src), IPv4Src) == 0)
							{	
								IsIn = true;

								AggrPacketList.at(e).NumberOfPackets++; 
								AggrPacketList.at(e).len += PacketList.at(i).len;

								break;
							}
							else
							{
								IsIn = false;
							}
							
						}
						
						if (IsIn == false)
						{
							AggrPacketList.at(ItemsCount).Aggr_ip_src = PacketList.at(i).ipv4ptr.ip_src;

							AggrPacketList.at(ItemsCount).IpType = 0;
							AggrPacketList.at(ItemsCount).NumberOfPackets = 1; 
							AggrPacketList.at(ItemsCount).len = PacketList.at(i).len;

							ItemsCount++;
						}
						
						IsIn = false;
						break;
					}
					case 1:
					{
						char IPv6Src[256];
						char printAbleIPv6src[INET6_ADDRSTRLEN];
						strcpy(IPv6Src,inet_ntop(AF_INET6, &(PacketList.at(i).ipv6ptr.ip6_src), printAbleIPv6src, INET6_ADDRSTRLEN));

						int e = 0;
						for (e = 0; e < ItemsCount; e++)
						{
							if (strcmp(inet_ntop(AF_INET6, &(AggrPacketList.at(e).Aggr_ip6_src), printAbleIPv6src, INET6_ADDRSTRLEN), IPv6Src) == 0)
							{	
								IsIn = true;

								AggrPacketList.at(e).IpType = 1;
								AggrPacketList.at(e).NumberOfPackets++; 
								AggrPacketList.at(e).len += PacketList.at(i).len;

								break;
							}
							else
							{
								IsIn = false;
							}
							
						}
						
						if (IsIn == false)
						{
							AggrPacketList.at(ItemsCount).Aggr_ip6_src = PacketList.at(i).ipv6ptr.ip6_src;

							AggrPacketList.at(ItemsCount).IpType = 1;
							AggrPacketList.at(ItemsCount).NumberOfPackets = 1; 
							AggrPacketList.at(ItemsCount).len = PacketList.at(i).len;

							ItemsCount++;
						}
						
						IsIn = false;
						break;
					}
					default:
					{
						break;
					}
				}
			}

			switch(svalue)
			{
				case -2:
				{
					for (int i = 0; i < ItemsCount; i++)
					{
						if (printed < lvalue)
						{
							if (AggrPacketList.at(i).IpType == 0)
							{
								printf("%s: %d %d\n", 
									inet_ntoa(AggrPacketList.at(i).Aggr_ip_src), 
									AggrPacketList.at(i).NumberOfPackets, 
									AggrPacketList.at(i).len);
							}
							else
							{
								char printAbleIPv6src[INET6_ADDRSTRLEN];
								printf("%s: %d %d\n", 
									inet_ntop(AF_INET6, &(AggrPacketList.at(i).Aggr_ip6_src), printAbleIPv6src, INET6_ADDRSTRLEN), 
									AggrPacketList.at(i).NumberOfPackets, 
									AggrPacketList.at(i).len);
							}
							printed++;
						}
						else
						{
							break;
						}
					}
					break;
				}
				case 0:
				{
					int Temp;
					for (int i = 0; i < ItemsCount; i++)
					{
						if (printed < lvalue)
						{
							Temp = GetMaxAggrPac(AggrPacketList, ItemsCount);

							if (AggrPacketList.at(Temp).IpType == 0)
							{
								printf("%s: %d %d\n", 
									inet_ntoa(AggrPacketList.at(Temp).Aggr_ip_src), 
									AggrPacketList.at(Temp).NumberOfPackets, 
									AggrPacketList.at(Temp).len);
							}
							else
							{
								char printAbleIPv6src[INET6_ADDRSTRLEN];
								printf("%s: %d %d\n", 
									inet_ntop(AF_INET6, &(AggrPacketList.at(Temp).Aggr_ip6_src), printAbleIPv6src, INET6_ADDRSTRLEN), 
									AggrPacketList.at(Temp).NumberOfPackets, 
									AggrPacketList.at(Temp).len);
							}

							AggrPacketList[Temp].NumberOfPackets = 0;
							printed++;
						}
						else
						{
							break;
						}
					}
					break;
				}
				case 1:
				{
					int Temp;
					for (int i = 0; i < ItemsCount; i++)
					{
						if (printed < lvalue)
						{
							Temp = GetMaxAggrLen(AggrPacketList, ItemsCount);

							if (AggrPacketList.at(Temp).IpType == 0)
							{
								printf("%s: %d %d\n", 
									inet_ntoa(AggrPacketList.at(Temp).Aggr_ip_src), 
									AggrPacketList.at(Temp).NumberOfPackets, 
									AggrPacketList.at(Temp).len);
							}
							else
							{
								char printAbleIPv6src[INET6_ADDRSTRLEN];
								printf("%s: %d %d\n", 
									inet_ntop(AF_INET6, &(AggrPacketList.at(Temp).Aggr_ip6_src), printAbleIPv6src, INET6_ADDRSTRLEN), 
									AggrPacketList.at(Temp).NumberOfPackets, 
									AggrPacketList.at(Temp).len);
							}

							AggrPacketList[Temp].len = 0;
							printed++;
						}
						else
						{
							break;
						}
					}
					break;
				}
				default:
				{
					break;
				}
			}

			break;
		}
		case 3:
		{
			for (int i = 0; i < NumberOfPackets; i++)
			{
				switch (PacketList.at(i).Layer2)
				{
					case 0:
					{
						char IPv4Dst[256];
						strcpy(IPv4Dst,inet_ntoa(PacketList.at(i).ipv4ptr.ip_dst));

						int e = 0;
						for (e = 0; e < ItemsCount; e++)
						{
							if (strcmp(inet_ntoa(AggrPacketList.at(e).Aggr_ip_dst), IPv4Dst) == 0)
							{	
								IsIn = true;

								AggrPacketList.at(e).NumberOfPackets++; 
								AggrPacketList.at(e).len += PacketList.at(i).len;

								break;
							}
							else
							{
								IsIn = false;
							}
							
						}
						
						if (IsIn == false)
						{
							AggrPacketList.at(e).Aggr_ip_dst = PacketList.at(i).ipv4ptr.ip_dst;

							AggrPacketList.at(ItemsCount).IpType = 0;
							AggrPacketList.at(ItemsCount).NumberOfPackets = 1; 
							AggrPacketList.at(ItemsCount).len = PacketList.at(i).len;

							ItemsCount++;
						}
						
						IsIn = false;
						break;
					}
					case 1:
					{
						char IPv6Dst[256];
						char printAbleIPv6dst[INET6_ADDRSTRLEN];

						strcpy(IPv6Dst,inet_ntop(AF_INET6, &(PacketList.at(i).ipv6ptr.ip6_dst), printAbleIPv6dst, INET6_ADDRSTRLEN));
						int e = 0;
						for (e = 0; e < ItemsCount; e++)
						{
							if (strcmp(inet_ntop(AF_INET6, &(AggrPacketList.at(e).Aggr_ip6_dst), printAbleIPv6dst, INET6_ADDRSTRLEN), IPv6Dst) == 0)
							{	
								IsIn = true;

								AggrPacketList.at(e).IpType = 1;
								AggrPacketList.at(e).NumberOfPackets++; 
								AggrPacketList.at(e).len += PacketList.at(i).len;

								break;
							}
							else
							{
								IsIn = false;
							}
							
						}
						
						if (IsIn == false)
						{
							AggrPacketList.at(ItemsCount).Aggr_ip6_dst = PacketList.at(i).ipv6ptr.ip6_dst;

							AggrPacketList.at(ItemsCount).IpType = 1;
							AggrPacketList.at(ItemsCount).NumberOfPackets = 1; 
							AggrPacketList.at(ItemsCount).len = PacketList.at(i).len;

							ItemsCount++;
						}
						
						IsIn = false;
						break;
					}
					default:
					{
						break;
					}
				}
			}

			switch(svalue)
			{
				case -2:
				{
					for (int i = 0; i < ItemsCount; i++)
					{
						if (printed < lvalue)
						{
							if (AggrPacketList.at(i).IpType == 0)
							{
								printf("%s: %d %d\n", 
									inet_ntoa(AggrPacketList.at(i).Aggr_ip_dst), 
									AggrPacketList.at(i).NumberOfPackets, 
									AggrPacketList.at(i).len);
							}
							else
							{
								char printAbleIPv6dst[INET6_ADDRSTRLEN];
								printf("%s: %d %d\n", 
									inet_ntop(AF_INET6, &(AggrPacketList.at(i).Aggr_ip6_dst), printAbleIPv6dst, INET6_ADDRSTRLEN), 
									AggrPacketList.at(i).NumberOfPackets, 
									AggrPacketList.at(i).len);
							}
							printed++;
						}
						else
						{
							break;
						}
					}
					break;
				}
				case 0:
				{
					int Temp;
					for (int i = 0; i < ItemsCount; i++)
					{
						if (printed < lvalue)
						{
							Temp = GetMaxAggrPac(AggrPacketList, ItemsCount);

							if (AggrPacketList.at(Temp).IpType == 0)
							{
								printf("%s: %d %d\n", 
									inet_ntoa(AggrPacketList.at(Temp).Aggr_ip_dst), 
									AggrPacketList.at(Temp).NumberOfPackets, 
									AggrPacketList.at(Temp).len);
							}
							else
							{
								char printAbleIPv6dst[INET6_ADDRSTRLEN];
								printf("%s: %d %d\n", 
									inet_ntop(AF_INET6, &(AggrPacketList.at(Temp).Aggr_ip6_dst), printAbleIPv6dst, INET6_ADDRSTRLEN), 
									AggrPacketList.at(Temp).NumberOfPackets, 
									AggrPacketList.at(Temp).len);
							}

							AggrPacketList[Temp].NumberOfPackets = 0;
							printed++;
						}
						else
						{
							break;
						}
					}
					break;
				}
				case 1:
				{
					int Temp;
					for (int i = 0; i < ItemsCount; i++)
					{
						if (printed < lvalue)
						{
							Temp = GetMaxAggrLen(AggrPacketList, ItemsCount);

							if (AggrPacketList.at(Temp).IpType == 0)
							{
								printf("%s: %d %d\n", 
									inet_ntoa(AggrPacketList.at(Temp).Aggr_ip_dst), 
									AggrPacketList.at(Temp).NumberOfPackets, 
									AggrPacketList.at(Temp).len);
							}
							else
							{
								char printAbleIPv6dst[INET6_ADDRSTRLEN];
								printf("%s: %d %d\n", 
									inet_ntop(AF_INET6, &(AggrPacketList.at(Temp).Aggr_ip6_dst), printAbleIPv6dst, INET6_ADDRSTRLEN), 
									AggrPacketList.at(Temp).NumberOfPackets, 
									AggrPacketList.at(Temp).len);
							}

							AggrPacketList[Temp].len = 0;
							printed++;
						}
						else
						{
							break;
						}
					}
					break;
				}
				default:
				{
					break;
				}
			}

			break;
		}
		case 4:
		{
			for (int i = 0; i < NumberOfPackets; i++)
			{
				switch (PacketList.at(i).Layer3)
				{
					case 1:
					{
						int e = 0;

						for (e = 0; e < ItemsCount; e++)
						{
							if (ntohs(AggrPacketList.at(e).Aggr_sport) == ntohs(PacketList.at(i).tcpptr.th_sport))
							{	
								IsIn = true;

								AggrPacketList.at(e).NumberOfPackets++; 
								AggrPacketList.at(e).len += PacketList.at(i).len;
										
								break;
							}
							else
							{
								IsIn = false;
							}
						}

						if (IsIn == false)
						{
							AggrPacketList.at(ItemsCount).Aggr_sport = PacketList.at(i).tcpptr.th_sport;

							AggrPacketList.at(ItemsCount).NumberOfPackets = 1; 
							AggrPacketList.at(ItemsCount).len = PacketList.at(i).len;

							ItemsCount++;
						}
						
						IsIn = false;
						break;
					}
					case 2:
					{
						int e = 0;

						for (e = 0; e < ItemsCount; e++)
						{
							if (ntohs(AggrPacketList.at(e).Aggr_sport) == ntohs(PacketList.at(i).udpptr.uh_sport))
							{	
								IsIn = true;

								AggrPacketList.at(e).NumberOfPackets++; 
								AggrPacketList.at(e).len += PacketList.at(i).len;

								break;
							}
							else
							{
								IsIn = false;
							}
						}

						if (IsIn == false)
						{
							AggrPacketList.at(ItemsCount).Aggr_sport = PacketList.at(i).udpptr.uh_sport;

							AggrPacketList.at(ItemsCount).NumberOfPackets = 1; 
							AggrPacketList.at(ItemsCount).len = PacketList.at(i).len;

							ItemsCount++;
						}
						
						IsIn = false;
						break;
					}
					default:
					{
						break;
					}
				}
			}

			switch(svalue)
			{
				case -2:
				{
					for (int i = 0; i < ItemsCount; i++)
					{
						if (printed < lvalue)
						{
							printf("%d: %d %d\n", 
								ntohs(AggrPacketList.at(i).Aggr_sport), 
								AggrPacketList.at(i).NumberOfPackets, 
								AggrPacketList.at(i).len);
							printed++;
						}
						else
						{
							break;
						}
					}
					break;
				}
				case 0:
				{
					int Temp;
					for (int i = 0; i < ItemsCount; i++)
					{
						if (printed < lvalue)
						{
							Temp = GetMaxAggrPac(AggrPacketList, ItemsCount);

							printf("%d: %d %d\n", 
								ntohs(AggrPacketList.at(Temp).Aggr_sport), 
								AggrPacketList.at(Temp).NumberOfPackets, 
								AggrPacketList.at(Temp).len);

							AggrPacketList[Temp].NumberOfPackets = 0;
							printed++;
						}
						else
						{
							break;
						}
					}
					break;
				}
				case 1:
				{
					int Temp;
					for (int i = 0; i < ItemsCount; i++)
					{
						if (printed < lvalue)
						{
							Temp = GetMaxAggrLen(AggrPacketList, ItemsCount);

							printf("%d: %d %d\n", 
								ntohs(AggrPacketList.at(Temp).Aggr_sport), 
								AggrPacketList.at(Temp).NumberOfPackets, 
								AggrPacketList.at(Temp).len);

							AggrPacketList[Temp].len = 0;
							printed++;
						}
						else
						{
							break;
						}
					}
					break;
				}
				default:
				{
					break;
				}
			}

			break;
		}
		case 5:
		{
			for (int i = 0; i < NumberOfPackets; i++)
			{
				switch (PacketList.at(i).Layer3)
				{
					case 1:
					{
						int e = 0;

						for (e = 0; e < ItemsCount; e++)
						{
							if (ntohs(AggrPacketList.at(e).Aggr_dport) == ntohs(PacketList.at(i).tcpptr.th_dport))
							{	
								IsIn = true;

								AggrPacketList.at(e).NumberOfPackets++; 
								AggrPacketList.at(e).len += PacketList.at(i).len;
										
								break;
							}
							else
							{
								IsIn = false;
							}
						}

						if (IsIn == false)
						{
							AggrPacketList.at(ItemsCount).Aggr_dport = PacketList.at(i).tcpptr.th_dport;

							AggrPacketList.at(ItemsCount).NumberOfPackets = 1; 
							AggrPacketList.at(ItemsCount).len = PacketList.at(i).len;

							ItemsCount++;
						}
						
						IsIn = false;
						break;
					}
					case 2:
					{
						int e = 0;

						for (e = 0; e < ItemsCount; e++)
						{
							if (ntohs(AggrPacketList.at(e).Aggr_dport) == ntohs(PacketList.at(i).udpptr.uh_dport))
							{	
								IsIn = true;

								AggrPacketList.at(e).NumberOfPackets++; 
								AggrPacketList.at(e).len += PacketList.at(i).len;

								break;
							}
							else
							{
								IsIn = false;
							}
						}

						if (IsIn == false)
						{
							AggrPacketList.at(ItemsCount).Aggr_dport = PacketList.at(i).udpptr.uh_dport;

							AggrPacketList.at(ItemsCount).NumberOfPackets = 1; 
							AggrPacketList.at(ItemsCount).len = PacketList.at(i).len;

							ItemsCount++;
						}
						
						IsIn = false;
						break;
					}
					default:
					{
						break;
					}
				}
			}

			switch(svalue)
			{
				case -2:
				{
					for (int i = 0; i < ItemsCount; i++)
					{
						if (printed < lvalue)
						{
							printf("%d: %d %d\n", 
								ntohs(AggrPacketList.at(i).Aggr_dport), 
								AggrPacketList.at(i).NumberOfPackets, 
								AggrPacketList.at(i).len);
							printed++;
						}
						else
						{
							break;
						}
					}
					break;
				}
				case 0:
				{
					int Temp;
					for (int i = 0; i < ItemsCount; i++)
					{
						if (printed < lvalue)
						{
							Temp = GetMaxAggrPac(AggrPacketList, ItemsCount);

							printf("%d: %d %d\n", 
								ntohs(AggrPacketList.at(Temp).Aggr_dport), 
								AggrPacketList.at(Temp).NumberOfPackets, 
								AggrPacketList.at(Temp).len);

							AggrPacketList[Temp].NumberOfPackets = 0;
							printed++;
						}
						else
						{
							break;
						}
					}
					break;
				}
				case 1:
				{
					int Temp;
					for (int i = 0; i < ItemsCount; i++)
					{
						if (printed < lvalue)
						{
							Temp = GetMaxAggrLen(AggrPacketList, ItemsCount);

							printf("%d: %d %d\n", 
								ntohs(AggrPacketList.at(Temp).Aggr_dport), 
								AggrPacketList.at(Temp).NumberOfPackets, 
								AggrPacketList.at(Temp).len);

							AggrPacketList[Temp].len = 0;
							printed++;
						}
						else
						{
							break;
						}
					}
					break;
				}
				default:
				{
					break;
				}
			}

			break;
		}
		default:
		{
			break;
		}
	}	
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
				ntohs(PacketList.qptr.Q_tpid2 & 0xFFF)); //tisknu spatne nhtons
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
				ntohs(PacketList.adptr.AD_tci )& 0xFFF,
				ntohs(PacketList.adptr.AD_tci2 )& 0xFFF);
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
			printf("ICMPv4: %u %u ",
				PacketList.icmpv4ptr.icmp4_type,
				PacketList.icmpv4ptr.icmp4_code);
			switch(PacketList.icmpv4ptr.icmp4_type)
			{
				case 0:  //Echo Reply
				{
					printf("echo reply\n");
					break;
				}
				case 3:  //Destination Unreachable
				{
					switch(PacketList.icmpv4ptr.icmp4_code)
					{
						case 0:  //net unreachable
						{
							printf("destination unreachable net unreachable\n");
							break;
						}
						case 1:  //host unreachable
						{
							printf("destination unreachable host unreachable\n");
							break;
						}
						case 2:  //protocol unreachable
						{
							printf("destination unreachable protocol unreachable\n");
							break;
						}
						case 3:  //port unreachable
						{
							printf("destination unreachable port unreachable\n");
							break;
						}
						case 4:  //fragmentation needed and DF set
						{
							printf("destination unreachable fragmentation needed and DF set\n");
							break;
						}
						case 5:  //source route failed
						{
							printf("destination unreachable source route failed\n");
							break;
						}
						default:
						{
							break;
						}
					}
					break;
				}
				case 4:  //Source Quench (Deprecated)
				{
					printf("source quench\n");
					break;
				}
				case 5:  //Redirect
				{
					switch(PacketList.icmpv4ptr.icmp4_code)
					{
						case 0:  //Redirect datagrams for the Network
						{
							printf("redirect redirect datagrams for the network\n");
							break;
						}
						case 1:  //Redirect datagrams for the Host
						{
							printf("redirect redirect datagrams for the host\n");
							break;
						}
						case 2:  //Redirect datagrams for the Type of Service and Network
						{
							printf("redirect redirect datagrams for the type of service and network\n");
							break;
						}
						case 3:  //Redirect datagrams for the Type of Service and Host
						{
							printf("redirect redirect datagrams for the type of service and host\n");
							break;
						}
						default:
						{
							break;
						}
					}
					break;
				}
				case 8:  //Echo
				{
					printf("echo\n");
					break;
				}
				case 11:  //Time Exceeded
				{
					switch(PacketList.icmpv4ptr.icmp4_code)
					{
						case 0:  //time to live exceeded in transit
						{
							printf("time exceeded time to live exceeded in transit\n");
							break;
						}
						case 1:  //fragment reassembly time exceeded
						{
							printf("time exceeded fragment reassembly time exceeded\n");
							break;
						}
						default:
						{
							break;
						}
					}
					break;
				}
				case 12:  //Parameter Problem
				{
					printf("parameter problem pointer indicates the error\n");
					break;
				}
				case 13:  //Timestamp
				{
					printf("timestamp\n");
					break;
				}
				case 14:  //Timestamp Reply
				{
					printf("timestamp reply\n");
					break;
				}
				case 15:  //Information Request (Deprecated)
				{
					printf("information request\n");
					break;
				}
				case 16:  //Information Reply (Deprecated)
				{
					printf("information reply\n");
					break;
				}
				default:
				{
					break;
				}
			} 
			break;
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
			printf("ICMPv6: %u %u ",
				PacketList.icmpv6ptr.icmp6_type,
				PacketList.icmpv6ptr.icmp6_code);
			switch(PacketList.icmpv6ptr.icmp6_type)
			{
				case 1:  //Destination Unreachable
				{
					switch(PacketList.icmpv6ptr.icmp6_code)
					{
						case 0:  //No route to destination
						{
							printf("destination unreachable no route to destination\n");
							break;
						}
						case 1:  //Communication with destination administratively prohibited
						{
							printf("destination unreachable communication with destination administratively prohibited\n");
							break;
						}
						case 2:  //Beyond scope of source address
						{
							printf("destination unreachable beyond scope of source address\n");
							break;
						}
						case 3:  //Address unreachable
						{
							printf("destination unreachable address unreachable\n");
							break;
						}
						case 4:  //Port unreachable
						{
							printf("destination unreachable port unreachable\n");
							break;
						}
						case 5:  //Source address failed ingress/egress policy
						{
							printf("destination unreachable source address failed ingress/egress policy\n");
							break;
						}
						case 6:  //Reject route to destination
						{
							printf("destination unreachable reject route to destination\n");
							break;
						}
						default:
						{
							break;
						}
					}
					break;
				}
				case 2:  //Packet Too Big
				{
					printf("packet too big");
					break;
				}
				case 3:  //Time Exceeded
				{
					switch(PacketList.icmpv6ptr.icmp6_code)
					{
						case 0:  //hop limit exceeded in transit
						{
							printf("time exceeded hop limit exceeded in transit\n");
							break;
						}
						case 1:  //fragment reassembly time exceeded
						{
							printf("time exceeded fragment reassembly time exceeded\n");
							break;
						}
						default:
						{
							break;
						}
					}
					break;
				}
				case 4:  //Parameter Problem
				{
					switch(PacketList.icmpv6ptr.icmp6_code)
					{
						case 0:  //Erroneous header field encountered
						{
							printf("parameter problem erroneous header field encountered\n");
							break;
						}
						case 1:  //Unrecognized Next Header type encountered
						{
							printf("parameter problem unrecognized next header type encountered\n");
							break;
						}
						case 2:  //Unrecognized IPv6 option encountered
						{
							printf("parameter problem unrecognized ipv6 option encountered\n");
							break;
						}
						default:
						{
							break;
						}
					}
					break;
				}
				case 128:  //Echo Request
				{
					printf("echo request\n");
					break;
				}
				case 129:  //Echo Reply
				{
					printf("echo reply\n");
					break;
				}
				default:
				{
					break;
				}
			} 
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

	struct Hole{
		u_int32_t first;
		u_int32_t last;
	};

	struct PacketToReassemble{
		struct in_addr ip_src; 
		struct in_addr ip_dst;  
		u_short ip_id;
		std::vector<Hole> Holes;
	};

	int GetNCPacketNumber(std::vector<PacketToReassemble> NCPackets, struct in_addr ip_src, struct in_addr ip_dst, u_short ip_id)
	{
		for (int i = 0; i < NCPackets.size(); ++i)
		{
			if (NCPackets.at(i).ip_src.s_addr == ip_src.s_addr && NCPackets.at(i).ip_dst.s_addr == ip_dst.s_addr && NCPackets.at(i).ip_id == ip_id)
			{
				return i;
			}
		}
		return -1;
	}

int main (int argc, char *argv[])
{
	bool hflag = false;
	int c;
	int index;
	unsigned int lvalue = 4294967295;
	int avalue = -2;
	int svalue = -2;
	char fvalue[256] = "";
	char filename[256] = "";
	unsigned int printed = 0;
	
	opterr = 0;

	while ((c = getopt (argc, argv, "ha:s:l:f:")) != -1)
	switch (c)
	{
		case 'h':
			hflag = true;
			break;
		case 'a':
			if (strcmp(optarg, "srcmac") == 0)
			{
				avalue = 0;
			}
			if (strcmp(optarg, "dstmac") == 0)
			{
				avalue = 1;
			}
			if (strcmp(optarg, "srcip") == 0)
			{
				avalue = 2;
			}
			if (strcmp(optarg, "dstip") == 0)
			{
				avalue = 3;
			}
			if (strcmp(optarg, "srcport") == 0)
			{
				avalue = 4;
			}
			if (strcmp(optarg, "dstport") == 0)
			{
				avalue = 5;
			}
			//strcpy(avalue, optarg);
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
		strcpy(filename, argv[index]);
		//ErrorFound(7);
	}

	if (hflag == true && avalue == -2 && svalue == -2 && strcmp(fvalue, "") == 0 && lvalue == -2)
	{
		PrintHelp();
	}
	else
	{
		if (hflag == true && (avalue != -2 || svalue != -2 || strcmp(fvalue, "") != 0 || lvalue != -2))
		{
			ErrorFound(8);
		}
	}

	//printf("%s\n", filename);
	//printf ("\nfilename = %s, hflag = %d, avalue = %d, svalue = %d, lvalue = %d, fvalue = %s\n\n", filename, hflag, avalue, svalue, lvalue, fvalue);

	pcap_t *handle; //ukazatel na soubor s pakety
	const u_char *packet;
	char errbuf[256];
	struct pcap_pkthdr header;
	struct bpf_program fp; // the compiled filter

	//nepopsane struktury
  	struct ETHHeader *eptr;
  	struct ADHeader *adptr;
	struct QHeader *qptr;
	struct IPv4Header *ipv4ptr;  //struktura pro IPv4 hlavicku
	struct IPv6Header *ipv6ptr;  //struktura pro IPv6 hlavicku
	struct IPv6ExtHeader *ext;
	struct ICMPv4Header *icmpv4ptr;
	struct ICMPv6Header *icmpv6ptr;
	const struct TCPHeader *tcpptr;	// pointer to the beginning of TCP header
	const struct UDPHeader *udpptr;	// pointer to the beginning of UDP header
	std::vector<PacketToReassemble> NCPackets;

	int PacketNumber = 0; 

	if ((handle = pcap_open_offline(filename,errbuf)) == NULL)
		ErrorFound(9);
	
	int NumberOfPackets = GetNumberOfPackets(filename, fvalue);
	
	std::vector<PacketData>  PacketList(NumberOfPackets);

	if (strcmp("", fvalue) != 0)
	{
		if (pcap_compile(handle,&fp,fvalue,0,0) == -1) // compile the filter
		{
			printf("chyba\n" );
		}
		if (pcap_setfilter(handle,&fp) == -1)
		{
			printf("chyba\n" );
		}
	}

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
				ipv4ptr = (struct IPv4Header*) (packet+EthTypeSize);
				IpSize = ipv4ptr->ip_hl*4; 

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
						ipv4ptr = (struct IPv4Header*) (packet+EthTypeSize);
						IpSize = ipv4ptr->ip_hl*4;

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
						ipv4ptr = (struct IPv4Header*) (packet+EthTypeSize);
						IpSize = ipv4ptr->ip_hl*4;

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
			/*
		struct IPv4Header {
			#if BYTE_ORDER == LITTLE_ENDIAN
				u_char  ip_hl:4,				/* header length 
				ip_v:4;				 /* version 
			#endif
			#if BYTE_ORDER == BIG_ENDIAN
				u_char  ip_v:4,				 /* version 
				ip_hl:4;				/* header length 
			#endif
			u_char ip_tos;			/* type of service 
			u_short ip_len;			/* total length 
			u_short ip_id;			/* identification 
			u_short ip_off;			/* fragment offset field 
			#define	IP_RF 0x8000			/* reserved fragment flag 
			#define	IP_DF 0x4000			/* dont fragment flag 
			#define	IP_MF 0x2000			/* more fragments flag 
			#define	IP_OFFMASK 0x1fff		/* mask for fragmenting bits 
			/*(flags_fragmentOffset & 0x4000) != 0
			u_char ip_ttl;			/* time to live 
			u_char ip_p;			/* protocol 
			u_short ip_sum;			/* checksum 
			struct in_addr ip_src; /* source address
			struct in_addr ip_dst; /* source and dest address 
		};*/
			//printf("%d: %u %u\n", PacketNumber, (ntohs(ipv4ptr->ip_off) & IP_OFFMASK) * 8, ntohs(ipv4ptr->ip_off) & IP_MF);



			if ((ntohs(ipv4ptr->ip_off) & IP_MF) != 0)
			{
				if (true)//(PacketNumber < 3)
				{
					if (GetNCPacketNumber(NCPackets, ipv4ptr->ip_src, ipv4ptr->ip_dst, ipv4ptr->ip_id) == -1)
					{
						printf("jsem hir\n");
						struct PacketToReassemble pkt;
						pkt.ip_src = ipv4ptr->ip_src;
						pkt.ip_dst = ipv4ptr->ip_dst;
						pkt.ip_id = ipv4ptr->ip_id;
						NCPackets.push_back(pkt);
					}
					printf("%d\n", NCPackets.size());
				}
			}
			else
			{
				PacketList[PacketNumber - 1].ipv4ptr = *ipv4ptr;
				PacketList[PacketNumber - 1].Layer2 = 0;
				switch(ipv4ptr->ip_p)
				{
					case 1:  //ICMP protocol
					{
						icmpv4ptr = (struct ICMPv4Header *) (packet+EthTypeSize+IpSize);

						PacketList[PacketNumber - 1].icmpv4ptr = *icmpv4ptr;
						PacketList[PacketNumber - 1].Layer3 = 0;
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
					default:  //Nor TCP nor UDP nor ICMPv4 nor ICMPv6
					{
						PacketList[PacketNumber - 1].Layer3 = 3;
						break;
					}
				}
			}		
		}

		if (ntohs(eptr->ether_type) == 0x86DD || (ntohs(eptr->ether_type) == 0x8100 && ntohs(qptr->Q_ether_type) == 0x86DD) || (ntohs(eptr->ether_type) == 0x88a8 && ntohs(adptr->AD_ether_type) == 0x86DD))
		{
			ipv6ptr = (struct IPv6Header*) (packet+EthTypeSize);

			PacketList[PacketNumber - 1].ipv6ptr = *ipv6ptr;
			PacketList[PacketNumber - 1].Layer2 = 1;

			u_int8_t  ip6_un1_nxt = ipv6ptr->ip6_ctlun.ip6_un1.ip6_un1_nxt;

			bool NextHeaders = true;
			int extraLenght = 0;

			while (NextHeaders)
			{
				switch (ip6_un1_nxt)
				{
					case 0:  //IPv6 Hop-by-Hop Option
					{
						ext = (struct IPv6ExtHeader*) (packet+EthTypeSize+IpSize+extraLenght);

						ip6_un1_nxt = ext->ip6e_nxt;
						extraLenght += ext->ip6e_len + 8;

						break;
					}
					case 43:  //Routing Header for IPv6
					{
						ext = (struct IPv6ExtHeader*) (packet+EthTypeSize+IpSize+extraLenght);

						ip6_un1_nxt = ext->ip6e_nxt;
						extraLenght += ext->ip6e_len + 8;

						break;
					}
					case 44:  //Fragment Header for IPv6
					{
						ext = (struct IPv6ExtHeader*) (packet+EthTypeSize+IpSize+extraLenght);

						ip6_un1_nxt = ext->ip6e_nxt;
						extraLenght += ext->ip6e_len + 8;

						break;
					}
					case 60:  //Destination Options for IPv6
					{
						ext = (struct IPv6ExtHeader*) (packet+EthTypeSize+IpSize+extraLenght);

						ip6_un1_nxt = ext->ip6e_nxt;
						extraLenght += ext->ip6e_len + 8;

						break;
					}
					default:
					{
						switch(ip6_un1_nxt)
						{
							case 6:  //TCP protocol
							{
								tcpptr = (struct TCPHeader *) (packet+EthTypeSize+IpSize+extraLenght);

								PacketList[PacketNumber - 1].tcpptr = *tcpptr;
								PacketList[PacketNumber - 1].Layer3 = 1;
								
								NextHeaders = false;
								break;
							}
							case 17: //UDP protocol
							{
								udpptr = (struct UDPHeader *) (packet+EthTypeSize+IpSize+extraLenght);

								PacketList[PacketNumber - 1].udpptr = *udpptr;
								PacketList[PacketNumber - 1].Layer3 = 2;  

								NextHeaders = false;
								break;
							}
							case 58:  //ICMPv6
							{
								icmpv6ptr = (struct ICMPv6Header *) (packet+EthTypeSize+IpSize+extraLenght);

								PacketList[PacketNumber - 1].icmpv6ptr = *icmpv6ptr;
								PacketList[PacketNumber - 1].Layer3 = 3;
								
								NextHeaders = false;
								break;
							}
							default:  //Nor TCP nor UDP nor ICMPv4 nor ICMPv6
							{
								NextHeaders = false;
								break;
							}
						}
						break;
					}
				}
			}

		}

		PacketList[PacketNumber - 1].ts = header.ts;
		PacketList[PacketNumber - 1].len = header.len;
		PacketList[PacketNumber - 1].PacketNumber = PacketNumber;
	}

	if (avalue == -2)
	{
		//razeni
		switch (svalue)
		{
			case -2:
			{
				for (int i = 0; i < NumberOfPackets; i++)
				{
					if (printed < lvalue)
					{
						printPacket(PacketList.at(i));
						printed++;
					}
					else
					{
						break;
					}
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
					if (printed < lvalue)
					{
						Temp = GetMax(PacketList, NumberOfPackets);
						printPacket(PacketList.at(Temp));
						PacketList[Temp].len = 0;
						printed++;
					}
					else
					{
						break;
					}
				}
				break;
			}
			default:
			{
				break;
			}
		}
	}
	else
	{
		printPacketAggr(PacketList, NumberOfPackets, avalue, svalue, lvalue);
	}

	pcap_close(handle);

	return 0;
}