/**
 * @author Marek Dolezel
 * @date 11/3/2016
 * @file stack_ints.c
 * @brief Implementation of dynamic stack_ints datatype.
 * Copyright (c) 2016
 */

/*
    Dear reader, you don't need to review this file. Think of it as black box.
    And one last thing, please don't ask me any questions.
*/

 #include <stdlib.h>
 #include <stdbool.h>
 #include "stack_ints.h"
 #include "internal_codes.h"

 stack_ints_t *stack_new(void)
 {
     stack_ints_t *s = (stack_ints_t *) malloc(sizeof(stack_ints_t));

     if (s == NULL)
        return NULL;

    s->stk = (int *) malloc(STACK_SIZE*sizeof(int));

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
void SPush(stack_ints_t *s, int item)
{
    if ((s->top + 1) <= s->alloc_size)
    {
        int top = s->top;
        s->stk[++top] = item;
        s->top = top;
    }
    else
    {   // Realloc when there is not enough space
        if ((s->stk=(int*)realloc(s->stk, sizeof(int)*s->nxt_realloc_size)) == NULL)
        {
        // malloc should not fail in practice
        }
        else
        {
            s->alloc_size = s->nxt_realloc_size;
            s->nxt_realloc_size*=2;

            SPush(s, item);
        }
    }
}

int STop(stack_ints_t *s)
{
    return s->top == -1? 0: s->stk[s->top];
}

void SPop(stack_ints_t *s)
{
    s->top <= -1 ?0: s->top--;
}

bool SEmpty(stack_ints_t *s)
{
    return s->top == -1 ? 1 : 0;
}

 void stack_del(stack_ints_t *s)
{
	free(s->stk);
	free(s);
}
