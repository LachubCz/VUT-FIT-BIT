/**
 * @author Marek Dolezel
 * @date 13_11_2016
 * @file stack_ints.h
 * @brief Interface for stack submodule
 * Copyright (c) 2016
 */

#ifndef STACK_INTS_H
#define STACK_INTS_H

#include <stdbool.h>

/**
 * @brief Data structure for stack
 */
typedef struct stack_ints_s {
    int *stk; /** < Pointer to array of items */
    int top;  /** < Index to current top of stack */
    size_t alloc_size; /**< Currently allocated space */
    size_t nxt_realloc_size; /**< Space to be allocated on next realloc */
} stack_ints_t;
/**
 * Create new stack.
 * @return  Pointer to newly allocated stack.
 */
stack_ints_t *stack_new(void);

/**
 * Destroy stack.
 * @param stack Pointer to stack to be destroyed.
 */
void stack_del(stack_ints_t *stack);
/**
 * Push new item onto the stack
 * @param stack Pointer to stack
 * @param item  new item
 */
void SPush(stack_ints_t *stack, int item);
/**
 * Check top of stack
 * @param  stack Pointer to stack
 * @return       item on top of stack
 */
int STop(stack_ints_t *stack);

/**
 * Pop item of the stack
 * @param stack pointer to stack
 */
void SPop(stack_ints_t *stack);
/**
 * Check if stack is empty
 * @param    stack  pointer to stack
 * @return   True, False
 */
bool SEmpty(stack_ints_t *stack);

#endif
