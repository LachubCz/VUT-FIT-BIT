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
#define BUFFER 2048 //velikost dat nacitanych ze socketu do bufferu

char COMMAND[4];
char REMOTE_PATH[MAX_PATH];
char LOCAL_PATH[MAX_PATH];

void err_print(int err_code)
{
	switch(err_code)
	{
		case 2:
			fprintf(stderr, "Not a directory.\n");
			exit(2);
		case 3:
			fprintf(stderr, "Directory not found.\n");
			exit(3);
		case 4:
			fprintf(stderr, "Directory not empty.\n");
			exit(4);
		case 5:
			fprintf(stderr, "Already exists.\n");
			exit(5);
		case 6:
			fprintf(stderr, "Not a file.\n");
			exit(6);
		case 7:
			fprintf(stderr, "File not found.\n");
			exit(7);
		case 8:
			fprintf(stderr, "Unknown error.\n");
			exit(8);
	}
}

int check_command(const char *cmd)
{
	if (strcmp(cmd, "del") == 0)
	{
		strcpy(COMMAND, "DEL");
		return 0;
	}
	else if (strcmp(cmd, "get") == 0)
	{
		strcpy(COMMAND, "GET");
		return 1;
	}
	else if (strcmp(cmd, "put") == 0)
	{
		strcpy(COMMAND, "PUT");
		return 2;
	}	
	else if (strcmp(cmd, "lst") == 0)
	{
		strcpy(COMMAND, "LST");
		return 3;
	}	
	else if (strcmp(cmd, "mkd") == 0)
	{
		strcpy(COMMAND, "MKD");
		return 4;
	}	
	else if (strcmp(cmd, "rmd") == 0)
	{
		strcpy(COMMAND, "RMD");
		return 5;
	}
	else
	{
		exit(1);
	}
}

void arguments (int argc, char const *argv[])
{
	bool fourth_argument;

	if (argc > 4 || argc < 3)
	{
		fprintf(stderr, "Chyba pri zadani argumentu.\n");
		exit(1);
	}

	if (argc == 4)
	{
		fourth_argument = true;
	}
	else
	{
		fourth_argument = false;
	}

	if (check_command(argv[1]) == 2)
	{
		if (!fourth_argument)
		{
			fprintf(stderr, "Chyba pri zadani argumentu.\n");
			exit(1);
		}
		else
		{
			strcpy(LOCAL_PATH, argv[3]);
		}
	}

	strcpy(REMOTE_PATH, argv[2]);

	if (strcmp(COMMAND, "get") == 0 && fourth_argument)
	{
		strcpy(LOCAL_PATH, argv[3]);
	}

	if (strcmp(COMMAND, "get") != 0 && strcmp(COMMAND, "put") != 0 && fourth_argument)
	{
		fprintf(stderr, "Chyba pri zadani argumentu.\n");
		exit(1);
	}
}

void getPort(char **Port)
{
	char Balast[BUFFER];
	char *temp = malloc(BUFFER);

	if (sscanf(REMOTE_PATH, "http://%s", Balast))
	{
		;//error handling
	}

	*Port = strtok(Balast, ":");
	*Port = strtok(NULL,"/");
	sprintf(temp, "%s", *Port);
	*Port = temp;
	free(temp);
}

void getServerName(char **ServerName)
{
	char Balast[BUFFER];
	char *temp = malloc(BUFFER);

	if (sscanf(REMOTE_PATH, "http://%s", Balast))
	{
		;//error handling
	}

	*ServerName = strtok(Balast, ":");
	sprintf(temp, "%s", *ServerName);
	*ServerName = temp;
	free(temp);
}

void getPath(char **Path)
{
	char Balast[BUFFER];
	char *temp = malloc(BUFFER);

	if (sscanf(REMOTE_PATH, "http://%s", Balast))
	{
		;//error handling
	}

	*Path = strtok(Balast, ":");
	*Path = strtok(NULL,"/");
	*Path = strtok(NULL, "\0");
	sprintf(temp, "/%s", *Path);
	*Path = temp;
	free(temp);
}

void getTime(char **act_time)
{
	time_t rawtime;
	struct tm * timeinfo;

	time (&rawtime);
	timeinfo = localtime (&rawtime);

	strftime (*act_time, 64, "CET %a, %d %b %G %H:%M:%S", timeinfo);
}

void getHeader (char *ServerName, char *Path, char *act_time)
{
	char *type = "?.type=";
	char *http = "HTTP/1.1";
	char *file = "file"; //nutne zmenit podle funkcniho zjistovani
	char *host = "Host: ";
	char *date = "Date: ";
	char *accept = "Accept: application/json";
	char *accept_en = "Accept-Encoding: identity";
	char *con_type = "Content-Type: application/octet-stream";
	char *con_len = "Content-Length: ";

	printf("%s %s%s%s %s\n%s%s\n%s%s\n%s\n%s\n%s\n%s\n", COMMAND, Path, type, file, http, host, ServerName, date, act_time, accept, accept_en, con_type, con_len);
}

int main(int argc, char const *argv[])
{
	int socket_fd;

	char buffer[BUFFER];
	bzero(buffer, BUFFER);

	struct sockaddr_in server_addr;
	struct hostent *server;

	arguments(argc, argv);

	char *Port = malloc(BUFFER);
	getPort(&Port);

	char *ServerName = malloc(BUFFER);
	getServerName(&ServerName);

	char *Path = malloc(BUFFER);
	getPath(&Path);

	char *act_time = malloc(BUFFER);
	getTime(&act_time);

	getHeader(ServerName, Path, act_time);

	socket_fd = socket(AF_INET, SOCK_STREAM, 0); //inicializace socketu
	if (socket_fd < 0) 
	{
		fprintf(stderr, "Chyba pri otvirani socketu.\n");
		exit(2);
	}

	server = gethostbyname(ServerName);
	if (server == NULL)
	{
		fprintf(stderr, "Server neexituje.\n");
		exit(3);
	}

	bzero((char *) &server_addr, sizeof(server_addr));

	server_addr.sin_family = AF_INET;

	bcopy((char *) server->h_addr, (char *) &server_addr.sin_addr.s_addr, server->h_length);

	int port = *Port;

	server_addr.sin_port = htons(port);

	if ((connect(socket_fd, (struct sockaddr *) &server_addr, sizeof(server_addr))) < 0)
	{
		fprintf(stderr, "K serveru se nelze pripojit.\n");
		exit(4);
	}

	printf("Napis Zpravu: ");
	fgets(buffer, BUFFER, stdin);

	if ((write(socket_fd, buffer, strlen(buffer))) < 0)
	{
		fprintf(stderr, "Chyba pri zapisovani do socketu.\n");
		exit(5);
	}

	bzero(buffer, BUFFER);

	if ((read(socket_fd, buffer, BUFFER)) < 0)
	{
		fprintf(stderr, "Chyba pri cteni ze socketu.\n");
		exit(6);
	}

	//printf("%s", buffer);

	return 0;
}
