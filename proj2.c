#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>

double absolut (double x)  //funkce pro vypocet absolutni hodnoty
{
    if (x >= 0)
        return x;
    else
        return -x;
}

double taylor_log(double x, unsigned int n)  //funkce pro vypocet prirozeneho logaritmu pomoci taylorova polynomu
{
    double t = 1.0;
    double s = 0;
    double e = 1.0;
    unsigned int i = 0;

    if (x < 0)  //osetreni zaporneho cisla
    {
        return NAN;
    }

    if (x == 0)  //osetreni nuly
    {
        return -INFINITY;
    }

    if (x == INFINITY)  //osetreni nekonecna
    {
        return INFINITY;
    }

    if (x == NAN)  //osetreni NAN
    {
        return NAN;
    }

    if (x < 0.8)  //zvolena mez pro pocitani jednotlivych polynomu
        {
            x = 1 - x;  //zvoleni vhodného x pro vypocet logaritmu
            while (i < n)  //
            {
                i = i + 1;
                e = (e*x);
                t = (e/i);
                s = s - t;
            }
            return s;
        }
    else
    {
        x = ((x-1)/x);  //zvoleni vhodného x pro vypocet logaritmu
        while (i < n)
        {
            i = i + 1;
            e = (e*x);
            t = (e/i);
            s = s + t;
        }
         return s;
    }
}

double cfrac_log(double x, unsigned int n)  //funkce pro vypocet prirozeneho logaritmu pomoci zretezeneho zlomku
{
    double cf = 1.0;
    double a, b;

    if (x < 0)  //osetreni zaporneho cisla
    {
        return NAN;
    }

    if (x == 0)  //osetreni nuly
    {
        return -INFINITY;
    }

    if (x == INFINITY)  //osetreni nekonecna
    {
        return INFINITY;
    }

    if (x == NAN)  //osetreni NAN
    {
        return NAN;
    }

    x = (x - 1)/(1 + x);
    for (; n >= 1; n--)  //cyklus zretezeneho zlomku
    {
        a = x * x * n *n;
        b = 2*n -1;
        cf = b - (a / cf);
    }
    x = (2*x) / cf;
    return x;
}

int epscalcul (double x, double n)  //funkce pro vypocet minimalniho poctu iteraci (pro tayloruv polynom)
{
    double t = 1.0;
    double s = 0;
    double e = 1.0;
    int i = 0;

    if (x < 0)  //osetreni zaporneho cisla
    {
        return 0;
    }

    if (x == 0)  //osetreni nuly
    {
        return 0;
    }

    if (x == INFINITY)  //osetreni nekonecna
    {
        return 0;
    }

    if (x == NAN)  //osetreni NAN
    {
        return 0;
    }

    if (x < 0.8)  //zvolena mez pro pocitani jednotlivych polynomu
        {
            x = 1 - x;  //zvoleni vhodného x pro vypocet logaritmu
            while (t >= n)
            {
                i = i + 1;
                e = (e*x);
                t = (e/i);
                s = s - t;
            }
        return i;
        }
    else
    {
        x = ((x-1)/x);  //zvoleni vhodného x pro vypocet logaritmu
        while (t >= n)
        {
            i = i + 1;
            e = (e*x);
            t = (e/i);
            s = s + t;
        }
    return i;
    }
}

int epscalcul2 (double x, double n)  //funkce pro vypocet minimalniho poctu iteraci (pro zretezeny zlomek)
{
    double cf = 1.0;
    double asdf;  //rozdil matematickeho logaritmu a logaritmu vypocitaneho zretezenym zlomkem
    double a, b, c;
    int t, f = 1;  //promenne udavajici pocet iteraci
    double true_log = log(x);  //logaritmus vypocitany pomoci matematicke funkce

    if (x < 0)  //osetreni zaporneho cisla
    {
        return 0;
    }

    if (x == 0)  //osetreni nuly
    {
        return 0;
    }

    if (x == INFINITY)  //osetreni nekonecna
    {
        return 0;
    }

    if (x == NAN)  //osetreni NAN
    {
        return 0;
    }

    x = (x - 1)/(1 + x);
    for ( ; ; f++)  //funkce hleda vhodny pocet iteraci od 1
    {
        t = f;  //aktualni pocet iteraci
        for (; t >= 1; t--)
        {
            a = x * x * t *t;
            b = 2*t -1;
            cf = b - (a / cf);
        }
        c = (2*x) / cf;

        asdf = true_log - c;  //rozdil mezi logaritmem a logaritmem vypocitam ze zretezeneho zlomku
        asdf = absolut(asdf);  //vysledek muze nabyvat zaporne hodnoty, proto je pouzuta funkce pro absolutni hodnotu

        if (asdf <= n)  //pokud je rozdil matematickeho logarimu a logaritmu vypocitaneho zretezenym zlomkem <= zadanehu rozdilu je vracen pocet iteraci
        {
            return f;
        }
    }
}

int main(int argc, char *argv [])
{
    double m;  //promena do niz jsou ukladany vysledky funkci pred vytisknutim
    char *control;  //ukazatel slouzici pro kontrolu spravne zadanych ciselnych argumentu

    switch (argc)  //switch pracujici s poctem argumentu
    {
        case 4 :  //zadany jsou 3 argumenty -> jde o variantu programu --log
        {
            if(strcmp(argv[1], "--log") == 0)  //test zdali je prvni argument "--log", pokud ano, program zacne ukladat promenne z argumentu
            {
                double x = strtod(argv[2], &control);  //ulozeni argumentu do promenne
                    if (*control != '\0')  //test uspesneho ukladani hodnoty argumentu do promenne
                    {
                        if(strcmp(control, "INFINITY") == 0)
                        {
                            x = INFINITY;
                        }
                        else
                        {
                            printf ("INVALID_ARGUMENT\n");
                            return 0;
                        }
                    }
                unsigned int n = strtoul(argv[3], &control, 0);  //ulozeni argumentu do promenne
                    if (*control != '\0')  //test uspesneho ukladani hodnoty argumentu do promenne
                    {
                        printf ("INVALID_ARGUMENT\n");
                        return 0;
                    }

                    printf ("       log(%.12g) = %.12g\n", x, log(x));
                    m = cfrac_log (x, n);
                    printf ("    cf_log(%.12g) = %.12g\n", x, m);
                    m = taylor_log (x, n);
                    printf ("taylor_log(%.12g) = %.12g\n", x, m);
            }
            else
            {
                printf ("INVALID_ARGUMENT\n");
            }
            return 0;
        }
        case 5 :  //zadany jsou 4 argumenty -> jde o variantu programu --iter
        {
            if(strcmp(argv[1], "--iter") == 0)  //test zdali je prvni argument "--iter", pokud ano, program zacne ukladat promenne z argumentu
            {
                double min = strtod(argv[2], &control);  //ulozeni argumentu do promenne
                    if (*control != '\0')  //test uspesneho ukladani hodnoty argumentu do promenne
                    {
                        if(strcmp(control, "INFINITY") == 0)
                        {
                            min = INFINITY;
                        }
                        else
                        {
                            printf ("INVALID_ARGUMENT\n");
                            return 0;
                        }
                    }
                double max = strtod(argv[3], &control);  //ulozeni argumentu do promenne
                    if (*control != '\0')  //test uspesneho ukladani hodnoty argumentu do promenne
                    {
                        if(strcmp(control, "INFINITY") == 0)
                        {
                            max = INFINITY;
                        }
                        else
                        {
                            printf ("INVALID_ARGUMENT\n");
                            return 0;
                        }
                    }
                double eps = strtod(argv[4], &control);  //ulozeni argumentu do promenne
                    if (*control != '\0')  //test uspesneho ukladani hodnoty argumentu do promenne
                    {
                        printf ("INVALID_ARGUMENT\n");
                        return 0;
                    }
                    printf ("       log(%.12g) = %.12g\n", min, log(min));
                    printf ("       log(%.12g) = %.12g\n", max, log(max));


                    int iter1 = epscalcul2 (min, eps);  //pocet iteraci pro dolni hranici
                    int iter2 = epscalcul2 (max, eps);  //pocet iteraci pro horni hranici
                    if (iter1 > iter2)  //dale se pocita s vetsim poctem iteraci
                    {
                        printf ("continued fraction iterations = %d\n", iter1);
                    m = cfrac_log (min, iter1);
                    printf ("    cf_log(%.12g) = %.12g\n", min, m);
                    m = cfrac_log (max, iter1);
                    printf ("    cf_log(%.12g) = %.12g\n", max, m);
                    }
                    else
                    {
                        printf ("continued fraction iterations = %d\n", iter2);
                    m = cfrac_log (min, iter2);
                    printf ("    cf_log(%.12g) = %.12g\n", min, m);
                    m = cfrac_log (max, iter2);
                    printf ("    cf_log(%.12g) = %.12g\n", max, m);
                    }


                    int taylor_iter1 = epscalcul(min, eps);  //pocet iteraci pro dolni hranici
                    int taylor_iter2 = epscalcul(max, eps);  //pocet iteraci pro horni hranici
                    if (taylor_iter1 > taylor_iter2)  //dale se pocita s vetsim poctem iteraci
                    {
                        printf ("taylor polynomial iterations = %d\n", taylor_iter1);
                        m = taylor_log (min, taylor_iter1);
                        printf ("taylor_log(%.12g) = %.12g\n", min, m);
                        m = taylor_log (max, taylor_iter1);
                        printf ("taylor_log(%.12g) = %.12g\n", max, m);
                    }
                    else
                    {
                        printf ("taylor polynomial iterations = %d\n", taylor_iter2);
                        m = taylor_log (min, taylor_iter2);
                        printf ("taylor_log(%.12g) = %.12g\n", min, m);
                        m = taylor_log (max, taylor_iter2);
                        printf ("taylor_log(%.12g) = %.12g\n", max, m);
                    }
            }
            else
            {
                printf ("INVALID_ARGUMENT\n");
            }
            return 0;
        }
        default :  //jakykoliv jiny pocet argumentu je chybny
        {
            printf ("WRONG_NUMBER_OF_ARGUMENTS\n");
            return 0;
        }
    }
    return 0;
}
