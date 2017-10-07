#include <stdio.h>
#include <stdlib.h>

int main (int argc, char *argv[])
{
	bool hflag = false;
	char *avalue = NULL;
	char *svalue = NULL;
	char *lvalue = NULL;
	char *fvalue = NULL;
	int index;
	int c;

	opterr = 0;

	while ((c = getopt (argc, argv, "abc:")) != -1)
	switch (c)
		{
			case 'h':
				hflag = true;
				break;
			case 'a':
				aflag = optarg;
				break;
			case 's':
				sflag = optarg;
				break;
			case 'l':
				lflag = optarg; // kontrola formatu cisla
				break;
			case 'f':
				fvalue = optarg;
				break;
			case '?':
				if (optopt == 'c')
					fprintf (stderr, "Option -%c requires an argument.\n", optopt);
				else if (isprint (optopt))
					fprintf (stderr, "Unknown option `-%c'.\n", optopt);
				else
					fprintf (stderr,
									 "Unknown option character `\\x%x'.\n",
									 optopt);
				return 1;
			default:
				abort ();
			}

	printf ("aflag = %d, bflag = %d, cvalue = %s\n", aflag, bflag, cvalue);

	for (index = optind; index < argc; index++)
		printf ("Non-option argument %s\n", argv[index]);
	return 0;
}