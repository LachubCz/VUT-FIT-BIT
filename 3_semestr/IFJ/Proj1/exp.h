 /**
 * @author Pavel Cerny, Petra Buchtova
 * @date 2_12_2016
 * @file exp.h
 * @brief hlavickovy soubor analyzatoru vyrazu
 * Copyright (c) 2016
 */
 
#ifndef EXP_H /* This definition has to correspond to file name */
#define EXP_H /* . */

#define TERMS 17
#define RULES 18
#define RULE_MAX_LENGTH 6  

#include "ilist.h"

#include "symbol_table.h"
#include "token.h"
#include "stack_symbol_table.h"


int parse_exp(token_t *token, ilist_t L, symb_table_t *ST, symb_table_t *STlocal, String *class);

#endif
