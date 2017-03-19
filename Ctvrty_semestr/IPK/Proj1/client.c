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
	bzero(LOCAL_PATH, MAX_PATH);

	if (argc > 4 || argc < 3)
	{
		fprintf(stderr, "Chyba pri zadani argumentu.(1)\n");
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
			fprintf(stderr, "Chyba pri zadani argumentu.(2)\n");
			exit(1);
		}
		else
		{
			strcpy(LOCAL_PATH, argv[3]);
		}
	}

	strcpy(REMOTE_PATH, argv[2]);

	if (strcmp(COMMAND, "GET") == 0 && fourth_argument)
	{
		strcpy(LOCAL_PATH, argv[3]);
	}

	if ((strcmp(COMMAND, "GET") != 0 && strcmp(COMMAND, "PUT") != 0 ) && fourth_argument)
	{
		fprintf(stderr, "Chyba pri zadani argumentu.(3)\n");
		exit(1);
	}
}

void getPort(char **Port)
{
	char Balast[BUFFER];
	char *temp = malloc(sizeof(char) * BUFFER);

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
	char *temp = malloc(sizeof(char) * BUFFER);

	if (sscanf(REMOTE_PATH, "http://%s", Balast))
	{
		;//error handling
	}

	*ServerName = strtok(Balast, ":");
	sprintf(temp, "%s", *ServerName);
	*ServerName = temp;
	free(temp);
}

void getPath(char **Path, int loc_or_rem)
{
	char Balast[BUFFER];
	char *temp = malloc(sizeof(char) * BUFFER);

	if (loc_or_rem == 0)
	{
		if (sscanf(REMOTE_PATH, "http://%s", Balast))
		{
			;//error handling
		}
	}
	else
	{
		if (sscanf(LOCAL_PATH, "http://%s", Balast))
		{
			;//error handling
		}
	}


	*Path = strtok(Balast, ":");
	*Path = strtok(NULL,"/");
	*Path = strtok(NULL, "\0");
	sprintf(temp, "/%s", *Path);
	*Path = temp;
	free(temp);
}

char * getHeader (char *ServerName, char *Path, int fof, int binary_file_len)
{
	char *type = "?.type=";

	char *file;

	if (fof == 1)
	{
		file = "file";
	}
	else
	{
		file = "folder";
	}
	
	char *http = "HTTP/1.1";
	char *host = "Host: ";
	char *date = "Date: ";
	char *accept = "Accept: application/json";
	char *accept_en = "Accept-Encoding: identity";
	char *con_type = "Content-Type: application/octet-stream";
	char *con_len = "Content-Length: ";

	char *tempPath = malloc(strlen(Path) * sizeof(char));
	tempPath = Path;
	
	time_t rawtime;
	struct tm * timeinfo;
	char act_time[64];
	time (&rawtime);
	timeinfo = localtime (&rawtime);
	strftime (act_time, 64, "%a, %d %b %G %H:%M:%S CET", timeinfo);

	int length = strlen(COMMAND) + strlen(Path) + strlen(type) + strlen(file) + strlen(http) + strlen(host) + strlen(ServerName) + strlen(date) + strlen(act_time) + strlen(accept) + strlen(accept_en) + strlen(con_type) + strlen(con_len);
	char *Header = malloc(length * sizeof(char) + 16 * sizeof(char));
	if (LOCAL_PATH[0] == '\0')
	{
		sprintf(Header, "%s %s%s%s %s\n%s%s\n%s%s\n%s\n%s\n%s\n", COMMAND, Path, type, file, http, host, ServerName, date, act_time, accept, accept_en, con_type);
	}
	else
	{
		sprintf(Header, "%s %s%s%s %s\n%s%s\n%s%s\n%s\n%s\n%s\n%s%d\n", COMMAND, Path, type, file, http, host, ServerName, date, act_time, accept, accept_en, con_type, con_len, binary_file_len);
	}
	
	return Header;
}

char * getFile()
{
	char *ptr = LOCAL_PATH;
	char *buffer = 0;
	long length;
	FILE *file = fopen (ptr, "rb");

	if (file != NULL)
	{
		fseek (file, 0, SEEK_END);
		length = ftell (file);
		fseek (file, 0, SEEK_SET);
		buffer = malloc (sizeof(char)*length);
		if (buffer)
		{
			fread (buffer, 1, length, file);
		}
		fclose (file);
	}
	else
	{
		fprintf(stderr, "Soubor nenalezen.\n");
		exit(7);
	}

	return buffer;
}

int file_or_folder(char *Path, int mode)
{
	char *PathTemp = malloc(sizeof(char)*strlen(Path) + 16);
	strcpy(PathTemp, Path);
	char *actualPath = NULL; 
	char *tempSave = NULL;
	actualPath = strtok(PathTemp,"/");
	
	while(actualPath != NULL)
	{
		tempSave = actualPath;
		actualPath = strtok(NULL,"/");
	}

	int len = strlen(tempSave);
	for(int i = 0; i < len; i++) 
	{
		if(tempSave[i] == '.') 
		{
			return 1;  //jedna pro file
		}
    }
    return 0; //pro folder
}

int main(int argc, char const *argv[])
{
	int socket_fd;
	int binary_file_len = 0;
	char buffer[BUFFER];
	char *binary_file;
	char *final_message;
	bzero(buffer, BUFFER);
	struct sockaddr_in server_addr;
	struct hostent *server;


	arguments(argc, argv);


	char *Port = malloc(sizeof(char) * BUFFER);
	getPort(&Port);
	char *ServerName = malloc(sizeof(char) * BUFFER);
	getServerName(&ServerName);
	char *Path = malloc(sizeof(char) * BUFFER);
	getPath(&Path, 0);
	char *LocalPath = malloc(sizeof(char) * BUFFER);
	getPath(&LocalPath, 1);


	int fof = file_or_folder(Path, 0);


	if (strcmp(COMMAND, "DEL") == 0)
	{
		if (fof == 0)
		{
			fprintf(stderr, "Nejedna se o soubor, ale o slozku.(1)\n");
			exit(9);
		}
	}
	else if (strcmp(COMMAND, "GET") == 0)
		{
			if (fof == 0)
			{
				fprintf(stderr, "Nejedna se o soubor, ale o slozku.(2)\n");
				exit(9);
			}
		}
	else if (strcmp(COMMAND, "PUT") == 0)
		{
			if (fof == 1)
			{
				fprintf(stderr, "Nejedna se o slozku, ale o soubor.(4)\n");
				exit(9);
			}

			binary_file = getFile();
			binary_file_len = strlen(binary_file);
		}
	else if (strcmp(COMMAND, "LST") == 0)
		{
			if (fof == 1)
			{
				fprintf(stderr, "Nejedna se o slozku, ale o soubor.(1)\n");
				exit(8);
			}
		}
	else if (strcmp(COMMAND, "MKD") == 0)
		{
			if (fof == 1)
			{
				fprintf(stderr, "Nejedna se o slozku, ale o soubor.(2)\n");
				exit(8);
			}
		}
	else if (strcmp(COMMAND, "RMD") == 0)
		{
			if (fof == 1)
			{
				fprintf(stderr, "Nejedna se o slozku, ale o soubor.(3)\n");
				exit(8);
			}
		}


	char *header = getHeader(ServerName, Path, fof, binary_file_len);  //opet vznika problem s path

	if (argc == 3)
	{
		final_message = malloc(strlen(header)*sizeof(char));
		sprintf(final_message, "%s", header);
	}
	else
	{
		final_message = malloc(strlen(header)*sizeof(char) + strlen(binary_file) * sizeof(char));
		sprintf(final_message, "%s\n%s", header, binary_file);
	}
	


	//inicializace socketu
	socket_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (socket_fd < 0) 
	{
		fprintf(stderr, "Chyba pri otvirani socketu.\n");
		exit(2);
	}



	//gethostbyname
	server = gethostbyname(ServerName);
	if (server == NULL)
	{
		fprintf(stderr, "Server neexituje.\n");
		exit(3);
	}



	//connect
	bzero((char *) &server_addr, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	bcopy((char *) server->h_addr, (char *) &server_addr.sin_addr.s_addr, server->h_length);
	int port = atoi(Port);
	server_addr.sin_port = htons(port);
	if ((connect(socket_fd, (struct sockaddr *) &server_addr, sizeof(server_addr))) < 0)
	{
		fprintf(stderr, "K serveru se nelze pripojit.\n");
		exit(4);
	}



	//odesilani dat na server
	if ((write(socket_fd, final_message, strlen(final_message))) < 0)
	{
		fprintf(stderr, "Chyba pri zapisovani do socketu.\n");
		exit(5);
	}



	//prijmani dat zpet
	bzero(buffer, BUFFER);
	if ((read(socket_fd, buffer, BUFFER)) < 0)
	{
		fprintf(stderr, "Chyba pri cteni ze socketu.\n");
		exit(6);
	}

	printf("\n%s\n", buffer);
	return 0;
}
