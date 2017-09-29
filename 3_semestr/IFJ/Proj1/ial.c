/**
 * @author Petr Buchal
 * @date 20_11_2016
 * @file ial.c
 * @brief vestavene funkce + tabulka symbolu
 * Copyright (c) 2016
 */

#include <stdio.h>
#include <limits.h>
#include <stdlib.h>
#include <float.h>
#include <string.h>
#include <errno.h>

#include "string.h"
//popis funkci je v zadani
char *readString()  //otestovano
{
    int ch;  //cteny znak
    unsigned len = 0;  //delka stringu
    unsigned size = 10;  //alokovana delka behem fce
    char *str = malloc (sizeof (char) * size);  //bude potreba si pamatovat, aby nedoslo k memory leaku
    
    if (str == NULL) return NULL;
    while(EOF!=(ch=fgetc(stdin)) && ch != '\n')
    {
    	str[len++]=ch;
    	if (len==size)
    	{
			str = realloc(str, sizeof(char)*(size+=16));
            if(!str)return str;
    	}
    }
    return realloc(str, sizeof(char)*len);
}

int readInt()  //otestovano, nutno doplnit error
{	
	char *input_string = readString();
	char *ptr = NULL;

	long input = strtol(input_string, &ptr, 10);
	
	if (input > INT_MAX || input < INT_MIN) 
    {
    	/* When there is an error, we set errno variable,
		   the caller has to test for errno. */
		errno = 1;
    	free(input_string);
    }
    else
    {
    	if (*ptr != '\0')
		{
			/* When there is an error, we set errno variable,
		   		the caller has to test for errno. */
			errno = 1;
			free(input_string);
		}
    	else
    	{
    		free(input_string);
    		return input;
    	}
    }
}

double readDouble()  //otestovano, z nejakeho duvodu nefunguje spravne pro zaporna cisla, nutno doplnit error
{
	char *input_string = readString();
	char *ptr = NULL;

	long double input = strtold(input_string, &ptr);

	if (input > DBL_MAX || input < DBL_MIN) 
    {
		/* When there is an error, we set errno variable,
		   		the caller has to test for errno. */
			errno = 1;
    }
    else
    {
    	if (*ptr != '\0')
		{
		 	/* When there is an error, we set errno variable,
		   		the caller has to test for errno. */
			errno = 1;
		}
    	else
    	{
    		return (double)input;
    	}
    }
}

/*void print_var(String *s, symb_table_t *T)
{
    data_t *data_var = malloc(sizeof(struct data_s));
    data_var->name = s;

    table_item_t *item = symbtab_search(T, data_var);

    if (item != NULL)
    {
        switch(item->data.type)
        {
        case 0:
            fprintf(2,"%d", item->data.i);
            break;
        case 1:
            fprintf(2,"%f", item->data.d);
            break;
        case 2:
            fprintf(2,"%s", item->data.str->string);
            break;
        default:
            ;  //error
        }
    }
    free(data_var);
}*/

void print()  //nutno domluvit se na parametrech
{
	String *temp_string = string_new();
	int state = 0;
	int i = 0;

	while(string->string[++i]) {
		case 0:
			if (c == '"') {
				state = 1;
				break;
			}
		case 1:
			if (isalnum(c)) {
				string_add_char(temp_string,c);
				break;
			}
			if (c == '\\') {
				string_add_char(temp_string, c);
				state = 2;
				break;
			}
			if (c == '"') {
				fprintf(2,"%s",temp_string->string);
				break;
			}
			if (c == '+') {
				state = 0;
				break;
			}
		case 2:
			if (c == '\\') {
				/* No need to add another backslash */
				break;
			}
			
	}
}




//vestavene funkce pro praci s retezci
int lenght(char *s)  //otestovano
{
	int lenght = strlen(s);
	return lenght;
}

char *substr(char *s, int i, int n)  //otestovano, nutno doplnit error
{
	int len = lenght(s);
	int y;
	char *str = malloc (sizeof (char) * (n+1));  //bude potreba si pamatovat, aby nedoslo k memory leaku

	if (str == NULL)
	{
		/* When there is an error, we set errno variable,
		   		the caller has to test for errno. */
			errno = 1;
	}

	if (len < (i+n))
	{
		/* When there is an error, we set errno variable,
		   		the caller has to test for errno. */
			errno = 10;
	}
	else
	{
		for (y = 0; y < n; i++, y++)
		{
			str[y]=s[i];
		}
		str[n] = '\0';
		return str;
	}
}

int compare(char *s1, char *s2)  //otestovano
{
	int ret = strcmp(s1, s2);
	if (ret == 0)
	{
		return 0;
	}
	else
	{
		if (ret > 0)
		{
			return 1;
		}
		else
		{
			return -1;
		}
	}
}

void ComputeJumps(char *p, int CharJump[256])  //pomocna fce pro find, otestovano
{
	unsigned char ch;
	int k;
	int len = lenght(p);
	for (ch = 0; ch < 255; ch++)
	{
		CharJump[ch] = len;
	}
	for (k = 0; k < len; k++)
	{
		CharJump[(int)p[k]] = len - k - 1;
	}
}

int find(char *s, char *search)  //vyuziti boyer-moor-algorytm, otestovano
{
	unsigned int CharJump[256];
	int j = (lenght(search)) - 1;
	int k = (lenght(search)) - 1;

	if (k == -1)
	{
		return 0;
	}

	ComputeJumps(search, CharJump);

	while(j <= (lenght(s)))
	{
		if (s[j] == search[k])
		{
		    if(k == 0)
		    {
                return j;
		    }
			j = j - 1;
			k = k - 1;
		}
		else
		{
			j = j + CharJump[(int)s[j]];
			k = (lenght(search)) - 1;
		}
	}
    return -1;
}

void swap (char *one, char *two)  //pomocna fce pro partition, otestovano
{
    char *sym;
    sym = *one;
    *one = *two;
    *two = sym;
}

void partition(char *s, int left, int right, int *i, int *j)  //pomocna fce pro quicksort, otestovano
{
	int PM;
	char sym;
	*i = left;
	*j = right;
	PM = s[(((*i) + (*j)) / 2)];
	do
	{
		while ((int)s[*i] < PM)
		{
			(*i) = (*i) + 1;
		}

		while ((int)s[*j] > PM)
		{
			(*j) = (*j) - 1;
		}
		if ((*i) <= (*j))
		{
		    swap(&s[(*i)], &s[(*j)]);
			(*i) = (*i) + 1;
			(*j) = (*j) - 1;
		}
	}
	while(!((*i) > (*j)));
}

char *quicksort (char *s, int left, int right)  //quicksort - nase varianta pro fci sort, otestovano
{
	int i;
	int j;
	partition(s, left, right, &i, &j);
	if (left < (j))
	{
		quicksort(s, left, (j));
	}
	if ((i) < right)
	{
		quicksort(s, (i), right);
	}
	return s;
}

char *sort (char *s)  //fce pro trideni vola quicksort, otestovano 
{
	int right = lenght(s);
	int left = 0;
	char *sorted = quicksort(s, left, (right-1));
	return sorted;
}

/**
 * @brief      funkce inicializuje tabulku symbolu
 *
 * @return     vraci ukazatel na strukturu symb_table_t
 */
symb_table_t *symbtab_new()
{
	symb_table_t *table = NULL;
    if((table = malloc(sizeof(struct table_s))) == NULL)
    {
        ;  //error
    }
    table->first = NULL;
	table->table_id = string_new();
    return table;
}

/**
 * @brief      funkce vklada novu polozku do tabulky symbolu, pokud polozka existuje aktualizuje jeji hodnotu (nerekurzivni)
 *
 * @param      T     odkaz na tabulku symbolu
 * @param      var   odkaz na strukturu s daty o promenne/funkci
 *
 * @return     vraci bool, podle toho zda-li bylo vlozeni uspesne
 */
bool symbtab_insert(symb_table_t *T, data_t *var)
{
	if (T == NULL)  //pokud neni nainicializovana tabulka, fce vraci false
	{
		return false;
	}

	unsigned long K = hashfunction (var);
	table_item_t *fptr = NULL;  // ukazatel na otcovský uzel F
    table_item_t *ptr = NULL;   // pomocny ukazatel

	if (T->first == NULL)  //pokud je prvni polozka prazdna priradi se prvek do prvni
	{

		if ((T->first = malloc(sizeof(struct table_item_s))) == NULL)
		{
			return false;  //error (mallock)
		}
		else  //prirazeni pokud je first prazdny
		{
			T->first->LPtr = NULL;
			T->first->RPtr = NULL;
			T->first->key = K;
			T->first->data = *var;
			return true;
		}
	}
    else
    {
	    fptr = T->first;  //ulozim si otcovsky uzel

	    while(1)  //cyklus pro nerekurzivni sestup tabulkou symbolu
	    {
	        if(fptr->key > K)
	        {
	            if(fptr->LPtr == NULL)
	            {
	            	if((ptr = malloc(sizeof(struct table_item_s))) != NULL)  //alokace pro novou polozku
	            	{
	            		ptr->data = *var;
			            ptr->LPtr = NULL;
			            ptr->RPtr = NULL;
			            ptr->key = K;
			            fptr->LPtr = ptr;
			            return true;
	         	    }
			        else
			        {
						return false;  //error (mallock)
			        }
		        }
		        else
		        {
		            fptr = fptr->LPtr;  //skok na zacatek cyklu s novym otcem
	        	}
	        }
	        else
	        {
	        	if(fptr->key < K)
	        	{
	        		if(fptr->RPtr == NULL)
	        		{
	          			if((ptr = malloc(sizeof(struct table_item_s))) != NULL)  //alokace pro novou polozku
	          			{
	          				ptr->data = *var;
				            ptr->LPtr = NULL;
				            ptr->RPtr = NULL;
				            ptr->key = K;
				            fptr->RPtr = ptr;
				            return true;
	          			}
	          			else
	          			{
	          				return false;  //error (mallock)
	          			}
	        		}
		        	else
		        	{
		         		fptr = fptr->RPtr;  //skok na zacatek cyklu s novym otcem
		        	}
	            }
	      		else
	      		{
	            	fptr->data = *var;  //klic je stejny - aktualizace hodnot
	        		return true;
	      		}
	        }
	    }
	}
}

/**
 * @brief      funkce pro hledani v tabulce symbolu
 *
 * @param      T     odkaz na tabulku symbolu
 * @param      var   odkaz na strukturu s daty o promenne/funkci
 *
 * @return     vraci ukazatek na polozku z tabulky symbolu - struktura table_item_t; pokud neexistuje vraci NULL
 */
table_item_t *symbtab_search(symb_table_t *T, String *s)
{
    data_t *var = data_create(s, 0, NULL);

	unsigned long K = hashfunction (var);

	if (T == NULL)  //pokud neni nainicializovana tabulka fce vraci NULL
	{
	    free(var);
		return NULL;
	}

	if (T->first == NULL)  //pokud je prvni polozka NULL vraci NULL
	{
	    free(var);
		return NULL;
	}
	else
	{
		if (T->first->key == K)
		{
		    free(var);
			return T->first;  //vraceni prvni polozky
		}
		else
		{
			table_item_t *fptr = T->first;  //nacetni otcovskeho uzlu
			while(1)  //cyklus pro nerekurzivni hledani v tabulce symbolu
			{
				if (fptr->key > K)
				{
					if(fptr->LPtr == NULL)  //nenalezeni polozky
					{
					    free(var);
						return NULL;
					}
					else
					{
						fptr = fptr->LPtr;  //skok na zacatek cyklu s novym otcem
					}
				}
				else
				{
					if (fptr->key < K)
					{
						if(fptr->RPtr == NULL)  //nenalezeni polozky
						{
						    free(var);
							return NULL;
						}
						else
						{
							fptr = fptr->RPtr;  //skok na zacatek cyklu s novym otcem
						}
					}
					else
					{
					    free(var);
						return fptr;  //nalez polozky
					}
				}
			}
		}
	}
}

/**
 * @brief      pomocna fuknce pro niceni tabulky symbolu
 *
 * @param      fptr     odkaz na prvni prvek tabulky symbolu
 *
 * @return     vraci bool, podle toho zda-li bylo zniceni tabulky uspesne
 */
bool symbtab_del_sec(table_item_t *fptr)
{
	if (fptr != NULL)  //pojistka aby se nerusil prazdny strom
	{
		symbtab_del_sec (fptr->LPtr);  //nejdrive se zrusi nejlevejsi uzly
		symbtab_del_sec (fptr->RPtr);  //pote se z rekurze vynoruje a rusi se prave
		destroy_data(&(fptr->data)); // znicime strukturu data
		free(fptr);  //ruseni uzlu
		fptr = NULL;
	}
	return true;
}

/**
 * @brief      funkce nici tabulku symbolu
 *
 * @param      T     odkaz na tabulku symbolu
 *
 * @return     vraci bool, podle toho zda-li bylo zniceni tabulky uspesne
 */
bool symbtab_del(symb_table_t *T)
{
	if ((symbtab_del_sec (T->first)) == true)
	{
	    string_del(T->table_id);
		free(T);
		T = NULL;
		return true;
	}
	else
	{
		return false;
	}
}

/**
 * @brief      hashovaci funkce pro tabulku symbolu (hashovani podle nazvu promenne nebo funkce)
 *
 * @param      var   datova struktura obsahujici nazev promenne nebo funkce
 *
 * @return     vraci unsigned long int
 */
unsigned long hashfunction(data_t *var)
{
	//prevzate ze stranek:
	//https://www.programmingalgorithms.com/algorithm/sdbm-hash?lang=C
	unsigned long hash = 0;
	unsigned int i = 0;
	unsigned int length = strlen(var->name->string);  //zjisteni delky nazvu
	char *str = (var->name->string);

	for (i = 0; i < length; str++, i++)
	{
		hash = (*(str)) + (hash << 6) + (hash << 16) - hash;
	}

	return hash;
}

/**
 * @brief      vytvori strukturu data_t a naplni ji
 *
 * @param      s     nazev promenne nebo funkce
 * @param[in]  type  typ promenne (int, double...)
 *
 * @return     vraci odkaz na vytvorenou strukturu data_t
 */
data_t *data_create(String *s, int type, instr_t *f_start)
{
    data_t *ret;
    ret = malloc(sizeof(struct data_s));
    ret->type = type;
    ret->name = s;
    ret->defined = false;
    ret->value = NULL;
    ret->f_start = f_start;
    ret->params = NULL;
    return ret;
}

/**
 * @brief      znici strukturu data_t
 *
 * @param      var   odkaz na strukturu data_t
 */
void destroy_data(data_t *var)
{
    if(var->type != T_STRING)
    {
        free(var->value);
    }
    else
    {
        string_del(var->value);
    }
    if(var->params != NULL)
    {
        string_del(var->params);
    }
    string_del(var->name);
    //free(var); //neni dynamicky... 
}

/**
 * @brief      funkce zmeni v data_t definovanost či nedefinovanost promenne na opacnou hodnotu
 *
 * @param      var   struktura promenne pro zmenu definovanosti
 */
void define_var(data_t *var)
{
		var->defined = true;
}
