/**
 * @author Petr Buchal
 * @date 25_11_2016
 * @file stack_symbol_table.h 
 * @brief hlavickovy soubor pro stack tabulek symbolu
 * Copyright (c) 2016
 */
 
#ifndef STACK_SYMBOL_TABLE_H
#define STACK_SYMBOL_TABLE_H

#include <stdbool.h>
#include "symbol_table.h"

typedef struct stack_symbol_table_s {
    symb_table_t **stk;
    int top;
    size_t alloc_size;
    size_t nxt_realloc_size;
} stack_symbol_table_t;

stack_symbol_table_t *stack_new_st(void);
void stack_del_st(stack_symbol_table_t *s);
void SPush_st(stack_symbol_table_t *s, symb_table_t *item);
symb_table_t *STop_st(stack_symbol_table_t *s);
void SPop_st(stack_symbol_table_t *s);
bool SEmpty_st(stack_symbol_table_t *s);

#endif 
