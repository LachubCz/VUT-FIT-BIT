/**
 * @author Marek Dolezel 
 * @date 11/06/2016
 * @file symbol_table.h
 * @brief Header file for symbol table implementation
 * Copyright (c) 2016
 */
 
#ifndef SYMBOL_TABLE_H /* This definition has to correspond to file name */
#define SYMBOL_TABLE_H /* . */

#include <stdio.h>
#include <stdbool.h>
#include "string.h"
#include "instr.h"

enum {
    T_INT,
    T_STRING,
    T_DOUBLE,
    T_FUNCTION,
    T_BOOL, 
	T_VOID
};

typedef struct  data_s {
    int type; /* type of identifier */
    String *name;
    void *value;
    bool defined;
    instr_t *f_start; // ukazatel na prvni instrukci funkce
    String *params; /* formal parameters of function */
        /* Let's suppose we have a function:
        static int foo(int a, String b, double c) 
        therefore params will be 'iisd'
                                  ^
                                  |----- return type 
        */
} data_t;

typedef struct table_item_s {
    unsigned long key; /* key is the name of ereridentifier */
    data_t data;  /* Data about identifier */
    struct table_item_s *LPtr; /* Pointer to next item on the left */
    struct table_item_s *RPtr; /* Pointer to next item on the right */
} table_item_t;

typedef struct table_s {
    struct table_item_s *first;
    String *table_id; // for identification of frame (to which function does the table belong)
} symb_table_t;

symb_table_t *symbtab_new();
bool symbtab_insert(symb_table_t *T, data_t *var);
table_item_t *symbtab_search(symb_table_t *T, String *s);
bool symbtab_del_sec(table_item_t *fptr);
bool symbtab_del(symb_table_t *T);
unsigned long hashfunction(data_t *var);

data_t *data_create(String *s, int type, instr_t *f_start);
void destroy_data(data_t *var);
void define_var(data_t *var);


#endif
