/**
 * Projekt ISA 2017/18 - Analyzator paketu
 * xbucha02, Petr Buchal
**/
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
#include <iostream>
using namespace std;

/*###############################################*/
/*###################Struktury###################*/
/*###############################################*/

//ICMPv4 Hlavicka
struct ICMPv4Header {
	u_int8_t icmp4_type;  //ICMPv4 Type
	u_int8_t icmp4_code;  //ICMPv4 Code
	u_int16_t icmp4_sum;  //ICMPv4 Checksum
};

//ICMPv6 Hlavicka
struct ICMPv6Header {
	u_int8_t icmp6_type;  //ICMPv6 Type
	u_int8_t icmp6_code;  //ICMPv6 Code
	u_int16_t icmp6_cksum;  //ICMPv6 Checksum
};

//UDP Hlavicka
struct UDPHeader {
	u_short	uh_sport;  //UDP Source port
	u_short	uh_dport;  //UDP Destination port
	short	uh_ulen;  //UDP Length
	u_short	uh_sum;  //UDP Checksum
};

//TCP Hlavicka
struct TCPHeader {
	u_short th_sport;  //TCP Source port
	u_short th_dport;  //TCP Destination port
	u_int th_seq;  //TCP Sequence number
	u_int th_ack;  //TCP Acknowledgement number
	u_char th_off;  //TCP Data offset
	u_char th_flags;  //TCP Flags
	u_short th_win;  //TCP Window
	u_short th_sum;  //TCP Checksum
	u_short th_urp;  //TCP Urgent pointer
	//Flag masks
	#define TH_FIN 0x01
	#define TH_SYN 0x02
	#define TH_RST 0x04
	#define TH_PUSH 0x08
	#define TH_ACK 0x10
	#define TH_URG 0x20
	#define TH_ECE 0x40
	#define TH_CWR 0x80
};

//IPv4 Hlavicka
struct IPv4Header {
	#if BYTE_ORDER == LITTLE_ENDIAN
		u_char  ip_hl:4,  //IPv4 Header length
		ip_v:4;  //IPv4 Version
	#endif
	#if BYTE_ORDER == BIG_ENDIAN
		u_char  ip_v:4,  //IPv4 Version
		ip_hl:4;  //IPv4 Header length
	#endif
	u_char ip_tos;  //IPv4 Type of service
	u_short ip_len;  //IPv4 Total length
	u_short ip_id;  //IPv4 Indentifier
	u_short ip_off;  //IPv4 Fragment offset
	u_char ip_ttl;  //IPv4 Time to live
	u_char ip_p;  //IPv4 Protocol
	u_short ip_sum;  //IPv4 Header checksum
	struct in_addr ip_src;  //IPv4 Source address
	struct in_addr ip_dst;  //IPv4 Destination address
	//Offset masks
	#define	IP_RF 0x8000  //IPv4 Mask for reserved bit
	#define	IP_DF 0x4000  //IPv4 Mask for dont fragment bit
	#define	IP_MF 0x2000  //IPv4 Mask for more fragments bit
	#define	IP_OFFMASK 0x1fff  //IPv4 Mask for offset
};

//IPv6 Rozsirujici hlavicka
struct IPv6ExtHeader{
	uint8_t  ip6e_nxt;  //IPv6 (extended) Next header
	uint8_t  ip6e_len;  //IPv6 (extended) Length
};

//IPv6 Hlavicka
struct IPv6Header {
	union {
		struct ip6_hdrctl {
			u_int32_t ip6_un1_flow;	 //IPv6 Flow label
			u_int16_t ip6_un1_plen;	 //IPv6 Payload length
			u_int8_t  ip6_un1_nxt;	 //IPv6 Next header
			u_int8_t  ip6_un1_hlim;	 //IPv6 Hop limit
		} ip6_un1;
		u_int8_t ip6_un2_vfc;  //IPv6 Version
	} ip6_ctlun;
	struct in6_addr ip6_src;  //IPv6 Source address
	struct in6_addr ip6_dst;  //IPv6 Destination address
};

//IEEE 802.1ad Hlavicka
struct ADHeader {
	uint8_t AD_dhost[ETHER_ADDR_LEN];  //IEEE 802.1ad Destination MAC address
	uint8_t AD_shost[ETHER_ADDR_LEN];  //IEEE 802.1ad Source MAC address
	uint16_t AD_tpid;  //IEEE 802.1ad TPID=0x88a8
	uint16_t AD_tci;  //IEEE 802.1ad PCI/DEI/VID
	uint16_t AD_tpid2;  //IEEE 802.1ad TPID=0x8100
	uint16_t AD_tci2;  //IEEE 802.1ad PCI/DEI/VID
	uint16_t AD_ether_type;  //IEEE 802.1ad EtherType
};

//IEEE 802.1Q Hlavicka
struct QHeader {
	uint8_t Q_dhost[ETHER_ADDR_LEN];  //IEEE 802.1Q Destination MAC address
	uint8_t Q_shost[ETHER_ADDR_LEN];  //IEEE 802.1Q Source MAC address
	uint16_t Q_tpid1;  //IEEE 802.1Q TPID=0x8100
	uint16_t Q_tpid2;  //IEEE 802.1Q PCI/DEI/VID
	uint16_t Q_ether_type;  //IEEE 802.1Q EtherType
};

//Ethernet Hlavicka
struct ETHHeader {
	uint8_t ether_dhost[ETHER_ADDR_LEN];  //Ethernet Destination MAC address
	uint8_t ether_shost[ETHER_ADDR_LEN];  //Ethernet Source MAC address
	uint16_t ether_type;  //Ethernet EtherType
};

//Struktura pro ukladani zpracovanych paketu
struct PacketData {
	int Layer1;  //Druh L2 vrstvy
	struct ETHHeader eptr;  //Ethernet hlavicka
	struct QHeader qptr;  //IEEE 802.1Q Hlavicka
	struct ADHeader adptr;  //IEEE 802.1ad Hlavicka
	int Layer2;  //Druh L3 vrstvy
	struct IPv4Header ipv4ptr;  //IPv6 Hlavicka
	struct IPv6Header ipv6ptr;  //IPv4 Hlavicka
	int Layer3;  //Druh L2 vrstvy
	struct TCPHeader tcpptr;  //TCP Hlavicka
	struct UDPHeader udpptr;  //UDP Hlavicka
	struct ICMPv4Header icmpv4ptr;  //ICMPv4 Hlavicka
	struct ICMPv6Header icmpv6ptr;  //ICMPv6 Hlavicka
	int PacketNumber;  //Cislo zpracovaneho paketu
	struct timeval ts;  //Casova znacka v mikrosekundach
	bpf_u_int32 len;  //Delka paketu v bajtech
};

//Struktura pro ulozeni agregovanych zpracovanych paketu
struct AggrData {
	uint8_t Aggr_shost[ETHER_ADDR_LEN];  //Agregovana zdrojova MAC adresa
	uint8_t Aggr_dhost[ETHER_ADDR_LEN];  //Agregovana cilova MAC adresa
   	struct in_addr Aggr_ip_src;  //Agregovana zdrojova IPv4 adresa
	struct in_addr Aggr_ip_dst;  //Agregovana cilova IPv4 adresa
	int IpType;  //Druh IP adresy
	struct in6_addr Aggr_ip6_src;  //Agregovana zdrojova IPv6 adresa
	struct in6_addr Aggr_ip6_dst;  //Agregovana cilova IPv6 adresa
	u_short	Aggr_sport;  //Agregovany zdrojovy port
	u_short	Aggr_dport;  //Agregovany cilovy port
	int NumberOfPackets;  //Pocet agregovanych paketu
	bpf_u_int32 len;  //Delka agregovanych paketu
};

//FRAGMENTACE
struct Hole{
	u_int32_t first;
	u_int32_t last;
};

//FRAGMENTACE
struct PacketToReassemble{
	struct in_addr ip_src; 
	struct in_addr ip_dst;  
	u_short ip_id;
	std::vector<Hole> Holes;
};

/*###############################################*/
/*#####################Funkce####################*/
/*###############################################*/

/**
 * funkce pro tisk napovedy
 */
void PrintHelp()
{
	printf("Usage:\n");
	printf("isashark [-h] [-a aggr-key] [-s sort-key] [-l limit] [-f filter-expression] file ...\n");
	printf("  -h  Vypise napovedu a ukonci program.\n");
	printf("  -a aggr-key  Zapnuti agregace podle klice aggr-key, coz muze byt srcmac znacici\n");
	printf("zdrojovou MAC adresu, dstmac znacici cilovou MAC adresu, srcip znacici zdrojovou IP adresu,\n");
	printf("dstip znacici cilovou IP adresu, srcport znacici cislo zdrojoveho transportniho portu nebo\n");
	printf("dstport znacici cislo ciloveho transportniho portu.\n");
	printf("  -s sort-key  Zapnuti razeni podle klice sort-key, coz muze byt packets (pocet paketu)\n");
	printf("nebo bytes (pocet bajtu).\n");
	printf("  -l limit  Nezaporne cele cislo v desitkove soustave udavajici limit poctu vypsanych polozek.\n");
	printf("  -f filter-expression  Program zpracuje pouze pakety, ktere vyhovuji filtru danemu retezcem\n");
	printf("filter-expression.\n");
	printf("  file  Cesta k souboru ve formatu pcap (citelny knihovnou libpcap). Mozne je zadat jeden\n");
	printf("a vice souboru.\n");
	exit(0);
}


/**
 * funkce na tisk chybovych hlasek a ukonceni programu
 * @param i cislo chybove hlasky
 */
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
			printf (stderr, "Non valid argument.\n");
			break;
		case 8:
			fprintf (stderr, "Non valid argument combination.\n");
			break;	
		case 9:
			fprintf (stderr, "Can't open file for reading.\n");
			break;
		case 10:
			fprintf (stderr, "Non valid pcap filter.\n");
			break;
		default:
			fprintf (stderr, "Error.\n");
			break;
	}
	exit(1);
}

/**
 * funkce na prevod retezce na integer
 * @param  str retezec na prevod
 * @return     integer po prevodu
 */
int CharToInt (char *str)
{
	int length = strlen(str);  //zjisteni delky stringu
	int number = 0;
	int i;

	for (i=0; i<length; i++)
	{
		if (!isdigit(str[i]))  //kontrola na znak
		{
			return -1;
		}
		if (str[i] == '-')  //kontrola na -
		{
			return -1;
		}
		//prevod
		number = number * 10;
		number = number + (((int) str[i]) - 48);
	}

	return number;
}

/**
 * funkce pro ziskani poctu paketu vyhovujicich filteru v danem souboru
 * @param  filename nazev souboru
 * @param  fvalue   filter
 * @return          pocet paktu v souboru
 */
int GetNumberOfPackets(char *filename, char *fvalue)
{
	int counter = 0;  //pocitadlo paketu
	pcap_t *handle;  //paketu capture handler
	const u_char *packet;  //ukazatel na paket
	char errbuf[256];  //error buffer
	struct pcap_pkthdr header;  //hlavicka packetu
	struct bpf_program fp;  //zkompilovany filter

	if ((handle = pcap_open_offline(filename,errbuf)) == NULL)  //otevreni souboru
	{
		ErrorFound(9);
	}

	if (strcmp("", fvalue) != 0)  //kontrola zdali byl zadan filter
	{
		if (pcap_compile(handle,&fp, fvalue, 0, 0) == -1)  //kompilace filteru
		{
			ErrorFound(10);
		}
		if (pcap_setfilter(handle, &fp) == -1)  //nastaveni filteru
		{
			ErrorFound(10);
		}
	}

	while ((packet = pcap_next(handle,&header)) != NULL)  //prochazeni souboru
	{
		counter++;
	}

	pcap_close(handle);  //uzavreni souboru
	return counter;
}

/**
 * Oprava retezce MAC adresy do formatu XX:XX:XX:XX:XX:XX
 * @param  str opravovana adresa
 * @return     opravena adrea
 */
int CorrectMacAdress(char *str)
{
	char Temp[256];  //docasna verze opravovaneho retezce
	strcpy(Temp, str);  //zkopirovani retezce do docasneho
	char Temp2[256];  //zalohovani 2-3 znaku
	char ReconstructedMacAdress[256];  //opravena MAC adresa
	int i = 0;  //pocitadlo znaku
	int counter = 0;  //pocitadlo zdali jsme dosahli hranice dvou znaku
	int e = 0;  //pocitadlo pro reconstructed retezec

	//stavovy automat na opravu MAC adresy
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

	strcpy(str,ReconstructedMacAdress);  //zkopirovani adresy na misto ukazatele
	return 0;
}

//FRAGMENTACE
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

/**
 * funkce na ziskani cisla prvku s maximalni poctem bajtu 
 * @param  PacketList      vektor prvku
 * @param  NumberOfPackets pocet prvku
 * @return                 cislo prvku s maximalni poctem bajtu
 */
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

/**
 * funkce na tisk paketu
 * @param PacketList tisknuty paket
 */
void printPacket(struct PacketData PacketList)
{
	char MacSrc[256];  //retezec pro tisk zdrojove MAC adresy
	char MacDst[256];  //retezec pro tisk cilove MAC adresy
	char printAbleIPv6src[INET6_ADDRSTRLEN];  //retezec pro tisk zdrojove IPv6 adresy
	char printAbleIPv6dst[INET6_ADDRSTRLEN];  //retezec pro tisk cilove IPv6 adresy

	switch (PacketList.Layer1)  //vyber formatu tisku L2 podle jejiho typu
	{
		case 0:  //Ethernet
		{
			strcpy(MacSrc, ether_ntoa((const struct ether_addr *)&PacketList.eptr.ether_shost));
			strcpy(MacDst, ether_ntoa((const struct ether_addr *)&PacketList.eptr.ether_dhost));
			CorrectMacAdress(MacSrc);
			CorrectMacAdress(MacDst);

			printf("%d: %ld%06ld %u | Ethernet: %s %s | ",
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

			printf("%d: %ld%06ld %u | Ethernet: %s %s %u | ",
				PacketList.PacketNumber,
				PacketList.ts.tv_sec,
				PacketList.ts.tv_usec,
				PacketList.len,
				MacSrc,
				MacDst,
				ntohs(PacketList.qptr.Q_tpid2 & 0xFFF));
			break;
		}
		case 2:  //IEEE 802.1ad
		{
			strcpy(MacSrc, ether_ntoa((const struct ether_addr *)&PacketList.adptr.AD_shost));
			strcpy(MacDst, ether_ntoa((const struct ether_addr *)&PacketList.adptr.AD_dhost));
			CorrectMacAdress(MacSrc);
			CorrectMacAdress(MacDst);

			printf("%d: %ld%06ld %u | Ethernet: %s %s %u %u | ",
				PacketList.PacketNumber,
				PacketList.ts.tv_sec,
				PacketList.ts.tv_usec,
				PacketList.len,
				MacSrc,
				MacDst,
				ntohs(PacketList.adptr.AD_tci)& 0xFFF,
				ntohs(PacketList.adptr.AD_tci2)& 0xFFF);
			break;
		}
		default:
		{
			break;
		}
	}

	switch (PacketList.Layer2)  //vyber formatu tisku L3 podle jejiho typu
	{
		case 0:  //IPv4
		{
			printf("IPv4: %s ",
				inet_ntoa(PacketList.ipv4ptr.ip_src));
			printf("%s %u | ",
				inet_ntoa(PacketList.ipv4ptr.ip_dst),
				PacketList.ipv4ptr.ip_ttl);
			break;
		}
		case 1:  //IPv6
		{
			printf("IPv6: %s %s %u | ",
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

	switch (PacketList.Layer3)  //vyber formatu tisku L4 podle jejiho typu
	{
		case 0:  //ICMPv4
		{
			printf("ICMPv4: %u %u",
				PacketList.icmpv4ptr.icmp4_type,
				PacketList.icmpv4ptr.icmp4_code);
			switch(PacketList.icmpv4ptr.icmp4_type)  //tisk chybovych hlasek podle typu
			{
				case 0:  //Echo Reply
				{
					printf(" echo reply\n");
					break;
				}
				case 3:  //Destination Unreachable
				{
					switch(PacketList.icmpv4ptr.icmp4_code)  //tisk chybovych hlasek podle kodu
					{
						case 0:  //net unreachable
						{
							printf(" destination unreachable net unreachable\n");
							break;
						}
						case 1:  //host unreachable
						{
							printf(" destination unreachable host unreachable\n");
							break;
						}
						case 2:  //protocol unreachable
						{
							printf(" destination unreachable protocol unreachable\n");
							break;
						}
						case 3:  //port unreachable
						{
							printf(" destination unreachable port unreachable\n");
							break;
						}
						case 4:  //fragmentation needed and DF set
						{
							printf(" destination unreachable fragmentation needed and DF set\n");
							break;
						}
						case 5:  //source route failed
						{
							printf(" destination unreachable source route failed\n");
							break;
						}
						default:
						{
							printf("\n");
							break;
						}
					}
					break;
				}
				case 4:  //Source Quench (Deprecated)
				{
					printf(" source quench\n");
					break;
				}
				case 5:  //Redirect
				{
					switch(PacketList.icmpv4ptr.icmp4_code)  //tisk chybovych hlasek podle kodu
					{
						case 0:  //Redirect datagrams for the Network
						{
							printf(" redirect redirect datagrams for the network\n");
							break;
						}
						case 1:  //Redirect datagrams for the Host
						{
							printf(" redirect redirect datagrams for the host\n");
							break;
						}
						case 2:  //Redirect datagrams for the Type of Service and Network
						{
							printf(" redirect redirect datagrams for the type of service and network\n");
							break;
						}
						case 3:  //Redirect datagrams for the Type of Service and Host
						{
							printf(" redirect redirect datagrams for the type of service and host\n");
							break;
						}
						default:
						{
							printf("\n");
							break;
						}
					}
					break;
				}
				case 8:  //Echo
				{
					printf(" echo\n");
					break;
				}
				case 11:  //Time Exceeded
				{
					switch(PacketList.icmpv4ptr.icmp4_code)  //tisk chybovych hlasek podle kodu
					{
						case 0:  //time to live exceeded in transit
						{
							printf(" time exceeded time to live exceeded in transit\n");
							break;
						}
						case 1:  //fragment reassembly time exceeded
						{
							printf(" time exceeded fragment reassembly time exceeded\n");
							break;
						}
						default:
						{
							printf("\n");
							break;
						}
					}
					break;
				}
				case 12:  //Parameter Problem
				{
					printf(" parameter problem pointer indicates the error\n");
					break;
				}
				case 13:  //Timestamp
				{
					printf(" timestamp\n");
					break;
				}
				case 14:  //Timestamp Reply
				{
					printf(" timestamp reply\n");
					break;
				}
				case 15:  //Information Request (Deprecated)
				{
					printf(" information request\n");
					break;
				}
				case 16:  //Information Reply (Deprecated)
				{
					printf(" information reply\n");
					break;
				}
				default:  //tisk ostatnich ICMPv4
				{
					printf("ICMPv4: %u %u\n",
						PacketList.icmpv4ptr.icmp4_type,
						PacketList.icmpv4ptr.icmp4_code);
					break;
				}
			} 
			break;
		}
		case 1:  //TCP
		{
			printf("TCP: %u %u %u %u ",
				ntohs(PacketList.tcpptr.th_sport),
				ntohs(PacketList.tcpptr.th_dport),
				ntohl(PacketList.tcpptr.th_seq),
				ntohl(PacketList.tcpptr.th_ack));
			//tisk flagu
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
			printf("UDP: %u %u\n", 
				ntohs(PacketList.udpptr.uh_sport),
				ntohs(PacketList.udpptr.uh_dport));
			break;
		}
		case 3:  //ICMPv6
		{
			printf("ICMPv6: %u %u",
				PacketList.icmpv6ptr.icmp6_type,
				PacketList.icmpv6ptr.icmp6_code);
			switch(PacketList.icmpv6ptr.icmp6_type)  //tisk chybovych hlasek podle typu  
			{
				case 1:  //Destination Unreachable
				{
					switch(PacketList.icmpv6ptr.icmp6_code)  //tisk chybovych hlasek podle kodu
					{
						case 0:  //No route to destination
						{
							printf(" destination unreachable no route to destination\n");
							break;
						}
						case 1:  //Communication with destination administratively prohibited
						{
							printf(" destination unreachable communication with destination administratively prohibited\n");
							break;
						}
						case 2:  //Beyond scope of source address
						{
							printf(" destination unreachable beyond scope of source address\n");
							break;
						}
						case 3:  //Address unreachable
						{
							printf(" destination unreachable address unreachable\n");
							break;
						}
						case 4:  //Port unreachable
						{
							printf(" destination unreachable port unreachable\n");
							break;
						}
						case 5:  //Source address failed ingress/egress policy
						{
							printf(" destination unreachable source address failed ingress/egress policy\n");
							break;
						}
						case 6:  //Reject route to destination
						{
							printf(" destination unreachable reject route to destination\n");
							break;
						}
						default:
						{
							printf("\n");
							break;
						}
					}
					break;
				}
				case 2:  //Packet Too Big
				{
					printf(" packet too big");
					break;
				}
				case 3:  //Time Exceeded
				{
					switch(PacketList.icmpv6ptr.icmp6_code)  //tisk chybovych hlasek podle kodu
					{
						case 0:  //hop limit exceeded in transit
						{
							printf(" time exceeded hop limit exceeded in transit\n");
							break;
						}
						case 1:  //fragment reassembly time exceeded
						{
							printf(" time exceeded fragment reassembly time exceeded\n");
							break;
						}
						default:
						{
							printf("\n");
							break;
						}
					}
					break;
				}
				case 4:  //Parameter Problem
				{
					switch(PacketList.icmpv6ptr.icmp6_code)  //tisk chybovych hlasek podle kodu
					{
						case 0:  //Erroneous header field encountered
						{
							printf(" parameter problem erroneous header field encountered\n");
							break;
						}
						case 1:  //Unrecognized Next Header type encountered
						{
							printf(" parameter problem unrecognized next header type encountered\n");
							break;
						}
						case 2:  //Unrecognized IPv6 option encountered
						{
							printf(" parameter problem unrecognized ipv6 option encountered\n");
							break;
						}
						default:
						{
							printf("\n");
							break;
						}
					}
					break;
				}
				case 128:  //Echo Request
				{
					printf(" echo request\n");
					break;
				}
				case 129:  //Echo Reply
				{
					printf(" echo reply\n");
					break;
				}
				default:  //tisk ostatnich ICMPv6
				{
					printf("ICMPv6: %u %u\n",
						PacketList.icmpv6ptr.icmp6_type,
						PacketList.icmpv6ptr.icmp6_code);
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

/**
 * funkce na ziskani cisla agregovaneho prvku s maximalni poctem bajtu 
 * @param  PacketList      vektor agregovanych prvku
 * @param  NumberOfPackets pocet agregovanych prvku
 * @return                 cislo agregovaneho prvku s maximalni poctem bajtu
 */
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
/**
 * funkce na ziskani cisla agregovaneho prvku s maximalni poctem paketu
 * @param  PacketList      vektor agregovanych prvku
 * @param  NumberOfPackets pocet agregovanych prvku
 * @return                 cislo agregovaneho prvku s maximalni poctem paketu
 */
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

/**
 * [printPacketAggr description]
 * @param PacketList      [description]
 * @param NumberOfPackets [description]
 * @param avalue          [description]
 * @param svalue          [description]
 * @param lvalue          [description]
 */
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

/*###############################################*/
/*##################Telo programu################*/
/*###############################################*/

int main (int argc, char *argv[])
{
	bool hflag = false;  //hodnota pro ulozeni argumentu -h
	unsigned int lvalue = 4294967295;  //hodnota pro ulozeni argumentu -l
	int avalue = -2;  //hodnota pro ulozeni argumentu -a
	int svalue = -2;  //hodnota pro ulozeni argumentu -s
	char fvalue[256] = "";  //hodnota pro ulozeni argumentu -f
	unsigned int printed = 0;  //pocet vytistenych paketu (pomocna promenna, kdyz je zadan arg l)
	
	//zpracovani argumentu
	int c;
	int index;
	opterr = 0;
	while ((c = getopt (argc, argv, "ha:s:l:f:")) != -1)  
	switch (c)
	{
		case 'h':
			hflag = true;
			break;
		case 'a':  //provadi se kontrola, zdali argument obsahuje pozadovane klicove slova, pokud ne, konci chybou
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
			if (avalue == -2)
			{
				ErrorFound(7);
			}
			break;
		case 's':  //provadi se kontrola, zdali argument obsahuje pozadovane klicove slova, pokud ne, konci chybou
			if (strcmp(optarg, "packets") == 0)
			{
				svalue = 0;
			}
			if (strcmp(optarg, "bytes") == 0)
			{
				svalue = 1;
			}
			if (svalue == -2)
			{
				ErrorFound(7);
			}
			break;
		case 'l':
			lvalue = CharToInt(optarg);  //prevod stringu na int
			if (lvalue == 4294967295)  //kontrola na zaporne cislo
			{
				ErrorFound(0);
			}
			break;
		case 'f':
			strcpy(fvalue, optarg);  //ziskani hodnoty filteru
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

	int NumberOfFiles = argc-optind;  //jakykoli vyraz mimo argumenty se pocita jako nazev souboru
	char filenames[NumberOfFiles][256];  //pole obsahujici nazvy souboru

	//printf ("hflag = %d, avalue = %d, svalue = %d, lvalue = %d, fvalue = %s\n\n", hflag, avalue, svalue, lvalue, fvalue);
	
	for (index = optind; index < argc; index++)
	{
		strcpy(filenames[argc - index - 1], argv[index]);  //ulozeni jmen souboru do pole
	}

	//pokud byl zadan argument -h, vypise se napoveda, pokud byl zadan zaroven s jinym, vypise se chybova hlaska, program konci
	if (hflag == true && avalue == -2 && svalue == -2 && strcmp(fvalue, "") == 0 && lvalue == 4294967295)  
	{
		PrintHelp();  //tisk napovedy
	}
	else
	{
		if (hflag == true && (avalue != -2 || svalue != -2 || strcmp(fvalue, "") != 0 || lvalue != 4294967295))
		{
			ErrorFound(8);
		}
	}

	/*for (int i = 0; i < NumberOfFiles; i++)
	{
		printf("%s\n", filenames[i]);
	}*/

	//Promenne pro praci s pcap knihovnou
	pcap_t *handle;  //packet capture handler
	char errbuf[256];  //error buffer
	struct pcap_pkthdr header;  //hlavicka packetu
	struct bpf_program fp;  //zkompilovany filter

	//Ukazatele na analyzu paketu
	const u_char *packet;  //ukazatel na paket
  	struct ETHHeader *eptr;  //ukazatel na Ethernet hlavicku
	struct QHeader *qptr;  //ukazatel na 802.1Q hlavicku
  	struct ADHeader *adptr;  //ukazatel na 802.1ad hlavicku
	struct IPv4Header *ipv4ptr;  //ukazatel na IPv4 hlavicku
	struct IPv6Header *ipv6ptr;  //ukazatel na IPv6 hlavicku
	struct IPv6ExtHeader *ext;  //ukazatel na Extended IPv6 hlavicku
	struct ICMPv4Header *icmpv4ptr;  //ukazatel na ICMPv4 hlavicku
	struct ICMPv6Header *icmpv6ptr;  //ukazatel na ICMPv6 hlavicku
	const struct TCPHeader *tcpptr;	 //ukazatel na TCP hlavicku
	const struct UDPHeader *udpptr;	 //ukazatel na UDP hlavicku
	int PacketNumber = 0;  //cislo zpracovaneho packetu

	std::vector<PacketToReassemble> NCPackets;  //vektor pro ukladani fragmentovanych IPv4 paketu

	int NumberOfPackets = 0;  //celkovy pocet packetu ve vsech souborech

	//ziskani celkoveho poctu packetu ze vsech souboru
	for (int i = 0; i < NumberOfFiles; i++)
	{
		NumberOfPackets += GetNumberOfPackets(filenames[i], fvalue);
	}
	 
	std::vector<PacketData> PacketList(NumberOfPackets);  //vektor zpracovanych paketu

	//cyklus prochazeni souboru jednoho po druhem
	for (int k = 0; k < NumberOfFiles; k++)
	{
		if ((handle = pcap_open_offline(filenames[k],errbuf)) == NULL)  //otevreni souboru
		{
			ErrorFound(9);
		}
		
		if (strcmp("", fvalue) != 0)  //kontrola zdali byl zadan filter
		{
			if (pcap_compile(handle,&fp,fvalue,0,0) == -1)  //kompilace filteru
			{
				ErrorFound(10);
			}
			if (pcap_setfilter(handle,&fp) == -1)  //aplikace filteru
			{
				ErrorFound(10); 
			}
		}

		while ((packet = pcap_next(handle,&header)) != NULL)  //v header jsou hodnoty hlavicky paketu, v packetu je ukazatel na zacatek
		{
			int EthTypeSize = 0;  //delka ethernetove hlavicky
			int IpSize = 0;  //delka IP hlavicky
			PacketNumber++;  //aktualni cislo packetu
			eptr = (struct ETHHeader *) packet;  //ukazetel na ethernetovou hlavicku

			switch (ntohs(eptr->ether_type))  //podle ethertypu se vybere bud IPv4 nebo IPv6, nebo se dal zkouma standart 802.1
			{
				case 0x0800:  //ETHERTYPE_IP IPV4
				{
					EthTypeSize = 14;
					ipv4ptr = (struct IPv4Header*) (packet+EthTypeSize);
					IpSize = ipv4ptr->ip_hl*4; 

					PacketList[PacketNumber - 1].eptr = *eptr;
					PacketList[PacketNumber - 1].Layer1 = 0;
					
					break;
				}
				case 0x86DD:  //ETHERTYPE_IP IPV6
				{
					EthTypeSize = 14;
					IpSize = 40;

					PacketList[PacketNumber - 1].eptr = *eptr;
					PacketList[PacketNumber - 1].Layer1 = 0;

					break;
				}
				case 0x8100:  //IEEE 802.1Q
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
						default:  //OSETRIT
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
						default:  //OSETRIT
						{
							break;
						}
					}
					break;	
				}
				default:  //OSETRIT
				{
					break;
				}
			}

			//rozebrani IPv4 protokolu
			if (ntohs(eptr->ether_type) == 0x0800 || (ntohs(eptr->ether_type) == 0x8100 && ntohs(qptr->Q_ether_type) == 0x0800) || (ntohs(eptr->ether_type) == 0x88a8 && ntohs(adptr->AD_ether_type) == 0x0800))
			{
				if ((ntohs(ipv4ptr->ip_off) & IP_MF) != 0)  //FRAGMENTACE
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
					PacketList[PacketNumber - 1].Layer2 = 0;  //zvoleni IPv4 vrstvy
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
						case 17:  //UDP protocol
						{
							udpptr = (struct UDPHeader*) (packet+EthTypeSize+IpSize);

							PacketList[PacketNumber - 1].udpptr = *udpptr;
							PacketList[PacketNumber - 1].Layer3 = 2;  

							break;
						}
						default:  //OSETRIT
						{
							break;
						}
					}
				}		
			}

			//rozebrani IPv6 protokolu
			if (ntohs(eptr->ether_type) == 0x86DD || (ntohs(eptr->ether_type) == 0x8100 && ntohs(qptr->Q_ether_type) == 0x86DD) || (ntohs(eptr->ether_type) == 0x88a8 && ntohs(adptr->AD_ether_type) == 0x86DD))
			{
				ipv6ptr = (struct IPv6Header*) (packet+EthTypeSize);

				PacketList[PacketNumber - 1].ipv6ptr = *ipv6ptr;
				PacketList[PacketNumber - 1].Layer2 = 1;  //zvoleni IPv6 vrstvy

				u_int8_t  ip6_un1_nxt = ipv6ptr->ip6_ctlun.ip6_un1.ip6_un1_nxt;  //prirazeni next header, kvuli zpracovani extension hlavicek

				bool NextHeaders = true;  //dokud je true zpracovavaji se extension hlavicky
				int extraLenght = 0;  //delka extension hlavicek

				while (NextHeaders)
				{
					switch (ip6_un1_nxt)
					{
						case 0:  //IPv6 Hop-by-Hop Option hlavicka
						{
							ext = (struct IPv6ExtHeader*) (packet+EthTypeSize+IpSize+extraLenght);

							ip6_un1_nxt = ext->ip6e_nxt;
							extraLenght += ext->ip6e_len + 8;

							break;
						}
						case 43:  //Routing Header for IPv6 hlavicka
						{
							ext = (struct IPv6ExtHeader*) (packet+EthTypeSize+IpSize+extraLenght);

							ip6_un1_nxt = ext->ip6e_nxt;
							extraLenght += ext->ip6e_len + 8;

							break;
						}
						case 44:  //Fragment Header for IPv6 hlavicka
						{
							ext = (struct IPv6ExtHeader*) (packet+EthTypeSize+IpSize+extraLenght);

							ip6_un1_nxt = ext->ip6e_nxt;
							extraLenght += ext->ip6e_len + 8;

							break;
						}
						case 60:  //Destination Options for IPv6 hlavicka
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
								default:  //OSETRIT
								{
									//NextHeaders = false;
									break;
								}
							}
							break;
						}
					}
				}
			}
			//prirazeni informaci pro prvni cast vypisu
			PacketList[PacketNumber - 1].ts = header.ts;  //epoch time
			PacketList[PacketNumber - 1].len = header.caplen;  //delka paketu
			PacketList[PacketNumber - 1].PacketNumber = PacketNumber;  //cislo paketu
		}

		pcap_close(handle);  //zavreni souboru
	}

	//tisk vysledku
	if (avalue == -2)  //pokud nebyla zadana agregace
	{
		switch (svalue)  //tisk podle razeni
		{
			case -2:  //bez razeni
			{
				;
			}
			case 0:  //podle packets
			{
				for (int i = 0; i < NumberOfPackets; i++)
				{
					if (printed < lvalue)  //kontrola zdali nebyl vytisten maximalni pocet tisknutych paketu
					{
						printPacket(PacketList.at(i));  //tisk paketu
						printed++;  //pocet jiz vytistenych paketu
					}
					else
					{
						break;
					}
				}
				break;
			}
			case 1:  //podle bytes
			{
				int Temp;  //cislo aktualne nejvetsiho packetu
				for (int i = 0; i < NumberOfPackets; i++)
				{
					if (printed < lvalue)  //kontrola zdali nebyl vytisten maximalni pocet tisknutych paketu
					{
						Temp = GetMax(PacketList, NumberOfPackets);  //nejvetsi delka v bytech
						printPacket(PacketList.at(Temp));  //tisk paketu
						PacketList[Temp].len = 0;  //po vytisteni se vynuluje delka a tudiz se paket nevytiskne znovu
						printed++;  //pocet jiz vytistenych paketu
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
	else  //pokud byla zadana agregace
	{
		printPacketAggr(PacketList, NumberOfPackets, avalue, svalue, lvalue);
	}

	return 0;
}