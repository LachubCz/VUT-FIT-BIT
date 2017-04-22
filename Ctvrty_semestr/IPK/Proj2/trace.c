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
int max_ttl = 30;
char ip_address[MAGIC_CONST];

//TODO: kontrola spravnosti IP adresy, kontrola zdali first_ttl, max_ttl obsahuji cisla
//kontrola spravnosti argumentu
void arguments (int argc, char const *argv[])
{
	bzero(ip_address, MAGIC_CONST);

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

void wait_recv(int fd){
    fd_set fds;
    struct timeval tv;
    FD_ZERO(&fds);
    FD_SET(fd, &fds);
    tv.tv_sec = 1;
    tv.tv_usec = 0;
    select(fd+1, &fds, NULL, NULL, &tv);
}

int proc_error(int fd, int ttl){
    char cbuf[512];
    struct msghdr msg;
    struct cmsghdr *cmsg;
    struct iovec iov;
    struct sock_extended_err *e;
    struct sockaddr_in addr;
    struct timeval tv;
    char rcvbuf[80];
    int rst, rethops;

    iov.iov_base = &rcvbuf;
    iov.iov_len = sizeof(rcvbuf);
    msg.msg_name = (unsigned char*)&addr;
    msg.msg_namelen = sizeof(addr);
    msg.msg_iov = &iov;
    msg.msg_iovlen = 1;
    msg.msg_flags = 0;
    msg.msg_control = cbuf;
    msg.msg_controllen = sizeof(cbuf);

    gettimeofday(&tv, NULL);
    rst = recvmsg(fd, &msg, MSG_ERRQUEUE);
    if(rst < 0){
        //printf("%2d: %s \n", ttl, "no reply");
        return -1;
    }

    for(cmsg = CMSG_FIRSTHDR(&msg); cmsg; cmsg = CMSG_NXTHDR(&msg, cmsg)){
        if(cmsg->cmsg_level == SOL_IP){
            if(cmsg->cmsg_type == IP_RECVERR){
                e = (struct sock_extended_err*) CMSG_DATA(cmsg);
            }
            else if(cmsg->cmsg_type == IP_TTL){
                memcpy(&rethops, CMSG_DATA(cmsg), sizeof(rethops));
            }
        }
    }

    if(e == NULL){
        return 0;
    }

    if(e->ee_origin == SO_EE_ORIGIN_LOCAL){
        printf("%2d: %s \n", ttl, "[LOCALHOST]");
        return 0;
    }
    else if(e->ee_origin == SO_EE_ORIGIN_ICMP){
        char abuf[128];
        struct sockaddr_in *sin = (struct sockaddr_in*)(e+1);
        inet_ntop(AF_INET, &sin->sin_addr, abuf, sizeof(abuf));
        printf("%2d: %s \n", ttl, abuf);
        if(e->ee_errno == ECONNREFUSED){
            return 1;
        }
        return 0;
    }
    return 0;
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
	memset(&tovictim, 0, sizeof(struct sockaddr_in)); //mozna bude treba funkce bzero nebo memset
	tovictim.sin_family = AF_INET;
	tovictim.sin_port = htons(33434);

	in_addr_t adress = inet_addr(ip_address);
	tovictim.sin_addr.s_addr = adress;

	struct sockaddr_in fromvictim;
	memset(&fromvictim, 0, sizeof(struct sockaddr_in));

	char useless[MAGIC_CONST] = {0};

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
		int counter = 0; //pocitadlo opakovaneho odeslani socketu
		control = setsockopt(sockfd, SOL_IP, IP_TTL, &ttl, sizeof(int));
		if (control != 0)
		{
			fprintf(stderr, "Chyba pri nastavovani vlastnosti socketu.(ttl = %d)(4)\n", ttl);
			exit(2);
		}

		//memset(&from, 0, sizeof(struct sockaddr_in));
		//target.sin_port = htons(port + ttl);

		control = sendto(sockfd, &useless, sizeof(useless), 0, (struct sockaddr*)&tovictim, sizeof(tovictim));
		if (control == -1) //chce to odstinit data
		{
			if (!(counter < 2))
			{
				fprintf(stderr, "Chyba pri odesilani socketu.(ttl = %d)(4)\n", ttl);
				exit(3);
			}
		}

		
		wait_recv(sockfd);
		
		control = recvfrom(sockfd, useless, sizeof(useless), MSG_DONTWAIT, (struct sockaddr*)&fromvictim, (socklen_t*)sizeof(fromvictim)); //chce to odstinit buf
		if(control > 0)
		{
			printf("%2d : %s \n", ttl, inet_ntoa(fromvictim.sin_addr));
			return 0;
		}
		else
		{
			control = proc_error(sockfd, ttl);
			switch (control)
			{
				case -1:
					ttl--;
				case 0:
					break;
				case 1:
					return 0;
			}
		}

		

		//printf("%d\n", ttl);
		/*
		else
		{
			fprintf(stderr, "END(ttl = %d)\n", ttl);
			exit(0);
		}
 		*/
		//return proc_error(fd, ttl); //dodelat fci procerror
		counter++;
	}
	return 0;
}
