#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdbool.h>
#include <netinet/in.h>
#include <netdb.h>
#include <time.h>

#define MAX_PATH 1024

//globalni promenne pro ulozeni argumentu
int first_ttl = 1;
int max_ttl = 30;
char ip_address[MAX_PATH];

//TODO kontrola spravnosti IP adresy
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
	printf("%d %d %s\n", first_ttl, max_ttl, ip_address);

	return 0;
}