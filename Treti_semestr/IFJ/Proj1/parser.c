/**
 * @author Marek Dolezel
 * @date 11/13/2016
 * @file parser.c
 * @brief Implementation of syntax analyzer.
 * Copyright (c) 2016
 */

#include <stdbool.h>
#include <stdio.h>

#include "stack_ints.h"
#include "ret_code.h"
#include "parser.h"
#include "token.h"
#include "scanner.h"
#include "internal_codes.h"
#include "parser_data_defs.h"
#include "exp.h"

#include "stack_symbol_table.h"
#include "symbol_table.h"
#include "ilist.h"
#include "instr.h"

void print(stack_ints_t *stack)
{
}
/*
  *@brief Tests top of the stack for being terminal.
  *@param X Top of the stack
 */
bool isTerminal(int X)
{
    int i = 0;
    while (terminals[i] != -1)
    {
        if (terminals[i] == X)
            return true;
        i++;
    }
    return false;
}
/*
* @brief Test whether top of stack and token are the same.
* @param CT current token
* @param X  top of stack
 */
bool match(int CT, int X)
{
    return CT == X ? true:false;
}
/*
* @brief Replace Non-terminal with other terminals and/or non-terminals.
* @param s Stack where we do the replacement
* @param rule We replace by the rule.
 */
void replace(stack_ints_t *s,int rule)
{
    int i=0;
    int item;
    SPop(s);    // r020 {-1             }

    while((item=ReplaceArr[rule][i]) != -1)
    {

        SPush(s,item);
        i++;

    }
}
/*
* @brief Create data structures for parser.
* @param pointer to data structure to create.
*/
int parser_create(p_data_t *data)
{
    data->stack = stack_new();
    data->token = token_new();

    if (data->stack == NULL || data->token == NULL) {
        if (data->stack != NULL)
            stack_del(data->stack);
        else if (data->token != NULL)
            token_del(data->token);

        return I_ERR_MALLOC;
    }
    return I_ERR_OK;
}
/*
* @brief Destroy data structures for parser.
* @param pointer to data structure to destroy.
*/void parser_destroy(p_data_t *data)
{
    stack_del(data->stack);
    token_del(data->token);
}



int insert_to_table(stack_symbol_table_t *frames, String *var_name, int type )
{
    /* Symbol table for particular function */
    symb_table_t *local = STop_st(frames);

    /* Check if this is not redefinition */
    if (symbtab_search(local, var_name) != NULL)
        return ERR_SEM_UNDEF;
    data_t *new_data = data_create(var_name, type, NULL);

    symbtab_insert(local, new_data);
    return ERR_NONE;
}

bool search_local(stack_symbol_table_t *frames, String *var_name)
{
    /* Symbol table for particular function */
    symb_table_t *local = STop_st(frames);

    /* Check if found */
    if (symbtab_search(local, var_name) != NULL)
        return true;
    else 
        return false;
    
}

/* Create new symbol_table on the stack for function */
int new_frame(stack_symbol_table_t *frames, String *func_name)
{
    symb_table_t *new = symbtab_new();
    new->table_id = func_name;
    SPush_st(frames, new);
}

/* Check whether identifier is fully qualified */
bool is_fq(String *name) {
    return strstr(name->string, ".") != NULL ? true : false;
}

/* Parser subrutine that handles syntax analysis */
int parse_driver(ilist_t L, symb_table_t *ST, stack_symbol_table_t *frames)
{

    p_data_t pdata; /* Data structures for parser to work with */
    token_type_t prev_token_type;
    String *current_class = NULL;
    String *current_function = NULL;
    String *current_id  = NULL;



    int error; /* Error from scanner */
    int CT, X; /* CT -> current token, X -> top of the stack */

    int state = 0; /* Current state of FSM for handling local variables */

    if (parser_create(&pdata) != I_ERR_OK)
        return ERR_INTERNAL;

    SPush(pdata.stack, T_DOLAR);
    SPush(pdata.stack, NT_CLASS_LIST);


    if ((error=token_get(pdata.token)) != ERR_NONE)
    {
             parser_destroy(&pdata);
             return error;
    }


    CT = (int) pdata.token->token_type;



    while(!SEmpty(pdata.stack))
    {
        prev_token_type = CT;


        X = STop(pdata.stack);

        if (X == T_DOLAR)
		{
			parser_destroy(&pdata);
			return ERR_NONE;
        }

        /* When terminal is on top of the stack */
        if (isTerminal(X))
        {

            bool result = match(CT, X);
            if (result != true)
            {

				        parser_destroy(&pdata);
                return ERR_SYNTAX;
            }

            SPop(pdata.stack);


            if ((error=token_get(pdata.token)) != ERR_NONE)
			      {
                if (error != T_DOLAR)
				        {
                    parser_destroy(&pdata);
                    return error;

                }
            }

            /* static TYPE name; -> global variable
               static TYPE name = -> global variable
               above are handled by pre_parser ^

               Here we need to:
                  1) detect local variable + save to loc.symbtable
                  2) detect function entry + create new loc.symbtable
                  3) detect function call + check with glob.symbtable
             */

            int token_type = pdata.token->token_type;
            switch(state)
            {
                case 0:
                    if(token_type == TK_ID) {
                        if(prev_token_type != TK_CLASS) {
                            current_id = pdata.token->str;
                        //current_function = pdata.token->str;
                        state = 1;
                        break;
                        }
                        state = 0;
                        break;
                    }
                    else if (token_type == TK_STATIC) {
                        state = 3;
                        break;
                    }
                    else if (token_type == TK_INT || token_type == TK_STRING || token_type == TK_DOUBLE) {
                        state = 2;
                        break;
                    }
                    else if (token_type == TK_CLASS) {
                        state = 0;
                        break;
                    }
                    break;
                case 1:
                    if (token_type == TK_LRBRACKET) { /* function call */

                        /* Check if function was defined */
                        if(symbtab_search(ST, str_fq(current_class,current_id)) == NULL)
                            return ERR_SEM_UNDEF;
                            
                        state = 0;
                        break;
                    }
                    else if (token_type == TK_SEMICOLON || token_type == TK_ASSIGNMENT) {
                        /* variable use */

                        /* check local table for variable */
                        if(!search_local(frames, str_fq(current_class,current_id)))
                        {   /* if not found in local table look in global */
                            if(symbtab_search(ST, str_fq(current_class,current_id)) == NULL)
                                return ERR_SEM_UNDEF;
                        }

                        state = 0;
                        break;
                    }
                    state = 0;
                    break;
                case 2:
                    if (token_type == TK_ID) {
                        current_id = pdata.token->str;

                        int type; /* Convert token type for symbol table */
                        switch(prev_token_type) {
                            case TK_INT:
                                type = T_INT;
                                break;
                            case TK_DOUBLE:
                                type = T_DOUBLE;
                                break;
                            case TK_STRING:
                                type = T_STRING;
                                break;

                        }
                        int ret;
                        /* Check for redefinition */
                        if((ret=insert_to_table(frames, current_id, type)) != ERR_NONE) {
                            return ret;
                        }

                        state = 0;
                        break;
                    }
                    else {
                        state = 0;
                        break;
                    }
                    state =0;
                    break;
                case 3:
                    if(token_type == TK_VOID || token_type == TK_INT || token_type == TK_DOUBLE) {
                        state = 4;
                        break;
                    }
                    else {
                        state = 0;
                        break;
                    }
                    state = 0;
                    break;
                case 4:
                    if (token_type == TK_ID) {
                        state = 5;
                        current_id = pdata.token->str;
                        break;
                    }
                    state = 0;
                    break;
                case 5:
                    if(token_type == TK_LRBRACKET) {
                        /* Function definition */
                        new_frame(frames, current_id);
                        state = 0;
                        break;
                    }
                    state = 0;
                


            }





            if (error != T_DOLAR)
			{
                CT = (int) pdata.token->token_type;
            }
            else CT=T_DOLAR;

        }
        /* When Non-terminal is on to of the stack we replace it */
        else if (T[X-42][CT] != 0)
		{ // X should not be smaller than 42, otherwise we get segfault
            replace(pdata.stack, T[X-42][CT]);
        }
        else if (X == NT_EXP) /* When non-terminal Exp is on top of the stack we call submodule */
        {


            if ((error=parse_exp(pdata.token, L, ST, ST /* here will be local ST*/, current_class)) != ERR_NONE) /// Vyřešit výsledek EXP (přiřazení, podmínka)
            {
				parser_destroy(&pdata);
				return error;
			}


            SPop(pdata.stack);


			CT = (int) pdata.token->token_type;
			X = STop(pdata.stack);

			if (X == NT_ARGS)
			{
				continue;
			}
			else
            {
                // print(pdata.stack);

                if ( match(CT,X) )
                    SPop(pdata.stack);
                else
                {
                    parser_destroy(&pdata);
                    return ERR_SYNTAX;
                }


                if ((error=token_get(pdata.token)) != ERR_NONE)
                {
                    if (error != T_DOLAR)
                    {
                        parser_destroy(&pdata);
                        return error;

                    }
                }
                if (error != T_DOLAR)
                {
                    CT = (int) pdata.token->token_type;
                }
                else CT=T_DOLAR;

			}
    } /* Action: When we find TRAP_CLASS_ID, we try to catch current class name */
    else if(X>100) {
        if (X == TRAP_CLASSID) {
            SPop(pdata.stack);
            current_class = pdata.token->str;
        }
    }
    else
    {
        parser_destroy(&pdata);
        return ERR_SYNTAX;
    }
    }
    return ERR_NONE;
    }
// vstupni bod parseru
int parser_entry(ilist_t L, symb_table_t *ST, stack_symbol_table_t *frames)
{
    return parse_driver(L,ST,frames);
}
