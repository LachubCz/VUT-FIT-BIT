  /**
 * @author Pavel Cerny, Petra Buchtova
 * @date 3_12_2016
 * @file exp_structures.h
 * @brief hlavickovy soubor pro exp_structures.c
 * Copyright (c) 2016
 */ 

#ifndef EXP_STRUCTURES_H
#define EXP_STRUCTURES_H
//#include "exp.h"
#include "symbol_table.h"

#ifndef PREC_ERR
#define PREC_ERR 21 /* This is hack, we cant include exp_data_defs.h here, as it will cause multiple definition error.
                       Please accept this ugly hack for now. */
#endif

typedef struct {
	int item;  // enum type in exp_data_defs.h
	void *data;
    /* --- 
    union {
		table_item_t * record;
		struct arg_list * args; 
	};*/
      // id, lit and nonterminals E need reference to record in symbol table_s
      // function call (especially nonterminal P) needs list of function arguments
} exp_item_t;


typedef struct exp_stack_s {
    exp_item_t **stk;
    int top;
    size_t alloc_size;
    size_t nxt_realloc_size;
} exp_stack_t;

exp_item_t * exp_item_new(void);
exp_stack_t *exp_stack_new(void);
void exp_stack_del(exp_stack_t *s);
void exp_SPush(exp_stack_t *s, exp_item_t *item);
exp_item_t *exp_STop(exp_stack_t *s);
void exp_SPop(exp_stack_t *s);
bool exp_SEmpty(exp_stack_t *s);




// --- single linked list for function arguments ---
struct arg_list_item {
    table_item_t *table_elem;
    struct arg_list_item *next;
}; 

struct arg_list {
    struct arg_list_item * first;
    struct arg_list_item * last;
    //int count;
};

struct arg_list * arg_list_new(void); // creates and inits list of func arguments
int arg_add(struct arg_list * list, table_item_t * item);

void arg_list_destroy(struct arg_list * list);




#endif



