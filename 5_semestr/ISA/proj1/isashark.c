#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include <unistd.h>
#include <string.h>
#include <pcap/pcap.h>
#include <netinet/ether.h>

#ifndef PCAP_ERRBUF_SIZE
#define PCAP_ERRBUF_SIZE (256)
#endif

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

	printf("%s\n", filename);
	//printf ("hflag = %d, avalue = %s, svalue = %s, lvalue = %d, fvalue = %s\n", hflag, avalue, svalue, lvalue, fvalue);

	pcap_t *handle; //ukazatel na soubor s pakety
	const u_char *packet;
	char errbuf[256];
	struct pcap_pkthdr header;

	//nepopsane struktury
	const struct tcphdr *my_tcp;
  	const struct udphdr *my_udp;
  	struct ether_header *eptr;

	if ((handle = pcap_open_offline(filename,errbuf)) == NULL)
		ErrorFound(9);

	int i = pcap_datalink(handle);

	printf("%d\n", i);

	while ((packet = pcap_next(handle,&header)) != NULL)  //v header jsou hodnoty hlavicky paketu, v packetu je ukazatel na zacatek
	{
		printf("Length: %d\n", header.len);


    eptr = (struct ether_header *) packet;
    printf("\tSource MAC: %s\n",ether_ntoa((const struct ether_addr *)&eptr->ether_shost));
    printf("\tDestination MAC: %s\n",ether_ntoa((const struct ether_addr *)&eptr->ether_dhost));
	}

	return 0;
}