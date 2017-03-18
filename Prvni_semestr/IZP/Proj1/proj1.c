#include <stdio.h>
#include <time.h>

char daybuf[30];
struct tm time_str;

int datum (int year, int month, int day)  //funkce pro zjisteni dnu v tydnu
{
    time_str.tm_year = year - 1900;
    time_str.tm_mon = month - 1;
    time_str.tm_mday = day;
    time_str.tm_hour = 0;
    time_str.tm_min = 0;
    time_str.tm_sec = 0;
    time_str.tm_isdst = -1;

    if (mktime(&time_str) == -1)
        strftime(daybuf, sizeof(daybuf), "-unknown- %Y-%m-%d", &time_str);
    else {
        strftime(daybuf, sizeof(daybuf), "%a %Y-%m-%d", &time_str);
         }
    return 0;
}

int main(int argc, char *argv[])
{

char text[102], detek_cislo[102];
int i, mezera[102], y, x, q, k, w, c, true_pal, v, h, date_test, pocet_znaku, index, year, month, day, velikost;


if (argc > 1)  //pokud je zadan libovolny argument program vypise svuj kratky popis
{
    printf ("Na vstupu jsou do programu nacitany znaky. \n");
    printf ("Program detekuje mezery mezy znaky a useky mezi mezerami dale zkouma.\n");
    printf ("Nejdrive probehne test na cislo, pokud je hodnota cisla vetsi nez INT_MAX,\n");
    printf ("program urci, ze jde o slovo, v opacnem pripade program provadi\n");
    printf ("jeste test na prvocislo. Dale probehne test na palindrom a datum.\n");
    printf ("U data urci program o ktery den v tydnu se jedna.\n");
    printf ("Pokud se nejedna ani o jednu z vyse zminovanych variant\n");
    printf ("program vyhodnoti usek znaku jako slovo.\n");
    printf ("Program zpracovava zadavane znaky dokud neni zadan EOF.\n");
    printf ("Pokud je zadano slovo delsi nez 100 znaku, program se ukonci.\n");
    argv[1] = "done";  //radek je zde jen pro pouziti argv, aby nevznikalo varovani o nepoutite hodnote
    return 0;
}

i = 1;  //prvni nacteny znak bude ulozen s indexem 1

for (i; i < 102; i++)  //cyklus nenacte slovo delsi nez 100 znaku, 101. znak je enter
{
    text[i] = getchar();
    if (text[i] == EOF)  //je-li zadan EOF, program se ukonci
        break;

    if (text[i] == '\n')  //po stisknuti enteru se zpracuji nactene znaky
    {
        index = 1;  //pouzivano jako index
        q = 1;  //pouzivano jako index
        pocet_znaku = i;  //od teto chvile je i pouzivano jako index pro dalsi casti programu
        x = 1;  //prvni nacteny znak
        k = 1;  //prvni nacteny znak
        h = 0;  //promena do ktere se uklada cislo, pokud ho program detekuje
        date_test = 0;  //pravdivostni hodnota pro urceni data
        text[i] = ' ';  //program premeni enter na mezeru -> mezera je posledni nacteny znak

        for (i = 1; i < 101; i++)  //vynulovani pole do kterého se ukládají pozice mezer z nactenych znaku
            mezera[i] = 0;

        for (i = 1; i < (pocet_znaku + 1); i++)  //cyklus zjistuje pozice mezer a uklada je do pole
        {
            if (text [i] == ' ')
            {
                mezera[index] = i;
                index++;
            }
        }

        while (mezera[q] != 0)  //cyklus pracuje dokud neni na vstupu nula, tedy dokud nebyl nacten posledni znak
        {
            for (i = 0; i < 101; i++)  //detek_cislo je pole, ktere se vyuziva pro detekci cisla
                detek_cislo[i] = 'c';

            if (x < mezera[q])
            k = x;  //pozice prvniho znaku pro zkoumani prvocisla

            velikost = 0;
            true_pal = 1;  //pravdivostni hodnota pro urceni palindromu
            y = x;  //pozice prvniho znaku pro zkoumani cisla
            c = mezera[q] - 1;  //pozice posledniho znaku pro zkoumani palindromu
            v = k;  //pozice prvniho znaku pro zkoumani palindromu
            index = v;  //pozice prvniho znaku pro zkoumani datumu
            i = 0;  //vynulovani indexu pro nasledujici cyklus

            for (y; y < mezera[q]; y++)  //cyklus detekujici cislice
            {
                i++;
                if (text[y] > 47 && text[y] < 58)  //do pole m se ulozi 'a', pokud ma znak ASCII hodnotu mezi 48 az 57
                    detek_cislo[i] = 'a';
                else
                {
                    if (text[y] == ' ')  //mezera oddeluje slova, tudiz se nepocita jako znak slova a do pole se rovnez ulozi 'a'
                        detek_cislo[i] = 'a';
                    else  //pokud nema znak ASCII hodnotu mezi 48 az 57, do pole m se ulozi 'n'
                        detek_cislo[i] = 'n';
                }
                x++;
            }

            for (i = 0; i < 101; i++)  //cyklus detekujici cislo
            {
                if (detek_cislo[i] == 'a' || detek_cislo[i] == 'c')  //pokud byli vsechny zkoumane znaky cislice, potom se v poli budou nachazet jen 'c' a 'a'
                    w = 'a';  //promena w s hodnotou 'a' programu rika, ze jde o cislo
                else  //pokud byl nejaky znak rozdilny od cislice, potom se v poli m bude nachazet 'n' a program urci, ze se nejedna o cislo
                    {
                    w = 'n';  //promena w s hodnotou 'n' programu rika, ze nejde o cislo
                    break;
                    }
            }

            if (w == 'a')  //program vi, ze jde o cislo, ted ho zajima zda jde o prvocislo
            {
                for (k; k < mezera[q]; k++)  //cyklus prevadejici jednotlive znaky na  pomoci jejich ASCII hodnot
                {
                    if (text[k] != ' ')  //pokud je hodnota cisla vetsi nez INT_MAX program urci, ze jde o slovo
                    {
                        if (h <= 214748364)
                        {
                            h = h * 10;
                            if (h == 2147483640)
                            {
                                if (text[k] == 56 || text[k] == 57)
                                {
                                    printf("word: ");
                                    for (v; v < mezera[q]; v++)
                                        if (text[v] != ' ')
                                            printf("%c", text[v]);
                                    printf ("\n");
                                    velikost = 1;
                                    break;
                                }
                                else
                                    h = h + (text[k] - 48);
                            }
                            else
                                h = h + (text[k] - 48);
                        }
                        else
                        {
                            w = 'n';  //hodnota je vetsi nez INT_MAX, program dale podnikne test na palindrom
                            velikost = 1;
                            break;
                        }
                    }
                }

                if (velikost == 0)
                {
                    for (i = 2; i <= h - 1; i++)  //cyklus zkoumajici prvocislo
                    {
                        if (h%i == 0)  //pokud je cislo deleno mensi hodnotou (nez samotne cislo) se zbytkem nula, pote se nejedna o prvocislo
                        {
                            printf("number: %d\n", h);
                            break;
                        }
                    }

                    if (h == i)  //pokud je cislo delitelne pouze samo sebou (a jednickou), pote jde o prvocislo
                    {
                        printf("number: %d (prime)\n", h);
                    }

                    if (h == 1)  //'1' je pripad se kterym si cyklus neporadi
                    {
                        printf ("number: %d \n", h);
                    }
                    if (mezera[q]-mezera[q-1] != 1)  //pokud by zde tato podminka nebyla, pri zadani vice mezer za sebou by program detekoval nulu
                    {
                        if (mezera[q] != 1)  //pokud by zde tato podminka nebyla, po stisknuti enteru na prazdnem vstupu by program detekoval nulu
                        {
                            if (h == 0)  //'0' je pripad se kterym si cyklus neporadi
                            {
                                printf ("number: %d\n", h);
                            }
                        }
                    }
                }
            }
            h = 0;  //vynulovani promene pro pripadne dalsi cykly

            if (w == 'n')  //program vi, ze nejde o cislo, ted ho zajima zda jde o datum, palindrom nebo slovo
            {
                k = v;  //jelikoz program muze zkoumat i cislo, ktere presahlo hodnotu INT_MAX, je treba pro jistotu nacist hodnotu k znovu z v
                if (k == 1)  //zkoumani zda jde o uplne prvni znak
                    {
                    for (k; k < mezera[q]; k++)  //pokud se pismena nerovnaji nejde o palindrom
                        {
                            if (text[k]!=text[c])
                            {
                                true_pal = 0;
                            }
                            c = c - 1;
                        }
                    }
                else
                    for (k = (k + 1); k < mezera[q]; k++)  //pokud se pismena nerovnaji nejde o palindrom
                    {
                        if (text[k]!=text[c])
                        {
                            true_pal = 0;
                        }
                        c = c - 1;
                    }

                if (true_pal == 1)  //pokud je hodnota promenne true_pal 1, pote jde o palindrom
                {
                    printf("word: ");
                    for (v; v < mezera[q]; v++)
                    if (text[v] != ' ')
                        printf("%c", text[v]);
                    printf (" (palindrom)\n");
                }
                else  //nejedna se o cislo, prvocislo ani palindrom, nyni probehne test na datum
                {
                    if (index == 1)  //zkoumani zda jde o uplne prvni znak
                    {
                        if ((mezera[q]-index) == 10)  //test poctu znaku v pripadnem datu
                        {
                            for (index; index < mezera[q]; index++)  //cyklus pomoci ASCII hodnot urci zda jde o strukturu podobnou datu
                                if (text[index] < 48 || text[index] > 57)
                                {
                                    if (index == 5 || index == 8)
                                    {
                                        if (text[index] == 45)
                                            date_test = 1;
                                        else
                                        {
                                            date_test = 0;
                                            break;
                                        }
                                    }
                                    else
                                    {
                                        date_test = 0;
                                        break;
                                    }

                                }
                                else
                                    date_test = 1;
                        }
                    }
                    else
                        if ((mezera[q]-index) == 11)  //test poctu znaku v pripadnem datu
                        {
                            for (index; index < mezera[q]; index++)  //cyklus pomoci ASCII hodnot urci zda jde o strukturu podobnou datu
                            {
                                if (text[index] < 48 || text[index] > 57)
                                {
                                    if (index == v)
                                    {
                                        if (text[index] == ' ')
                                            date_test = 1;
                                    }
                                    else
                                    {
                                        if (index == (v + 5) || index == (v + 8))
                                        {
                                            if (text[index] == 45)
                                                date_test = 1;
                                            else
                                            {
                                                date_test = 0;
                                                break;
                                            }
                                        }
                                        else
                                        {
                                            date_test = 0;
                                            break;
                                        }
                                    }
                                }
                                else
                                    date_test = 1;
                        }}
                        else date_test = 0;

                    if (date_test == 1)  //program vi, ze jde o strukturu podobnou datumu, nyni prevede znaky na cisla
                    {
                        year = 0;
                        month = 0;
                        day = 0;

                        index = v;
                        for (index; index < mezera[q]; index++)  //prevedeni znaku na rok
                        {
                            if (text[index] == 45)
                                break;
                            if (text[index] != ' ')
                                year = ((year * 10) + (text[index] - 48) * 10);
                        }
                        year = year/10;

                        for (index = (index + 1); index < mezera[q]; index++)  //prevedeni znaku na mesic
                        {
                            if (text[index] == 45)
                                break;
                            if (text[index] != ' ')
                                month = ((month * 10) + (text[index] - 48) * 10);
                        }
                        month = month/10;

                        for (index = (index + 1); index < mezera[q]; index++)  //prevedeni znaky na den
                        {
                            if (text[index] == 45)
                                break;
                            if (text[index] != ' ')
                                day = ((day * 10) + (text[index] - 48) * 10);
                        }
                        day = day/10;

                        if (year > 1899 && day > 0 && day < 32 && month > 0 && month < 13)  //finalni test na datum
                        {
                            datum(year, month, day);  //volani funkce datum, ktera urci den, o ktery se jedna
                            printf("date: %s\n", daybuf);
                            true_pal = 1;
                        }
                        else  //pokud nejde o datum jde o slovo
                        {
                            printf("word: ");
                            for (v; v < mezera[q]; v++)
                                if (text[v] != ' ')
                                    printf("%c", text[v]);
                            printf ("\n");
                        }
                    }

                    else  //pokud nejde o datum jde o slovo
                    {
                        printf("word: ");
                        for (v; v < mezera[q]; v++)
                            if (text[v] != ' ')
                                printf("%c", text[v]);
                        printf ("\n");
                    }
                }
            }

                q++;
        }

        i = 0;
    }
}
return 0;
}

