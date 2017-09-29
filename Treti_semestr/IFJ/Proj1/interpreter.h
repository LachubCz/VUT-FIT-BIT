/**
 * @author Radim Červinka
 * @date 8_12_2016
 * @file interpreter.h
 * @brief hlavickovy soubor interpreteru
 * Copyright (c) 2016
 */

#ifndef INTERPRETER_H /* This definition has to correspond to file name */
#define INTERPRETER_H /* . */

/**
* @brief Entry point to interpreter submodule
* @param T pointer to symbol table
* @param ilist pointer to instruction list
*/
int interpret(symb_table_t *T, ilist_t ilist);

#endif
