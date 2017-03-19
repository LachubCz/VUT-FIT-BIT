#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include <stdlib.h>
#include <netinet/in.h>

#define MAX_PATH 1024 //maximalni delka path
#define BUFFER 2048 //velikost dat nacitanych ze socketu do bufferu

int PORT; 
char ROOT_FOLDER[MAX_PATH];

int number_from_string(const char *str)
{
	int i = 0, number;
	
	while(i < strlen(str))
	{
		if (!isdigit(str[i]))
		{
			fprintf(stderr, "Chyba pri zadani argumentu.\n");
			exit(1);
		}
		i++;
	}

	number = atoi(str);
	return number;
}

//TODO osetrit vetsi nez 1024 delka path; osetrit stav kdyz je jeden parametr zadan dvakrat
void arguments(int argc, char const *argv[])
{
	PORT = 6677;
	getcwd(ROOT_FOLDER, sizeof(ROOT_FOLDER));

	if (argc == 1)
	{
		return;
	}
	else if (argc == 3)
		{
			if (!strcmp(argv[1], "-r"))
			{
				strcpy(ROOT_FOLDER, argv[2]);
			}
			else if (!strcmp(argv[1], "-p"))
				{
					PORT = number_from_string(argv[2]);
				}
			else
			{
				fprintf(stderr, "Chyba pri zadani argumentu.\n");
				exit(1);
			}
		}
	else if (argc == 5)
		{
			if (!strcmp(argv[1], "-r"))
			{
				strcpy(ROOT_FOLDER, argv[2]);
			}
			else if (!strcmp(argv[1], "-p"))
				{
					PORT = number_from_string(argv[2]);
				}
			else
			{
				fprintf(stderr, "Chyba pri zadani argumentu.\n");
				exit(1);
			}

			if (!strcmp(argv[3], "-r"))
			{
				strcpy(ROOT_FOLDER, argv[4]);
			}
			else if (!strcmp(argv[3], "-p"))
				{
					PORT = number_from_string(argv[4]);
				}
			else
			{
				fprintf(stderr, "Chyba pri zadani argumentu.\n");
				exit(1);
			}
		}
	else
	{
		fprintf(stderr, "Chyba pri zadani argumentu.\n");
		exit(1);
	}

	return;
}

void send_state(int state, int socket_fd2)
{
	char message[17];
	bzero(message, 17);

	switch (state)
	{
		case 200:
			strcpy(message, "200 OK");
			break;
		case 404:
			strcpy(message, "404 Not Found");
			break;
		case 400:
			strcpy(message, "400 Bad Request");
			break;
	}

	if ((write(socket_fd2, message, strlen(message))) < 0)
	{
		fprintf(stderr, "Chyba pri zapisovani do socketu.\n");
		exit(6);
	}
}

int main(int argc, char const *argv[])
{
	int socket_fd, socket_fd2;

	char buffer[BUFFER];
	bzero(buffer, BUFFER);

	struct sockaddr_in server_addr, client_addr; //sockaddr_in je struktura obsahujici internetovou adresu
	
	arguments(argc, argv); //funkce na zpracovani argumentu

	socket_fd = socket(AF_INET, SOCK_STREAM, 0); //inicializace socketu
	if (socket_fd < 0) 
	{
		fprintf(stderr, "Chyba pri otvirani socketu.\n");
		exit(2);
	}

	bzero((char *) &server_addr, sizeof(server_addr)); //nastavi vsechny hodnoty v bufferu na nulu

	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = INADDR_ANY;
	server_addr.sin_port = htons(PORT);

	if ((bind(socket_fd, (struct sockaddr *) &server_addr, sizeof(server_addr))) < 0)
	{
		fprintf(stderr, "Chyba pri bindovani socketu.\n");
		exit(3);
	}

	listen(socket_fd, 5);

	int cl_addr_size = sizeof(client_addr);
	socket_fd2 = accept(socket_fd, (struct sockaddr *) &client_addr, &cl_addr_size);
	if (socket_fd2 < 0)
	{
		fprintf(stderr, "Chyba pri prijmani socketu.\n");
		exit(4);
	}

	if ((read(socket_fd2, buffer, BUFFER)) < 0)
	{
		fprintf(stderr, "Chyba pri cteni ze socketu.\n");
		exit(5);
	}

	printf("\n%s\n", buffer);

	send_state(200, socket_fd2);

	return 0;
}
