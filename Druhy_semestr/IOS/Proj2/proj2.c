/**
 *
 * IOS - Projekt 2, Roller Coaster
 * Autor: Petr Buchal (xbucha02)
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "proj2.h"


int main(int argc, char *argv[])
{

     if (argc != 5)  //kontrola poctu argumentu
    {
        fprintf(stderr, "INVALID_NUMBER_OF_ARGUMENTS\n");
        return 1;
    }

    for(int i = 1; i < 5; i++)  //kontrola ciselnych argementu
    {
        if (!isdigit(*argv[i]))
        {
            fprintf(stderr, "INVALID_ARGUMENTS\n");
            return 1;
        }
    }

    for(int i = 1; i < 5; i++)  //kontrola celociselnych argumentu
    {
        for (unsigned j = 0; j < strlen(argv[i]); j++)
        {
            if (!(argv[i][j] > 47 && argv[i][j] < 58))
            {
                fprintf(stderr, "INVALID_ARGUMENTS\n");
                return 1;
            }
        }
    }

    //prirazeni argumentu do promenych (pro lepsi prehlednost)
    int P = atoi(argv[1]);
    int C = atoi(argv[2]);
    int PT = atoi(argv[3]);
    int RT = atof(argv[4]);

    if (!(RT >= 0 && RT < 5001) || !(PT >= 0 && PT < 5001) || !(P > C) || !(C > 0) || !(P > 0))  //kontrola limitu cisel
    {
        fprintf(stderr, "INVALID_ARGUMENTS\n");
        return 1;
    }

    if ((P%C) != 0)  //kontrola zda-li je P nasobkem C
    {
        fprintf(stderr, "INVALID_ARGUMENTS\n");
        return 1;
    }

    //printf("Hello world! - |%d| |%d| |%d| |%d|\n", P, C, PT, RT);

    return 0;
}