#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <sys/stat.h>
#include <time.h>
#include <stdbool.h>

#define MAX_PATH 1024 //maximalni delka path
#define BUFFER 2048 //velikost dat nacitanych ze socketu do bufferu

//TODO navratove hodnoty anglicky
//globalni promenne pro ulozeni argumentu
int PORT; 
char ROOT_FOLDER[MAX_PATH];

//struktura pro praci s prijatou hlavicku
typedef struct {
	char *command;
	char *path;
	char *type;
	char *host;
	char *date;
	char *accept;
	char *accept_en;
	char *con_type;
	char *con_len;
} Header;

//kontrola cisla portu a jeho prevedeni na int
int number_from_string(const char *str)
{
	int i = 0, number;
	
	while(i < strlen(str))
	{
		if (!isdigit(str[i]))
		{
			fprintf(stderr, "Argument error.\n");
			exit(1);
		}
		i++;
	}

	number = atoi(str);
	return number;
}

//TODO osetrit vetsi nez 1024 delka path; osetrit stav kdyz je jeden parametr zadan dvakrat
//kontrola argumentu serveru a jejich nahrani do globalnich promennych
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
				fprintf(stderr, "Argument error.\n");
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
				fprintf(stderr, "Argument error.\n");
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
				fprintf(stderr, "Argument error.\n");
				exit(1);
			}
		}
	else
	{
		fprintf(stderr, "Argument error.\n");
		exit(1);
	}

	return;
}

//ziskani hlavicky, ktera se odesila zpet klientovi
char * getHeader (int message, int num_con_len)
{
	char *ret_text;
	if (message == 0)
	{
		ret_text = "200 OK";
	}
	else if (message == 1)
		{
			ret_text = "404 Not Found";
		}
	else if (message == 2)
		{
			ret_text = "400 Bad Request";
		}

	char str_con_len[16];
	sprintf(str_con_len, "%d", num_con_len);

	char *http = "HTTP/1.1 ";
	char *date = "Date: ";
	char *con_type = "Content-Type: application/json";
	char *con_len = "Content-Length: ";
	char *con_en = "Content-Encoding: identity";

	time_t rawtime;
	struct tm * timeinfo;
	char act_time[64];
	time (&rawtime);
	timeinfo = localtime (&rawtime);
	strftime (act_time, 64, "%a, %d %b %G %H:%M:%S CET", timeinfo);

	int length = strlen(http) + strlen(ret_text) + strlen(date) + strlen(act_time) + strlen(con_type) + strlen(con_len) + strlen(str_con_len) + strlen(con_en);
	
	char *Header = malloc(length * sizeof(char) + 16 * sizeof(char));

	sprintf(Header, "%s%s\n%s%s\n%s\n%s%s\n%s\n", http, ret_text, date, act_time, con_type, con_len, str_con_len, con_en);
	
	return Header;
}

//odeslani hlavicky/hlavicky se souborem klientovi zpet pres socket
void send_state(int socket_fd2, char *message)
{
	if ((write(socket_fd2, message, strlen(message))) < 0)
	{
		fprintf(stderr, "Chyba pri zapisovani do socketu.\n");
		exit(6);
	}
}

//TODO vyhazovani chyb podle errno
//operace smazat soubor
void DEL(Header *header, int socket_fd2)
{
	char *path = malloc(MAX_PATH*sizeof(char));
	sprintf(path,"%s%s", ROOT_FOLDER, header->path);

	int errCheck = remove(path);
	if (errCheck == -1)  //doplnit errno
	{
		char *header_str = getHeader(1, 0);
		send_state(socket_fd2, header_str);
	}
	else
	{
		char *header_str = getHeader(0, 0);
		send_state(socket_fd2, header_str);
	}
	free(path);
}

//TODO vyhazovani chyb podle errno
//operace ziskat soubor
void GET(Header *header, int socket_fd2)
{
	char *path = malloc(MAX_PATH*sizeof(char));
	sprintf(path,"%s%s", ROOT_FOLDER, header->path);
	
	char *ptr = path;
	char *buffer = 0;
	long length;
	char *final_message;
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
		fprintf(stderr, "File not found.\n");
		exit(7);
	}

	int len = strlen(buffer);
	char *header_str = getHeader(0, len);
	final_message = malloc(strlen(header_str)*sizeof(char) + strlen(buffer) * sizeof(char) + 16 * sizeof(char));

	sprintf(final_message, "%s%s", header_str, buffer);
	send_state(socket_fd2, final_message);

	free(path);
}

//TODO vyhazovani chyb podle errno
//operace vlozit soubor
void PUT(Header *header, int socket_fd2)
{
	int length = atoi (header->con_len);
	char *binary_file = malloc(sizeof(char)*length + 16 * sizeof(char));

	if (length == 0)
	{
		binary_file = "";
	}
	else
	{
		if (read(socket_fd2, binary_file, length) < 0)
		{
			fprintf(stderr, "Chyba pri cteni ze socketu.\n");
			exit(5);
		}
	}

	char *path = malloc(MAX_PATH*sizeof(char));
	sprintf(path,"%s%s", ROOT_FOLDER, header->path);

	FILE *file = fopen(path, "w");
	if (file == NULL)
	{
		fprintf(stderr, "Chyba pri zapisu souboru.\n");
		exit(10);
	}

	fprintf(file, "%s", binary_file);
	fclose(file);

	char *header_str = getHeader(0, 0);
	send_state(socket_fd2, header_str);

	free(path);
}

//TODO cela funkce
//operace vypsani obsahu adresare
void LST(Header *header, int socket_fd2)
{
	char *path = malloc(MAX_PATH*sizeof(char));
	sprintf(path,"%s%s", ROOT_FOLDER, header->path);
	
	char *header_str = getHeader(2, 0);
	send_state(socket_fd2, header_str);
	free(path);
}

//TODO vyhazovani chyb podle errno
//operace vytvorit slozku
void MKD(Header *header, int socket_fd2)
{
	char *path = malloc(MAX_PATH*sizeof(char));
	sprintf(path,"%s%s", ROOT_FOLDER, header->path);
	int errCheck = mkdir(path, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
	if (errCheck == -1)
	{
		char *header_str = getHeader(1, 0);
		send_state(socket_fd2, header_str);
	}
	else
	{
		char *header_str = getHeader(0, 0);
		send_state(socket_fd2, header_str);
	}
	free(path);
}

//TODO vyhazovani chyb podle errno
//operace smazat slozku
void RMD(Header *header, int socket_fd2)
{
	char *path = malloc(MAX_PATH*sizeof(char));
	sprintf(path,"%s%s", ROOT_FOLDER, header->path);
	int errCheck = rmdir(path);
	if (errCheck == -1)  //doplnit errno
	{
		char *header_str = getHeader(1, 0);
		send_state(socket_fd2, header_str);
	}
	else
	{
		char *header_str = getHeader(0, 0);
		send_state(socket_fd2, header_str);
	}
	free(path);
}

//funkce rozhoduje o tom ktera funkce se vykona
void cmd_switch(Header *header, int socket_fd2)
{
	if (strcmp(header->command, "DEL") == 0)
	{
		if (strcmp(header->type, "file") == 0)
		{
			DEL(header, socket_fd2);
		}
		else
		{
			RMD(header, socket_fd2);
		}
	}
	else if (strcmp(header->command, "GET") == 0)
	{
		if (strcmp(header->type, "file") == 0)
		{
			GET(header, socket_fd2);
		}
		else
		{
			LST(header, socket_fd2); 
		}
		
	}
	else if (strcmp(header->command, "PUT") == 0)
	{
		if (strcmp(header->type, "file") == 0)
		{
			PUT(header, socket_fd2);
		}
		else
		{
			MKD(header, socket_fd2);
		}
	}
}

//funkce rozparseruje prichozi hlavicku a nahraje ji do struktury vyse
void HeaderParser(char *header_str, int socket_fd2)
{
	Header *header = malloc(sizeof(Header));

	header->command = strtok(header_str, " ");
	header->path = strtok(NULL, "?");
	strtok(NULL, "=");
	header->type = strtok(NULL, " ");
	strtok(NULL, ":");
	header->host = strtok(NULL, "\n");
	strtok(NULL, ":");
	header->date = strtok(NULL, "\n");
	strtok(NULL, ":");
	header->accept = strtok(NULL, "\n");
	strtok(NULL, ":");
	header->accept_en = strtok(NULL, "\n");
	strtok(NULL, ":");
	header->con_type = strtok(NULL, "\n");
	strtok(NULL, ":");
	header->con_len = strtok(NULL, "\n");
	strtok(NULL, ":");

	cmd_switch(header, socket_fd2);
}

//nacteni hlavicky zpravy od klienta
void ClientRequest(int socket_fd2)
{
	int buffer_limit = BUFFER;
	int position = 0;
	char letter[2];

	char *request = malloc(sizeof(char) * BUFFER);
	request[0] = '\0';

	while(true)
	{
		bzero(letter, 2);

		if (read(socket_fd2, letter, 1) < 0)
		{
			fprintf(stderr, "Chyba pri cteni ze socketu.\n");
			exit(5);
		}

		position = strlen(request);

		if ((position + 16) > buffer_limit)
		{
			buffer_limit += BUFFER;
			request = realloc(request, sizeof(char) *  buffer_limit);
		}

		request[position] = letter[0];
		request[position + 1] = '\0';

		if (position > 1)
		{
			if (request[position - 1] == '\n' && request[position] == '\n')
			{
				break;
			}
		}
	}

	HeaderParser(request, socket_fd2);
}

int main(int argc, char const *argv[])
{
	int socket_fd, socket_fd2;
	char buffer[BUFFER];
	bzero(buffer, BUFFER);
	struct sockaddr_in server_addr, client_addr; 
	
	arguments(argc, argv);

	socket_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (socket_fd < 0) 
	{
		fprintf(stderr, "Chyba pri otvirani socketu.\n");
		exit(2);
	}

	bzero((char *) &server_addr, sizeof(server_addr));

	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = INADDR_ANY;
	server_addr.sin_port = htons(PORT);

	//bind
	if ((bind(socket_fd, (struct sockaddr *) &server_addr, sizeof(server_addr))) < 0)
	{
		fprintf(stderr, "Chyba pri bindovani socketu.\n");
		exit(3);
	}

	//listen
	listen(socket_fd, 5);
	int cl_addr_size = sizeof(client_addr);

	//cyklus ktery ceka na zpravy od klientu
	while(true)
	{	//accept
		socket_fd2 = accept(socket_fd, (struct sockaddr *) &client_addr, &cl_addr_size);
		if (socket_fd2 < 0)
		{
			fprintf(stderr, "Chyba pri prijmani socketu.\n");
			exit(4);
		}

		ClientRequest(socket_fd2);
	}

	return 0;
}
