#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include <unistd.h>
#include <string.h>

void PrintHelp()
{
	printf("This line is every help you will ever get.\n");
	exit(0);
}

void ErrorFound(int i)
{
	switch(i)
	{
		case 0:
			fprintf (stderr, "Non valid argument -l.\n");
			break;
		case 1:
			fprintf (stderr, "Option -a requires an argument.\n");
			break;
		case 2:
			fprintf (stderr, "Option -s requires an argument.\n");
			break;
		case 3:
			fprintf (stderr, "Option -l requires an argument.\n");
			break;
		case 4:
			fprintf (stderr, "Option -f requires an argument.\n");
			break;
		case 5:
			fprintf (stderr, "Unknown option.\n");
			break;
		case 6:
			fprintf (stderr, "Unknown option character.\n");
			break;
		case 7:
			printf ("Non-option argument.\n");
			break;
		case 8:
			fprintf (stderr, "Non valid argument combination.\n");
			break;			
	}
	exit(1);
}

int CharToInt (char *str)
{
	int length = strlen(str);
	int number = 0;
	for (int i=0; i<length; i++)
	{
		if (!isdigit(str[i]))
		{
			return -1;
		}
		if (str[i] == '-')
		{
			return -1;
		}
		number = number * 10;
		number = number + (((int) str[i]) - 48);
	}

	return number;
}

int main (int argc, char *argv[])
{
	bool hflag = false;
	int c;
	int index;
	int lvalue = -2;
	char avalue[256] = "";
	char svalue[256] = "";
	char fvalue[256] = "";
	
	opterr = 0;

	while ((c = getopt (argc, argv, "ha:s:l:f:")) != -1)
	switch (c)
		{
			case 'h':
				hflag = true;
				break;
			case 'a':
				strcpy(avalue, optarg);
				break;
			case 's':
				strcpy(svalue, optarg);
				break;
			case 'l':
				lvalue = CharToInt(optarg);
				if (lvalue < 0)
					ErrorFound(0);
				break;
			case 'f':
				strcpy(fvalue, optarg);
				break;
			case '?':
				if (optopt == 'a')
					ErrorFound(1);
				else if (optopt == 's')
					ErrorFound(2);
				else if (optopt == 'l')
					ErrorFound(3);
				else if (optopt == 'f')
					ErrorFound(4);
				else if (isprint (optopt))
					ErrorFound(5);
				else
					ErrorFound(6);
			default:
				break;
			}

	for (index = optind; index < argc; index++)
    	ErrorFound(7);

	if (hflag == true && strcmp(avalue, "") == 0 && strcmp(svalue, "") == 0 && strcmp(fvalue, "") == 0 && lvalue == -2)
	{
		PrintHelp();
	}
	else
	{
		ErrorFound(8);
	}

	printf ("hflag = %d, avalue = %s, svalue = %s, lvalue = %d, fvalue = %s\n", hflag, avalue, svalue, lvalue, fvalue);

	return 0;
}