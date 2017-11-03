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

struct sniff_ip {
#if BYTE_ORDER == LITTLE_ENDIAN
             u_char  ip_hl:4,                /* header length */
                     ip_v:4;                 /* version */
#endif
#if BYTE_ORDER == BIG_ENDIAN
            u_char  ip_v:4,                 /* version */
                    ip_hl:4;                /* header length */
#endif
    u_char ip_tos;		    /* type of service */
    u_short ip_len;		    /* total length */
    u_short ip_id;		    /* identification */
    u_short ip_off;		    /* fragment offset field */
#define	IP_RF 0x8000			/* reserved fragment flag */
#define	IP_DF 0x4000			/* dont fragment flag */
#define	IP_MF 0x2000			/* more fragments flag */
#define	IP_OFFMASK 0x1fff		/* mask for fragmenting bits */
/*(flags_fragmentOffset & 0x4000) != 0*/
    u_char ip_ttl;		    /* time to live */
    u_char ip_p;		    /* protocol */
    u_short ip_sum;		    /* checksum */
    struct in_addr ip_src,ip_dst; /* source and dest address */
};

struct ADHeader {
    uint8_t AD_dhost[6];
    uint8_t AD_shost[6];
    uint16_t AD_tpid;
    uint16_t AD_tci;
    uint16_t AD_tpid2;
    uint16_t AD_tci2;
    uint16_t AD_ether_type;
};

struct QHeader {
  /* u_char Q_dhost[6];
    u_char Q_shost[6];
    u_short Q_tpid;
    u_short Q_ether_type;
 */
    uint8_t Q_dhost[6];
    uint8_t Q_shost[6];
    uint16_t Q_tpid1;
    uint16_t Q_tpid2;
    uint16_t Q_ether_type;
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
	struct sniff_ip *ip_layer;

	if ((handle = pcap_open_offline(filename,errbuf)) == NULL)
		ErrorFound(9);

	//int i = pcap_datalink(handle);

	//printf("%d\n", i);
	
	while ((packet = pcap_next(handle,&header)) != NULL)  //v header jsou hodnoty hlavicky paketu, v packetu je ukazatel na zacatek
	{
		
		//printf("Length: %d\n", header.len);
	int EthTypeSize;

    	eptr = (struct ether_header *) packet;
    	//printf("\tSource MAC: %s %s\n",ether_ntoa((const struct ether_addr *)&eptr->ether_shost));
    	//printf("\tDestination MAC: %s\n",ether_ntoa((const struct ether_addr *)&eptr->ether_dhost));

    	//printf("%s\n", packet);

    	switch (ntohs(eptr->ether_type))
    	{
    		case 0x0800: //ETHERTYPE_IP IPV4
    		EthTypeSize = 14;
    		printf("| Ethernet %s %s\n",ether_ntoa((const struct ether_addr *)&eptr->ether_shost), ether_ntoa((const struct ether_addr *)&eptr->ether_dhost));
    		//printf("ETHERTYPE_IP IPV4\n");
    		break;

    		case 0x86DD: //ETHERTYPE_IP IPV6
    		EthTypeSize = 14;
    		printf("| Ethernet %s %s\n",ether_ntoa((const struct ether_addr *)&eptr->ether_shost), ether_ntoa((const struct ether_addr *)&eptr->ether_dhost));
    		//printf("ETHERTYPE_IP IPV6\n");
    		break;

    		case 0x8100: //IEEE 802.1Q
    		EthTypeSize = 18;
    		printf("| Ethernet %s %s\n",ether_ntoa((const struct ether_addr *)&eptr->ether_shost), ether_ntoa((const struct ether_addr *)&eptr->ether_dhost));
    		qptr = (struct QHeader *) packet;
 
    			if (ntohs(qptr->Q_ether_type) == 0x0800)//neni zde osetreny ten wtf soubor
    			{
    				printf("IPV4\n" );
    			}
    			else
    			{
    				if (ntohs(qptr->Q_ether_type) == 0x86DD)
    				{
    					printf("IPV6\n" );
    					    				u_int size_ip;

    				ip_layer = (struct sniff_ip*) (packet+EthTypeSize);
    				//size_ip = ip->ip_hl;
    				switch (ip_layer->ip_p){
    					case 6:
    					printf("TCT\n");
    					break;
						case 17: // UDP protocol
						printf("UDP\n");
						break;
						default:
						printf("shit\n");
						break;
    			}
    				}
    				else
    				{
    					printf("other\n");
    				}
    			}
    		break;

    		case 0x88a8: //IEEE 802.1ad
    		EthTypeSize = 22;
    		printf("| Ethernet %s %s\n",ether_ntoa((const struct ether_addr *)&eptr->ether_shost), ether_ntoa((const struct ether_addr *)&eptr->ether_dhost));
			adptr = (struct ADHeader *) packet;
    		//printf("IEEE 802.1Q\n");
    		//    		qptr = (struct QHeader *) packet;
    		//printf("IEEE 802.1Q\n");	
    		    		//printf("%s\n", );
    		    //printf("%x\n", qptr->Q_ether_type);
    		    //printf("| Ethernet %s %s\n",ether_ntoa((const struct ether_addr *)&qptr->Q_shost), ether_ntoa((const struct ether_addr *)&qptr->Q_dhost));
    			if (ntohs(adptr->AD_ether_type) == 0x0800)
    			{
    				printf("IPV4\n" );
    				u_int size_ip;

    				ip_layer = (struct sniff_ip*) (packet+EthTypeSize);
    				//size_ip = ip->ip_hl;
    				switch (ip_layer->ip_p){
    					case 6:
    					printf("TCT\n");
    					break;
						case 17: // UDP protocol
						printf("UDP\n");
						break;
						default:
						printf("shit\n");
						break;
    			}}
    			else
    			{
    				if (ntohs(adptr->AD_ether_type) == 0x86DD)
    				{
    					printf("IPV6\n" );

    				}
    				else
    				{
    					printf("other\n");
    				}
    			}
    		break;

    		default:
    		EthTypeSize = -1;
    		//printf("wtf\n");
    		break;
    	}
    	/*
    	if (/* condition 
    	{
    		
    	}
    	my_ip = (struct ip*) (packet+SIZE_ETHERNET);)*/

	}


	return 0;
}