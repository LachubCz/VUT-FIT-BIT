/**
 * @author Radim Cervinka
 * @date 21/11/2016
 * @file ilist.h
 * @brief Linked list for pseudo-instructions
 * Copyright (c) 2016
 */



#ifndef ILIST_H /* This definition has to correspond to file name */
#define ILIST_H /* . */

#include <stdio.h>
#include "token.h"
#include "symbol_table.h"
#include "instr.h"

/**
* @brief Header for instruction list
*/
typedef struct ilist_s {
    instr_t *first; /**< First instruction on the list */
    instr_t *last;  /**< Last instruction on the list */
    instr_t *active; /**< Active instruction */
} *ilist_t;

/**
* @brief Initialize new instruction list
* @return pointer to newly created list
*/
ilist_t ilist_new();

/**
* @brief Create new instruction
* @param  i_type type of instruction
* @param  a1 1st operand
* @param  a2 2nd operand
* @param  a3 3rd operand
* @return pointer to newly created instruction
*/
instr_t *ilist_item_creator(int i_type, void *a1, void *a2, void *a3);

/**
* @brief Insert instruction to the end of list
* @param L instruction list
* @param I instruction
*/
void ilist_insert_last(ilist_t L, instr_t *I);

/**
* @brief Insert instruction after active, does not change activity!
* @param L instruction list
* @param I instruction
*/
void ilist_insert_after_active(ilist_t L, instr_t *I);

/**
* @brief Move activity to next item on list
* @param L instruction list
*/
void ilist_next(ilist_t L);

/**
* @brief Free entire list of instructions
* @param L instruction list
*/
void ilist_free(ilist_t L);

/**
* @brief Return pointer to current instruction
* @param L instruction list
*/
instr_t *ilist_data_get(ilist_t L);

/**
* @brief Create if block after active instruction
* @param T symbol table
* @param L instruction list
* @param cond if condition
*/
void ilist_if_creator(symb_table_t T, ilist_t L, void *cond);

/**
* @brief Create while block after active instruction
* @param T symbol table
* @param L instruction list
* @param cond if condition
*/
void ilist_while_creator(symb_table_t T, ilist_t L, void *cond);

#endif
