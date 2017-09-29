/**
 * @author Petr Buchal
 * @date 20/11/2016
 * @file symbol_table.h
 * @brief symbol table implementation
 * Copyright (c) 2016
 */

//tabulka symbolu musi byt implementovana v ial.c, ne samostane
#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include <stdbool.h>

#include "symbol_table.h"
#include "instr.h"


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
 * @brief      vytvori strukturu data_t a naplni ji
 *
 * @param      s     nazev promenne nebo funkce
 * @param[in]  type  typ promenne (int, double...)
 *
 * @return     vraci odkaz na vytvorenou strukturu data_t
 */
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
