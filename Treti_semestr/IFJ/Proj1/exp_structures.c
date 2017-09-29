 /**
 * @author Pavel Cerny, Petra Buchtova
 * @date 1_12_2016
 * @file exp_structures.c
 * @brief obsahuje pomocne struktury pro analyzator vyrazu
 * Copyright (c) 2016
 */

#include <stdlib.h>
#include <stdbool.h>
// ? #include "stack_symbol_table.h"
#include "internal_codes.h"
#include "exp_structures.h"
#include "symbol_table.h"
#include "ret_code.h"


exp_item_t * exp_item_new(void)
{
    exp_item_t * item = malloc(sizeof(exp_item_t));
    item->item = PREC_ERR;
    return item;    
}
void exp_item_del(exp_item_t * item)
{
    free(item);
}

exp_stack_t *exp_stack_new(void)
{
    exp_stack_t *s = (exp_stack_t *) malloc(sizeof(exp_stack_t));
    
    if (s == NULL)
        return NULL;

    s->stk = (exp_item_t **) malloc(STACK_SIZE*sizeof(exp_item_t *));

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

void exp_SPush(exp_stack_t *s, exp_item_t *item)
{
    if ((s->top + 1) <= s->alloc_size)
    {
        int top = s->top;
        s->stk[++top] = item;
        s->top = top;
    }
    else
    {
        if ((s->stk=(exp_item_t**)realloc(s->stk, sizeof(exp_item_t)*s->nxt_realloc_size)) == NULL)
        {
            ;  //error
        }
        else
        {
            s->alloc_size = s->nxt_realloc_size;
            s->nxt_realloc_size*=2;

            exp_SPush(s, item);
        }
    }
}

exp_item_t *exp_STop(exp_stack_t *s)
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

void exp_SPop(exp_stack_t *s)
{
    exp_item_del(s->stk[s->top]);
    (s->top <= -1)?0: s->top--;
}

bool exp_SEmpty(exp_stack_t *s)
{
    return s->top == -1 ? 1 : 0;
}

void exp_stack_del(exp_stack_t *s)  //dodelat
{
    for( ; s->top != -1; s->top--)
    {
        if(s->stk[s->top] == NULL)
        {
            break;
        }
        else
        {
            exp_item_del(s->stk[s->top]);  // sem pridat fci na niceni itemu
        }
    }
    if(s != NULL)
    {
        free(s);
    }
}


// --- single linked list for function arguments ---

struct arg_list * arg_list_new(void) // creates and inits list of func arguments
{
	struct arg_list * alist = (struct arg_list*) malloc (sizeof(struct arg_list)); 
	alist->first = NULL;
	alist->last = NULL;

	return alist;
}
int arg_add(struct arg_list * list, table_item_t * item)
{
	struct arg_list_item *new_item = (struct arg_list_item*) malloc(sizeof(struct arg_list_item));
	if (new_item == NULL)
		return ERR_INTERNAL;

	new_item->table_elem = item;
	new_item->next = NULL;

	if (list->first == NULL) 
		list->first = new_item;
	else 
		list->last->next = new_item;

	list->last= new_item;

	return ERR_NONE;
}

void arg_list_destroy(struct arg_list * list)
{
	struct arg_list_item *pom;

	while (list->first != NULL)
	{
		pom = list->first;
		free (list->first);
		list->first = pom;	
	} 
	list->last = NULL; 
	
}

