/**
 * @author Marek Dolezel
 * @date 10/2/2016
 * @file parser.h
 * @brief Interface for syntax analyzer submodule
 * Copyright (c) 2016
 */

#ifndef PARSER_H /* This definition has to correspond to file name */
#define PARSER_H /* . */

#include "token.h"
#include "stack_ints.h"

#include "stack_symbol_table.h"
#include "symbol_table.h"
#include "ilist.h"
#include "instr.h"

/**
 * entry point to syntax analyzer submodule
 * @param  L      pointer to instruction list
 * @param  ST     pointer to global symbol table
 * @param  frames pointer to array of local symbol tables
 * @return        error code defined in ret_code.h
 */
int parser_entry(ilist_t L, symb_table_t *ST, stack_symbol_table_t *frames);

#define X_MAX 100 // proc 100?? :o non-terminalu je 16 :D (s EXP 17)
#define CT_MAX 100 // tech taky neni uplne sto :D

#define T_DOLAR 41

/**
 * @brief Data structures for syntax analyzer.
 */
typedef struct
{
    stack_ints_t *stack; /**< Stack for terminals, non-terminals */
    token_t *token; /**< Current token being processed */
} p_data_t;

/**
 * Helper function - prints stack content.
 * @param stack   stack to print
 */
void print(stack_ints_t *stack);

#endif
