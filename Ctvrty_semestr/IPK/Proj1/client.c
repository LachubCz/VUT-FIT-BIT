#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdbool.h>

#define MAX_PATH 1024 //

char COMMAND[4]; //
char REMOTE_PATH[MAX_PATH]; //
char LOCAL_PATH[MAX_PATH]; //

/**
 * [err_print description]
 * @param err_code [description]
 */
void err_print(int err_code)
{
	switch(err_code)
	{
		case 2:
			fprintf(stderr, "Not a directory.");
			exit(2);
		case 3:
			fprintf(stderr, "Directory not found.");
			exit(3);
		case 4:
			fprintf(stderr, "Directory not empty.");
			exit(4);
		case 5:
			fprintf(stderr, "Already exists.");
			exit(5);
		case 6:
			fprintf(stderr, "Not a file.");
			exit(6);
		case 7:
			fprintf(stderr, "File not found.");
			exit(7);
		case 8:
			fprintf(stderr, "Unknown error.");
			exit(8);
	}
}

/**
 * [check_command description]
 * @param  cmd [description]
 * @return     [description]
 */
int check_command(const char *cmd)
{
	if (strcmp(cmd, "del") == 0)
	{
		strcpy(COMMAND, cmd);
		return 0;
	}
	else if (strcmp(cmd, "get") == 0)
	{
		strcpy(COMMAND, cmd);
		return 1;
	}
	else if (strcmp(cmd, "put") == 0)
	{
		strcpy(COMMAND, cmd);
		return 2;
	}	
	else if (strcmp(cmd, "lst") == 0)
	{
		strcpy(COMMAND, cmd);
		return 3;
	}	
	else if (strcmp(cmd, "mkd") == 0)
	{
		strcpy(COMMAND, cmd);
		return 4;
	}	
	else if (strcmp(cmd, "rmd") == 0)
	{
		strcpy(COMMAND, cmd);
		return 5;
	}
	else
	{
		exit(1);
	}
}

/**
 * [arguments  description]
 * @param argc [description]
 * @param argv [description]
 */
void arguments (int argc, char const *argv[])
{
	bool fourth_argument;

	if (argc > 4 || argc < 3)
	{
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
			printf("yshir\n");
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
		exit(1);
	}
}

int main(int argc, char const *argv[])
{
	arguments(argc, argv);

	return 0;
}