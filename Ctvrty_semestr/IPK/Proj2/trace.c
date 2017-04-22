#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdbool.h>
#include <netinet/in.h>
#include <netdb.h>
#include <time.h>
#include <arpa/inet.h>

#define MAX_PATH 1024

//globalni promenne pro ulozeni argumentu
int first_ttl = 1;
int max_ttl = 30;
char ip_address[MAX_PATH];

//TODO: kontrola spravnosti IP adresy, kontrola zdali first_ttl, max_ttl obsahuji cisla
//kontrola spravnosti argumentu
void arguments (int argc, char const *argv[])
{
	bzero(ip_address, MAX_PATH);

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

int main(int argc, char const *argv[])
{
	//kontrola argumentu
	arguments(argc, argv);
	//printf("%d %d %s\n", first_ttl, max_ttl, ip_address);
	
	//struktura pro cas
	struct timeval timeout;
	//memset(&timeout, 0, sizeof(struct timeval)); //mozna bude treba funkce bzero nebo memset
	timeout.tv_sec = 2;
	timeout.tv_usec = 0;
	
	int sockfd;
	int control;
	int ttl = first_ttl;

	struct sockaddr_in tovictim;
	//memset(&tovictim, 0, sizeof(struct sockaddr_in)); //mozna bude treba funkce bzero nebo memset
	tovictim.sin_family = AF_INET;
	tovictim.sin_port = htons(33434);

	in_addr_t adress = inet_addr(ip_address);
	tovictim.sin_addr.s_addr = adress;

	struct sockaddr_in fromvictim;
	memset(&from, 0, sizeof(struct sockaddr_in));

	//vytvoreni socketu
	sockfd = socket(AF_INET, SOCK_DGRAM, 0);  //mozna misto nuly IPPROTO_ICMP, nula znamena obecny socket
	if (sockfd < 0)
	{
		fprintf(stderr, "Chyba pri vytvareni socketu.(1)\n");
		exit(2);
	}

	//nastaveni options socketu
	control = setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(struct timeval));  //nastaveni timeoutu
	if (control < 0)  //mozna zde ma byt vetsi nez nula
	{
		fprintf(stderr, "Chyba pri nastavovani vlastnosti socketu.(1)\n");
		exit(2);
	}

	int one = 1;
	control = setsockopt(sockfd, SOL_IP, IP_RECVERR, &one, sizeof(int));  //zjistit co presne je one 
	if (control != 0)
	{
		fprintf(stderr, "Chyba pri nastavovani vlastnosti socketu.(2)\n");
		exit(2);
	}

	control = setsockopt(sockfd, SOL_IP, IP_RECVTTL, &one, sizeof(int));
	if (control != 0)
	{
		fprintf(stderr, "Chyba pri nastavovani vlastnosti socketu.(3)\n");
		exit(2);
	}

	for (; ttl < max_ttl; ttl++)
	{
		control = setsockopt(sockfd, SOL_IP, IP_TTL, &ttl, sizeof(int));
		if (control != 0)
		{
			fprintf(stderr, "Chyba pri nastavovani vlastnosti socketu.(ttl = %d)(4)\n", ttl);
			exit(2);
		}

		//memset(&from, 0, sizeof(struct sockaddr_in));
		//target.sin_port = htons(port + ttl);
		if(sendto(sockfd, &data, sizeof(data), 0, (struct sockaddr*)&target, sizeof(target)) == -1) //chce to odstinit data
		{
			fprintf(stderr, "Chyba pri odesilani socketu.(ttl = %d)(4)\n", ttl);
			exit(3);
		}

		//wait_recv(fd);
		if(recvfrom(sockfd, buf, sizeof(buf), MSG_DONTWAIT, (struct sockaddr*)&from, (socklen_t*)sizeof(from)) > 0) //chce to odstinit buf
		{
			printf("%2d : %s \n", ttl, inet_ntoa(from.sin_addr));
			return 0;
		}

		return proc_error(fd, ttl); //dodelat fci procerror

	}
	return 0;
}
