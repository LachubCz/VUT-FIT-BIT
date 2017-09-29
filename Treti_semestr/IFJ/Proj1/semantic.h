/**
 * @author Pavel Cerny 
 * @date 1_12_2016
 * @file semantic.h
 * @brief header file for semantic analysis functions
 * Copyright (c) 2016
 */
 
 
#include<stdio.h>
#include "ret_code.h"
#include "exp_structures.h"
// #include "exp_data_defs.h"
#include "stack_symbol_table.h"
/*
enum {
    T_BOTH_INT,
    T_BOTH_NUMBERS,
    T_FIRST_STRING,
    T_SECOND_STRING,
    T_BOTH_STRING,
    T_ERROR
}; */

int type_comparison(data_t *add1, data_t *add2, int operation);
/*
bool define_control (stack_symbol_table_t *frames, symb_table_t *global);
bool symbtab_control_sec(table_item_t *fptr);
bool symbtab_control(symb_table_t *T);
*/
int get_func_ret_type(data_t *function);
int func_call(data_t *result, struct arg_list *arguments, data_t *function);
bool is_fully_qual(String * id_name);
int str_to_type(String * params, int idx);
