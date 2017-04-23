#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdbool.h>
#include <netinet/in.h>
#include <sys/time.h>
#include <netdb.h>
#include <errno.h>
#include <time.h>
#include <arpa/inet.h>
#include <linux/errqueue.h>

#define MAGIC_CONST 1024

//globalni promenne pro ulozeni argumentu
int first_ttl = 1;
int max_ttl = 31;
char ip_address[MAGIC_CONST];

//globalni promenna pro pristup k casu odeslani pro vsechny funkce
struct timeval start;
int typeOfIP;
char lastIP[MAGIC_CONST];

/*
 * TODO: 
 *  - kontrola zdali first_ttl, max_ttl obsahuji cisla
 */

//kontrola spravnosti argumentu
void arguments (int argc, char const *argv[])
{
	//vynulovani promenne pro ip adresu
	bzero(ip_address, MAGIC_CONST);

	//pomocne boolovske promenne
	bool first_ttl_bool = false;
	bool max_ttl_bool = false;
	bool ip_address_bool = false;

	switch (argc)
	{
		case 1:
			fprintf(stderr, "Chyba pri zadani argumentu.(1)\n");
			exit(1);
		case 2:
			//prvni argument
			strcpy(ip_address, argv[1]);
			break;
		case 3:
			fprintf(stderr, "Chyba pri zadani argumentu.(2)\n");
			exit(1);
		case 4:
			//prvni argument
			if (strcmp (argv[1], "-f") == 0)
			{
				first_ttl_bool = true;
				first_ttl = atoi(argv[2]);
			}
			else
			{
				if (strcmp (argv[1], "-m") == 0)
				{
					max_ttl_bool = true;
					max_ttl = atoi(argv[2]);
				}
				else
				{
					ip_address_bool = true;
					strcpy(ip_address, argv[1]);
				}
			}
			//druhy argument
			if (strcmp (argv[2], "-f") == 0)
			{
				if (!first_ttl_bool)
				{
					first_ttl_bool = true;
					first_ttl = atoi(argv[3]);
				}
				else
				{
					fprintf(stderr, "Chyba pri zadani argumentu.(3)\n");
					exit(1);
				}
			}
			else
			{
				if (strcmp (argv[2], "-m") == 0)
				{
					if (!max_ttl_bool)
					{
						max_ttl_bool = true;
						max_ttl = atoi(argv[3]);
					}
					else
					{
						fprintf(stderr, "Chyba pri zadani argumentu.(4)\n");
						exit(1);
					}
				}
				else
				{
					if (!ip_address_bool)
					{
						ip_address_bool = true;
						strcpy(ip_address, argv[3]);
					}
					else
					{
						fprintf(stderr, "Chyba pri zadani argumentu.(5)\n");
						exit(1);
					}
				}
			}
			break;
		case 5:
			fprintf(stderr, "Chyba pri zadani argumentu.(6)\n");
			exit(1);
		case 6:
			//prvni argument
			if (strcmp (argv[1], "-f") == 0)
			{
				first_ttl_bool = true;
				first_ttl = atoi(argv[2]);
			}
			else
			{
				if (strcmp (argv[1], "-m") == 0)
				{
					max_ttl_bool = true;
					max_ttl = atoi(argv[2]);
				}
				else
				{
					ip_address_bool = true;
					strcpy(ip_address, argv[1]);
				}
			}
			//druhy argument
			if (strcmp (argv[2], "-f") == 0)
			{
				if (!first_ttl_bool)
				{
					first_ttl_bool = true;
					first_ttl = atoi(argv[3]);
				}
				else
				{
					fprintf(stderr, "Chyba pri zadani argumentu.(7)\n");
					exit(1);
				}
			}
			else
			{
				if (strcmp (argv[2], "-m") == 0)
				{
					if (!max_ttl_bool)
					{
						max_ttl_bool = true;
						max_ttl = atoi(argv[3]);
					}
					else
					{
						fprintf(stderr, "Chyba pri zadani argumentu.(8)\n");
						exit(1);
					}
				}
			}
			//treti argument
			if (strcmp (argv[3], "-f") == 0)
			{
				if (!first_ttl_bool)
				{
					first_ttl_bool = true;
					first_ttl = atoi(argv[4]);
				}
				else
				{
					fprintf(stderr, "Chyba pri zadani argumentu.(10)\n");
					exit(1);
				}
			}
			else
			{
				if (strcmp (argv[3], "-m") == 0)
				{
					if (!max_ttl_bool)
					{
						max_ttl_bool = true;
						max_ttl = atoi(argv[4]);
					}
					else
					{
						fprintf(stderr, "Chyba pri zadani argumentu.(11)\n");
						exit(1);
					}
				}
				else
				{
					if (!ip_address_bool)
					{
						ip_address_bool = true;
						strcpy(ip_address, argv[3]);
					}
					else
					{
						if (!(first_ttl_bool || max_ttl_bool))
						{
							fprintf(stderr, "Chyba pri zadani argumentu.(12)\n");
							exit(1);
						}
					}
				}
			}
			//ctvrty argument
			if (strcmp (argv[4], "-f") == 0)
			{
				if (!first_ttl_bool)
				{
					first_ttl_bool = true;
					first_ttl = atoi(argv[5]);
				}
				else
				{
					fprintf(stderr, "Chyba pri zadani argumentu.(13)\n");
					exit(1);
				}
				
			}
			else
			{
				if (strcmp (argv[4], "-m") == 0)
				{
					if (!max_ttl_bool)
					{
						max_ttl_bool = true;
						max_ttl = atoi(argv[5]);
					}
					else
					{
						fprintf(stderr, "Chyba pri zadani argumentu.(14)\n");
						exit(1);
					}
				}
				else
				{
					if (first_ttl_bool && max_ttl_bool)
					{
						if (!ip_address_bool)
						{
							ip_address_bool = true;
							strcpy(ip_address, argv[5]);
						}
					}
					else
					{
						fprintf(stderr, "Chyba pri zadani argumentu.(15)\n");
						exit(1);
					}
				}
			}
			break;
		default:
			fprintf(stderr, "Chyba pri zadani argumentu.(16)\n");
			exit(1);
	}
}


//vrati pocet ms mezi dobou odeslani packetu a prijetim packetu
double timeDifference(struct timeval * start, struct timeval * end)
{
	double end_us = end->tv_usec;
	double end_s = end->tv_sec;
	double start_us = start->tv_usec;
	double start_s = start->tv_sec;
	return (((end_us - start_us) / 1000) + ((end_s - start_s) * 1000));
}


//cekani az budou data ready
void waitForSelect(int sockfd, struct timeval timeout)
{
	fd_set rfds;
	FD_ZERO(&rfds);
	FD_SET(sockfd, &rfds);
	select(sockfd+1, &rfds, NULL, NULL, &timeout);
}


//prochazi errorove zpravy od serveru
int errorCheck4(int sockfd, int ttl)
{
	struct iovec iov;  //struktura pro shromazdovani/rozptylovani vstupu a vystupu, pouziva se v msghr
	char address[MAGIC_CONST];  //retezec pouzity ve strukture iovec
	iov.iov_base = &address;  //adresa
	iov.iov_len = sizeof(address);  //pocet bytu na prenost

	char useless[MAGIC_CONST];  //retezec pro doprovodne data u zpravy
	struct msghdr small_header;  //jednotlive hlavicky se ziskavaji z cmshgr pomoci CMSG_FIRSTHDR a CMSG_NXTHDR
	small_header.msg_iov = &iov;  //shromazdovaci/rozptylovaci pole
	small_header.msg_iovlen = 1;  //clenove ve shromazdovacim poli
	small_header.msg_control = useless;  //doprovodne data
	small_header.msg_controllen = sizeof(useless);  //delka doprovodnych dat
	small_header.msg_flags = 0;  //flagy

	struct timeval end;  //casova struktura pro zaznamenani prijeti packetu
	
	int control;  //pomocna promenna pro kontrolu
	recvmsg(sockfd, &small_header, MSG_ERRQUEUE);  //vraci pocet ziskanych bytu, pokud doslo k chybe, snizi se ttl packet se posle znovu z mainu
	gettimeofday(&end, NULL);  //ziskani casu prijeti zpravy 

	struct cmsghdr *big_header;  //hlavicka pro data asociovane s datagramem, ziskavaji se z ni jednotive msghdr
	struct sock_extended_err *error;  //struktura, ktera se vyuziva pro ukladani erroru (prepinac IP_RECVERR)
	big_header = CMSG_FIRSTHDR(&small_header);
	while(big_header)
	{
		if(big_header->cmsg_level == SOL_IP)
		{
			if(big_header->cmsg_type == IP_RECVERR)
			{
				error = (struct sock_extended_err*) CMSG_DATA(big_header);
			}
		}
		big_header = CMSG_NXTHDR(&small_header, big_header);
	}

	if(error == NULL)  //zadna errorova zprava -> program dospel cile, predani rizeni mainu a konec
	{
		return 0;
	}
	else
	{
		if(error->ee_origin == SO_EE_ORIGIN_ICMP)
		{
			char ip_address[MAGIC_CONST];
			struct sockaddr_in *sin = (struct sockaddr_in*)(error+1);
			inet_ntop(AF_INET, &sin->sin_addr, ip_address, sizeof(ip_address));
			if (error->ee_type == 3)  //destination unrechable
			{
				if (error->ee_code == 0)  //network unreachable
				{
					printf("%2d   %s   N!\n", ttl, ip_address);
				}
				else
				{
					if (error->ee_code == 1)  //host unreachable
					{
						printf("%2d   %s   H!\n", ttl, ip_address);
					}
					else
					{
						if (error->ee_code == 2)  //protocol unreachable
						{
							printf("%2d   %s   P!\n", ttl, ip_address);
						}
						else
						{
							if (error->ee_code == 13)  //communication administratively prohibited
							{
								printf("%2d   %s   X!\n", ttl, ip_address);
							}
						}
					}
				}
			}
			else
			{
				if (error->ee_type == 11)
				{
					if (timeDifference(&start, &end) > 2000)  //rozhodovani o timeoutu
					{
						printf("%2d   %s   *\n", ttl, ip_address, timeDifference(&start, &end));
					}
					else
					{
						printf("%2d   %s   %.3f ms\n", ttl, ip_address, timeDifference(&start, &end));
					}
				}
			}

			if(error->ee_errno == ECONNREFUSED)
			{
				return 1;
			}
			else
			{
				return 0;
			}
		}
		return 0;
	}
}


//prochazi errorove zpravy od serveru
int errorCheck6(int sockfd, int ttl)
{
    struct iovec iov;  //struktura pro shromazdovani/rozptylovani vstupu a vystupu, pouziva se v msghr
    char address[MAGIC_CONST];  //retezec pouzity ve strukture iovec
    iov.iov_base = &address;  //adresa
    iov.iov_len = sizeof(address);  //pocet bytu na prenost

    char useless[MAGIC_CONST];  //retezec pro doprovodne data u zpravy
    struct msghdr small_header;  //jednotlive hlavicky se ziskavaji z cmshgr pomoci CMSG_FIRSTHDR a CMSG_NXTHDR
    small_header.msg_iov = &iov;  //shromazdovaci/rozptylovaci pole
    small_header.msg_iovlen = 1;  //clenove ve shromazdovacim poli
    small_header.msg_control = useless;  //doprovodne data
    small_header.msg_controllen = sizeof(useless);  //delka doprovodnych dat
    small_header.msg_flags = 0;  //flagy

    struct timeval end;  //casova struktura pro zaznamenani prijeti packetu

    int control;  //pomocna promenna pro kontrolu
    recvmsg(sockfd, &small_header, MSG_ERRQUEUE);  //vraci pocet ziskanych bytu, pokud doslo k chybe, snizi se ttl packet se posle znovu z mainu
    gettimeofday(&end, NULL);  //ziskani casu prijeti zpravy 

    struct cmsghdr *big_header;  //hlavicka pro data asociovane s datagramem, ziskavaji se z ni jednotive msghdr
    struct sock_extended_err *error;  //struktura, ktera se vyuziva pro ukladani erroru (prepinac IP_RECVERR)
    big_header = CMSG_FIRSTHDR(&small_header);
    while(big_header)
    {
        //print("%d\n", big_header->cmsg_level);
        if(big_header->cmsg_level == SOL_IPV6)
        {
            //printf("%d\n", big_header->cmsg_type);
            if(big_header->cmsg_type == IPV6_RECVERR)
            {
                //printf("nice\n");
                error = (struct sock_extended_err*) CMSG_DATA(big_header);
            }
            //printf("nice2\n");
        }
        //printf("nice3\n");
        big_header = CMSG_NXTHDR(&small_header, big_header);
    }

    if(error == NULL)  //zadna errorova zprava -> program dospel cile, predani rizeni mainu a konec
    {
        return 0;
    }
    else
    {
        //printf("t\n");
        if(error->ee_origin == SO_EE_ORIGIN_ICMP6)
        {
            //printf("z\n");
            char ip_address[MAGIC_CONST];
            //printf("u\n");
            struct sockaddr_in6 *sin = (struct sockaddr_in6*)(error+1);
            //printf("i\n");
            inet_ntop(AF_INET6, &sin->sin6_addr, ip_address, sizeof(ip_address));
            //printf("o\n");
            if (strcmp(lastIP, ip_address)==0)
            {
                return 1;
            }
            bzero(lastIP, MAGIC_CONST);
            strcpy(lastIP, ip_address);
            if (error->ee_type == 1)  //destination unrechable
            {
                if (error->ee_code == 0)  //network unreachable
                {
                    printf("%2d   %s   N!\n", ttl, ip_address);
                }
                else
                {
                    if (error->ee_code == 1)  //host unreachable
                    {
                        printf("%2d   %s   H!\n", ttl, ip_address);
                    }
                    else
                    {
                        if (error->ee_code == 2)  //protocol unreachable
                        {
                            printf("%2d   %s   P!\n", ttl, ip_address);
                        }
                        else
                        {
                            if (error->ee_code == 13)  //communication administratively prohibited
                            {
                                printf("%2d   %s   X!\n", ttl, ip_address);
                            }
                        }
                    }
                }
            }
            else
            {
                if (error->ee_type == 3)
                {
                    if (timeDifference(&start, &end) > 2000)  //rozhodovani o timeoutu
                    {
                        printf("%2d   %s   *\n", ttl, ip_address, timeDifference(&start, &end));
                    }
                    else
                    {
                        printf("%2d   %s   %.3f ms\n", ttl, ip_address, timeDifference(&start, &end));
                    }
                }
            }

            if(error->ee_errno == ECONNREFUSED)
            {
                return 1;
            }
            else
            {
                return 0;
            }
        }
        return 0;
    }
}


int main(int argc, char const *argv[])
{
	//kontrola argumentu
	arguments(argc, argv);
	
	//testovani spravnosti IP adresy a jejiho typu
	int control;  //promena pro kontrolu navratovych hodnot
	struct addrinfo hints;
	struct addrinfo *result;

	memset(&hints, 0, sizeof(struct addrinfo));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = 0;

	control = getaddrinfo(ip_address, NULL, &hints, &result);  //kontrola IP adrsy
	if (control != 0)
	{
		fprintf(stderr, "Chyba pri zpracovani IP adresy.(%s)\n", gai_strerror(control));
		exit(2);
	}

	//zjisteni o jaky typ IP adresy jde
	switch(result->ai_family)
	{
		case AF_INET:
			typeOfIP = 4;
			break;
		case AF_INET6:
			typeOfIP = 6;
			break;
		default:
			fprintf(stderr, "Neznamy typ IP adresy.(1)\n");
			exit(2);
			break;
	}

	//casova struktura pro timeout
	struct timeval timeout;
	timeout.tv_sec = 2;
	timeout.tv_usec = 0;
	
	//casova struktura pro zaznamenani prijekti socketu zpet
	struct timeval end;

	int sockfd;  //cislo socketu
	int ttl = first_ttl;  //pocitadlo aktualni hodnoty ttl

    //retezec pro data z funkce recvfrom a sendto, neni vyuzivany, ale je treba
    char useless[MAGIC_CONST] = {0};
    int one = 1;  //kvuli nutnemu ukazeli pro IP_RECVERR
    int counter = 0; //pocitadlo opakovaneho odeslani socketu, v pripade nepodareneho odeslani se pokus zopakuje jeste jednou

    if (typeOfIP == 4)  //IPv4
    {
        //nastaveni sitove adresy k cili
        struct sockaddr_in tovictim;
        tovictim.sin_family = AF_INET;
        tovictim.sin_port = htons(33434);

        in_addr_t adress = inet_addr(ip_address);
        tovictim.sin_addr.s_addr = adress;

        //nastaveni sitove adresy od cile
        struct sockaddr_in fromvictim;

        //vytvoreni socketu
        sockfd = socket(AF_INET, SOCK_DGRAM, 0);  //mozna misto nuly IPPROTO_ICMP, nula znamena obecny socket

        if (sockfd < 0)
        {
            fprintf(stderr, "Chyba pri vytvareni socketu.(1)(IPv4)\n");
            exit(3);
        }

        //nastaveni vlastnosti socketu
        control = setsockopt(sockfd, SOL_IP, IP_RECVERR, &one, sizeof(int));  //nastaveni prijimani erroru
        if (control != 0)
        {
            fprintf(stderr, "Chyba pri nastavovani vlastnosti socketu.(2)(IPv4)\n");
            exit(3);
        }

        //cyklus pro incrementaci tll a zasilani zprav cili
        for (; ttl < max_ttl; ttl++)
        {
            control = setsockopt(sockfd, SOL_IP, IP_TTL, &ttl, sizeof(int));  //nastaveni ttl
            if (control != 0)
            {
                fprintf(stderr, "Chyba pri nastavovani vlastnosti socketu.(ttl = %d)(4)(IPv4)\n", ttl);
                exit(3);
            }

            control = sendto(sockfd, &useless, sizeof(useless), 0, (struct sockaddr*)&tovictim, sizeof(struct sockaddr_in));  //nastaveni packetu
            if (control == -1)
            {
                if (!(counter < 2))  //pokud se neposlal packet ani na druhy pokus program konci s chybou
                {
                    fprintf(stderr, "Chyba pri odesilani socketu.(ttl = %d)(1)(IPv4)\n", ttl);
                    exit(4);
                }
            }
            else  //pri uspesnem odeslani se zaznamenava cas
            {
                gettimeofday(&start, NULL);
                counter = 0;
            }

            waitForSelect(sockfd, timeout);
            control = recvfrom(sockfd, useless, sizeof(useless), MSG_DONTWAIT, (struct sockaddr*)&fromvictim, (socklen_t*)sizeof(fromvictim));
            if(control > 0)  //control oznacuje pocet prijatych bytu, v pripade erroru je -1, pokud bylo zavreno spojeni hodnota je nula
            {
                gettimeofday(&end, NULL);
                printf("%2d   %s   %.3f ms\n", ttl, inet_ntoa(fromvictim.sin_addr), timeDifference(&start, &end));
                break;
            }
            else
            {
                control = errorCheck4(sockfd, ttl);
                switch (control)
                {
                    case 0:
                        break;
                    case 1:
                        return 0;
                }
            }
            counter++;
        }
    }
    else  //IPv6
    {
        struct sockaddr_in6 tovictim;
        tovictim.sin6_family = AF_INET6;
        tovictim.sin6_port = htons(33434);

        struct sockaddr_in6 adress;
        inet_pton(AF_INET6, ip_address, &(adress.sin6_addr));
        tovictim.sin6_addr = adress.sin6_addr;

        //nastaveni sitove adresy od cile
        struct sockaddr_in6 fromvictim;

        //vytvoreni socketu
        sockfd = socket(AF_INET6, SOCK_DGRAM, 0);  //mozna misto nuly IPPROTO_ICMP, nula znamena obecny socket

        if (sockfd < 0)
        {
            fprintf(stderr, "Chyba pri vytvareni socketu.(1)(IPv6)\n");
            exit(3);
        }

        //nastaveni vlastnosti socketu
        control = setsockopt(sockfd, SOL_IPV6, IPV6_RECVERR, &one, sizeof(int));  //nastaveni prijimani erroru
        if (control != 0)
        {
            fprintf(stderr, "Chyba pri nastavovani vlastnosti socketu.(2)(IPv6)\n");
            exit(3);
        }

        //cyklus pro incrementaci tll a zasilani zprav cili
        for (; ttl < max_ttl; ttl++)
        {
            control = setsockopt(sockfd, SOL_IPV6, IPV6_UNICAST_HOPS, &ttl, sizeof(int));  //nastaveni ttl
            if (control != 0)
            {
                fprintf(stderr, "Chyba pri nastavovani vlastnosti socketu.(ttl = %d)(4)(IPv6)\n", ttl);
                exit(3);
            }

            control = sendto(sockfd, &useless, sizeof(useless), 0, (struct sockaddr*)&tovictim, sizeof(struct sockaddr_in6));  //nastaveni packetu
            if (control == -1)
            {
                if (!(counter < 2))  //pokud se neposlal packet ani na druhy pokus program konci s chybou
                {
                    fprintf(stderr, "Chyba pri odesilani socketu.(ttl = %d)(1)(IPv6)\n", ttl);
                    exit(4);
                }
            }
            else  //pri uspesnem odeslani se zaznamenava cas
            {
                gettimeofday(&start, NULL);
                counter = 0;
            }

            waitForSelect(sockfd, timeout);
            control = recvfrom(sockfd, useless, sizeof(useless), MSG_DONTWAIT, (struct sockaddr*)&fromvictim, (socklen_t*)sizeof(fromvictim));
            if(control > 0)  //control oznacuje pocet prijatych bytu, v pripade erroru je -1, pokud bylo zavreno spojeni hodnota je nula
            {
                gettimeofday(&end, NULL);
                inet_ntop(AF_INET6, &(fromvictim.sin6_addr), useless, sizeof(useless));
                printf("%2d   %s   %.3f ms\n", ttl, useless, timeDifference(&start, &end));
                break;
            }
            else
            {
                control = errorCheck6(sockfd, ttl);
                //control = 0;
                switch (control)
                {
                    case 0:
                        break;
                    case 1:
                        return 0;
                }
            }
            counter++;
        }
    }
	return 0;
}
