#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include <stdlib.h>

#define MAX_PATH 1024 //

int PORT; //
char ROOT_FOLDER[MAX_PATH]; //

/**
 * [number_from_string description]
 * @param  str [description]
 * @return     [description]
 */
int number_from_string(const char *str)
{
	int i = 0, number;
	
	while(i < strlen(str))
	{
		if (!isdigit(str[i]))
		{
			exit(0);
		}
		i++;
	}

	number = atoi(str);
	return number;
}
/**
 * [arguments description]
 * TODO osetrit vetsi nez 1024 delka path; osetrit stav kdyz je jeden parametr zadan dvakrat
 * @param argc [description]
 * @param argv [description]
 */
void arguments(int argc, char const *argv[])
{
	PORT = 6677;
	getcwd(ROOT_FOLDER, sizeof(ROOT_FOLDER));

	if (argc == 3)
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
			exit(1);
		}
	}
	else
	{
		exit(1);
	}
}

int main(int argc, char const *argv[])
{
	arguments(argc, argv);

	return 0;
}
