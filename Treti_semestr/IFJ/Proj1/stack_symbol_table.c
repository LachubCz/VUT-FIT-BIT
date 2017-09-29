/**
 * @author Petr Buchal
 * @date 25_11_2016
 * @file stack_symbol_table.c 
 * @brief implementace stacku tabulek symbolu
 * Copyright (c) 2016
 */

/* 
    Dear reader, you don't need to review this file. Think of it as black box.
    And one last thing, please don't ask me any questions.
*/
#include <stdlib.h>
#include <stdbool.h>
#include "stack_symbol_table.h"
#include "internal_codes.h"
#include <string.h>

stack_symbol_table_t *stack_new_st(void)
{
    stack_symbol_table_t *s = (stack_symbol_table_t *) malloc(sizeof(stack_symbol_table_t));
    
    if (s == NULL)
        return NULL;

    s->stk = (symb_table_t **) malloc(STACK_SIZE*sizeof(symb_table_t*));

    if (s->stk == NULL)
    {
        free(s);
        return NULL;
    }

    s->top = -1;
    s->alloc_size = STACK_SIZE;
    s->nxt_realloc_size = 2 * STACK_SIZE;

    return s;
}

void SPush_st(stack_symbol_table_t *s, symb_table_t *item)
{
    if ((s->top + 1) <= s->alloc_size)
    {
        int top = s->top;
        s->stk[++top] = item;
        s->top = top;
    }
    else
    {
        if ((s->stk=(symb_table_t**)realloc(s->stk, sizeof(symb_table_t)*s->nxt_realloc_size)) == NULL)
        {
            ;  //error
        }
        else
        {
            s->alloc_size = s->nxt_realloc_size;
            s->nxt_realloc_size*=2;

            SPush_st(s, item);
        }
    }
}

symb_table_t *STop_st(stack_symbol_table_t *s)
{
    if(s->top == -1)
    {
        return NULL;
    }
    else
    {
        return s->stk[s->top];
    }
}

void SPop_st(stack_symbol_table_t *s)
{
    s->top <= -1 ?0: s->top--;
}

bool SEmpty_st(stack_symbol_table_t *s)
{
    return s->top == -1 ? 1 : 0;
}

 void stack_del_st(stack_symbol_table_t *s)
{
    for (; s->top != -1; s->top--)
    {
        if(s->stk[s->top] == NULL)
        {
            break;
        }
        else
        {
            symbtab_del(s->stk[s->top]);
        }
    }
    if(s != NULL)
    {
        free(s);
    }
}

symb_table_t *get_local_symtab(stack_symbol_table_t *s, String *function)
{
    for(int i = 0; i<s->top; i++) {
        if (!strcmp(s->stk[i]->table_id->string, function->string ))
            return s->stk[i];
    }
}

bool save_local_symbtab(stack_symbol_table_t *s, symb_table_t *T, String *function) 
{   
    T->table_id = function;
    SPush_st(s,T);
   
}
