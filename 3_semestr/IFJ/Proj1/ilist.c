/**
 * @author Radim Červinka 
 * @date 18/11/2016
 * @file ilist.c
 * @brief module for operations with instruction list
 * Copyright (c) 2016
 */

#include<stdio.h>
#include "ilist.h"
#include "instr.h"

ilist_t ilist_new()
{
  ilist_t L = malloc(sizeof(struct ilist_s));
  if (L != NULL)
  {
    L->first = NULL;
    L->last = NULL;
    L->active = NULL;
  }
  else
  {
      fprintf(stderr, "Doslo k alokacni chybe pri inicializaci seznamu instrukci.");
      return NULL; // potreba osetrit v mainu
  }
  return L;
}

instr_t *ilist_item_creator(int i_type, void *a1, void *a2, void *a3)
{
  instr_t *I = malloc(sizeof(struct instr_s));
  if (I == NULL)
  {
    fprintf(stderr, "Chyba alokace noveho prvku seznamu instrukci, funkce ilist_item_creator");
    return NULL; // potreba osetrit v mainu
  }
  
  I->instrType = i_type;
  I->addr1 = a1;
  I->addr2 = a2;
  I->addr3 = a3;
  I->next = NULL;
  
  return I;
}
                     
void ilist_insert_last(ilist_t L, instr_t *I)
{
   if ((L != NULL) && (I != NULL))
   {
    L->last->next = I;
    L->last = I;
    I->next = NULL;
   }
   else
   {
     fprintf(stderr, "Doslo k chybe ve funkci ilist_insert_last."); 
     fprintf(stderr, "Funkci byl predan NULL ukazatel na instrukci nebo seznam instrukci.");
   }
}

void ilist_insert_after_active(ilist_t L, instr_t *I)
{
   if ((L != NULL) && (I != NULL) && (L->active != NULL))
   {
    I->next = L->active->next;
    L->active->next = I;
   }
   else
   {
     fprintf(stderr, "Doslo k chybe ve funkci ilist_insert_after_active."); 
     fprintf(stderr, "Funkci byl predan NULL ukazatel na instrukci nebo seznam instrukci.");
   }
}

void ilist_next(ilist_t L)
{
    if(L == NULL)
    {
      fprintf(stderr, "Funkce ilist_data_get byla zavolana s NULL pointrem na seznam instrukci.");
      return;  // potreba osetrit v mainu
    }
  
    if (L->active != NULL)
      L->active = L->active->next;
}


void ilist_free(ilist_t L)
{
  if (L == NULL) {
    return;
  }
  
   while (L->first != NULL)
   {
      L->active = L->first;
      L->first = L->first->next;
      free(L->active);
   }
   free(L);
}


instr_t *ilist_data_get(ilist_t L)
{
    if(L == NULL)
    {
      fprintf(stderr, "Funkce ilist_data_get byla zavolana s NULL pointrem na seznam instrukci.");
      return NULL;  // potreba osetrit v mainu
    }
      
    if(L->active != NULL)
    {
      return L->active;
    }
    else
    {
      fprintf(stderr, "Funkce ilist_data_get byla zavolana nad neaktivnim seznamem.");
      return NULL; // potreba osetrit v mainu
    }
}


void ilist_if_creator(symb_table_t T, ilist_t L, void *cond)
{
  if (L == NULL) {
    return;
  }
  
  // udelej EOI
  instr_t *EOI = ilist_item_creator(I_LABEL, NULL, NULL, NULL);
  if (EOI == NULL)
  {
    // pruser, chyba, resit
    return;
  }

  
  // udelej JMP diky add EOI
  instr_t *JMP = ilist_item_creator(I_JMP, NULL, NULL, EOI);
  if (JMP == NULL)
  {
    // pruser, chyba, resit
    return;
  }

    
  // udelej ELSE label
  instr_t *ELSE = ilist_item_creator(I_LABEL, NULL, NULL, NULL);
  if (ELSE == NULL)
  {
    // pruser, chyba, resit
    return;
  }

  
  // udelej JNC diky add label else
  instr_t *JNC = ilist_item_creator(I_JNC, cond, NULL, ELSE);
  if (JNC == NULL)
  {
    // pruser, chyba, resit
    return;
  }
  
  // dam to do seznamu
  ilist_insert_after_active(L, EOI);
  ilist_insert_after_active(L, ELSE);
  ilist_insert_after_active(L, JMP);
  ilist_insert_after_active(L, JNC);
  ilist_next(L); // posunu 1 aktivitu
     
}

void ilist_while_creator(symb_table_t T, ilist_t L, void *cond)
{
  if (L == NULL ) {
    return;
  }
  
  // udelej EOI
  instr_t *SOW = ilist_item_creator(I_LABEL, NULL, NULL, NULL);
  if (SOW == NULL)
  {
    // pruser, chyba, resit
    return;
  }

  
  // udelej JMP diky add EOI
  instr_t *JMP = ilist_item_creator(I_JMP, NULL, NULL, SOW);
  if (JMP == NULL)
  {
    // pruser, chyba, resit
    return;
  }

    
  // udelej ELSE label
  instr_t *EOW = ilist_item_creator(I_LABEL, NULL, NULL, NULL);
  if (EOW == NULL)
  {
    // pruser, chyba, resit
    return;
  }

  
  // udelej JNC diky add label else
  instr_t *JNC = ilist_item_creator(I_JNC, cond, NULL, EOW);
  if (JNC == NULL)
  {
    // pruser, chyba, resit
    return;
  }
  
  // dam to do seznamu
  ilist_insert_after_active(L, EOW);
  ilist_insert_after_active(L, JMP);
  ilist_insert_after_active(L, JNC);
  ilist_insert_after_active(L, SOW);
  ilist_next(L); // posunu 2x aktivitu
  ilist_next(L);
     
}
