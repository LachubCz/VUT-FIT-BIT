/**
 * @author Pavel Cerny, Petra Buchtova
 * @date 2_12_2016
 * @file exp.c
 * @brief implementace analyzatoru vyrazu
 * Copyright (c) 2016
 */
#include <stdio.h>

#include "scanner.h"
#include "parser.h"
#include "token.h"
#include "parser.h"
#include "exp.h"
#include "exp_data_defs.h"
#include "exp_structures.h"
#include "ret_code.h"

#include "ilist.h"
#include "stack_symbol_table.h"
#include "symbol_table.h"
#include "exp_structures.h"
#include "string.h"
#include "semantic.h"

int first_go = 1; 

struct exp_needs{
	symb_table_t *STglobal;
	symb_table_t *STlocal;
	String *classname;
};


void print_exp(exp_stack_t *stack) 
{
    
}

bool is_term (prec_chars symbol)
{
	if (symbol <= END) // END is a prec_char with the biggest value... should be positive 
		return true;
	else 
		return false;
}

// returns terminal that is closest to the top of the stack  
prec_chars last_term (exp_stack_t *stack)
{
	int top = stack->top;
	// terminal should be either on top, or right below the top 
	// there can't be two nonterminals next to each other  
	if (is_term(stack->stk[top]->item))
		return stack->stk[top]->item; 
	else
		if (is_term(stack->stk[top-1]->item))
			return stack->stk[top-1]->item; 
		else 
			return PREC_ERR; 
}

// returns number of a rule matching with the content of the stack (current handle)  
// if there is no such a rule, returns -1  
int match_rule (exp_stack_t *stack)
{
	prec_chars act_rule[RULE_MAX_LENGTH]; // content of the stack 
	prec_chars act_char = (exp_STop(stack))->item;
	int i = 0;

	while (act_char != HDL_START) // store whole content of the stack until we meet start of the handle 
	{
		act_rule[i] = act_char;		
		act_char = (stack->stk[stack->top-(i+1)])->item;
		i++;
	}
	act_rule[i] = EOR; // mark the end of the rule 

	for (int j=0; j<RULES; j++)
	{ 
		i = 0;
		while (act_rule[i] != EOR)
		{
			if (act_rule[i] != prec_rules[j][i+1]) // at prec_rules[j][0] there is a nonterminal we reduce to - left side of the rule  
				break;
			i++;
		}
		if (prec_rules[j][i+1] == EOR) 
			return j;
	}
	// we didn't find any matching rule 
	return -1;
}

// inserts handle right after the last terminal 
void insert_handle (exp_stack_t *stack)
{
	exp_item_t *new_item = exp_item_new();
	new_item->item = HDL_START;
	exp_item_t *top = exp_STop(stack);
	if (is_term(top->item)) // there is a terminal on top of the stack 
	{
		exp_SPush (stack, new_item);
	}
		 
	else // there is a nonterminal on top of the stack (one in a row at most) 
	{ // we need to add the handle right after the terminal
		
		stack->top--; //  
		exp_SPush (stack, new_item); 
		exp_SPush (stack, top); // add the nonterminal back 
	}
}

void clean_rule(exp_stack_t *stack)
{
	prec_chars act_char = exp_STop(stack)->item;
	while (act_char != HDL_START)
	{
		exp_SPop(stack);
		act_char = exp_STop(stack)->item;
	}
	exp_SPop(stack);
}
// processes tokens to prec_chars (terminals)


int read_input (token_t *tk, exp_stack_t *stack, exp_item_t **SItem, struct exp_needs *info) 
{

	 String *id_str = NULL;
	 table_item_t *record = NULL;
	 data_t *new_rec = NULL;
	 *SItem = exp_item_new();
	 if ( *SItem == NULL )
		{
			return ERR_INTERNAL;
		}

	if (!first_go) {
		int err = token_get(tk);
		if (err != ERR_NONE)
			return err;
	}

	first_go = 0;
	
	
	switch (tk->token_type)
	{
		case TK_MULTI: (*SItem)->item = MUL; break; 
		case TK_DIV: (*SItem)->item = DIV; break; 
		case TK_PLUS: (*SItem)->item = PLUS; break; 
		case TK_MINUS: (*SItem)->item = MINUS; break; 
		case TK_LRBRACKET: (*SItem)->item = LPAR; break; 
		case TK_RRBRACKET: (*SItem)->item = RPAR;	break; 

		case TK_STRING_IMM: // nutno kontrolovat povolene akce 
			// Je potreba hodnotu podle typu nahrat do E => Docasnou promennou generujeme uz tady
			
			new_rec = data_create(NULL, T_STRING, NULL);
			new_rec->value = tk->str; // inicializace 
			(*SItem)->item = LIT; 
			(*SItem)->data = new_rec;
			break;
		case TK_INT_IMM:
			new_rec = data_create(NULL, T_INT, NULL);
			int *Ival = malloc(sizeof(int));
			if(Ival != NULL)
				*Ival = tk->i;
			new_rec->value = Ival;
			
			(*SItem)->item = LIT; 
			(*SItem)->data = new_rec;
			break;
		case TK_DOUBLE_IMM: 
			new_rec = data_create(NULL, T_DOUBLE, NULL);
			double *Dval = malloc(sizeof(double));
			if(Dval != NULL)
				*Dval = tk->d;
			new_rec->value = Dval;
			(*SItem)->item = LIT; 
			(*SItem)->data = new_rec;
			break;
 
		
		case TK_ID:
	// prohledat symbol table, pokud existuje, zjistit, zda se jedna o funkci, ci promennou
	// 	pokud neexistuje, jedna se o chybu

			id_str = tk->str;
			if ( is_fully_qual(id_str) )
				record = symbtab_search(info->STglobal, id_str);
			else
			{	
				//// LOKALNI TABULKY  !!
				if ( (record = symbtab_search(info->STlocal, id_str)) == NULL)
				{
					id_str = string_new();
					string_init_str(id_str, info->classname->string);
					string_add_char(id_str, '.');
					string_add_str(id_str, tk->str);
					record = symbtab_search(info->STglobal, id_str);
				} 
			}

			if (record == NULL)
				return ERR_SEM_UNDEF;
			// if fail, record = sybmtab_search(STglobal);
			int err = ERR_NONE;
			if ( record->data.type == T_FUNCTION ) 
				(*SItem)->item = FUNC; 
			else if ( (record->data.type == T_INT) || (record->data.type == T_DOUBLE) || (record->data.type == T_STRING))
					(*SItem)->item = ID;
			else err = ERR_INTERNAL;

			//destroy_data(record);
			if (err != ERR_NONE)
				return err;
			(*SItem)->data = &record->data;			
			break;

		case TK_COMMA: (*SItem)->item = COMMA; break;
		case TK_GREATER: (*SItem)->item = GREATER; break;
		case TK_LESSER: (*SItem)->item = LESSER; break;
		case TK_EQUALGREATER: (*SItem)->item = GRTEQL; break;
		case TK_EQUALLESSER: (*SItem)->item = LESEQL; break;
		case TK_EQUAL: (*SItem)->item = EQUAL; break;
		case TK_NONEQUAL: (*SItem)->item = NOTEQL; break;
		case TK_SEMICOLON: (*SItem)->item = END; break; // might be the end of an expression 
		// LL parser has to check 
		
		default: (*SItem)->item = PREC_ERR; break; // sorry, we don't accept other token types in expressions! 
	}
	
	if ((*SItem)->item == PREC_ERR) 
		return ERR_SYNTAX; 
	else 
		return ERR_NONE;
}



int do_exp_semantic(void **return_val, exp_stack_t *stack, int rule /*, stack_symbol_table_t * STstack, symb_table_t *STglobal*/) // will need also instruction tape
{
	//symb_table_t *table;
	data_t *function;
	struct arg_list *arguments;
	int err;
	switch(rule)
	{
		/*	!!!!  - instrukce (odkazy na prom. v atributech)
			*/
			// ---- ARITMETICS ----
		case 0 : // MUL E*E
			err = type_comparison(stack->stk[stack->top-2]->data, stack->stk[stack->top]->data, MUL);
			// type check and implicit conversion (here 'err' is used for type of new auxiliary variable)
			if( err != -1)
			{
				if ( (*return_val = data_create(NULL , err, NULL)) == NULL )
					return ERR_INTERNAL;
			}
			else return ERR_SEM_TYPE_COMP;
			
			break;
		case 1 : // DIV 
			err = type_comparison(stack->stk[stack->top-2]->data, stack->stk[stack->top]->data, DIV);
			if( err != -1)
			{
				if ( (*return_val = data_create(NULL , err, NULL)) == NULL )
					return ERR_INTERNAL;
			}
			else return ERR_SEM_TYPE_COMP;
			
			break;
		case 2 : // PLUS
			err = type_comparison(stack->stk[stack->top-2]->data, stack->stk[stack->top]->data, PLUS);
			if( err != -1)
			{
				if ( (*return_val = data_create(NULL , err, NULL)) == NULL )
					return ERR_INTERNAL;
			}
			else return ERR_SEM_TYPE_COMP;
			break;
		case 3 : // MINUS
			err = type_comparison(stack->stk[stack->top-2]->data, stack->stk[stack->top]->data, MINUS);
			if( err != -1)
			{
				if ( (*return_val = data_create(NULL , err, NULL)) == NULL )
					return ERR_INTERNAL;
			}
			else return ERR_SEM_TYPE_COMP;
			break;


			// ---- SIMPLES ----
		case 4 : // (E)
			//	- zkopirovat atributy instance (E) do E
			*return_val = (stack->stk[stack->top -1])->data; 
			break;
		case 5 : // id
			//	(= navazat neterminal E na zaznam v symbol table [muze byt lokalni i globalni])					
		case 6 : // lit
			//	Pomocna promenna byla vytvorena uz v read_input( ... , table_item_t **value) => provadime pouze akce pro 'id' 		
			*return_val = (exp_STop(stack))->data;	
			break;


			/* ---- FUNCTIONS ----
				- pamatovat si typy parametru pro kontrolu typu podle deklarace
				- instrukce pro vlozeni na zasobnik
					- (top zasobniku?)
					- aktualni (/naslednou) pozici v ilist
					- pozice promenne pro navratovou hodnotu (pristupnou po navratu z funkce)
					- hodnoty vyrazu odpovidajici parametrum funkce (promenne s id formalniho parametru)
					- instrukce pro nepodmineny skok 
			*/
		case 7 : // f()
			function = (stack->stk[stack->top-2])->data;
			err = get_func_ret_type(function); // such as type of return value
			if(err == -1) // yeah, void return type is error in expression
			{
				return ERR_RUN_UNINIT_VAR;
			}
			*return_val = data_create(NULL, err, NULL);
				return func_call(*return_val, NULL, function);				
			break;

		case 8 : // f(E)
			arguments = arg_list_new(); // create list of arguments so we can give it to the function
			err = arg_add(arguments, (stack->stk[stack->top-1])->data ); // ..and feed it with the E data
			if( err != ERR_NONE ){
				arg_list_destroy (arguments);
				return err;
			}

			function = stack->stk[stack->top-3]->data; // get function metadata
			err = get_func_ret_type(function); // such as type of return value
			if(err == -1) // yeah, void return type is error in expression
			{
				arg_list_destroy(arguments);
				return ERR_RUN_UNINIT_VAR;
			}
			*return_val = data_create(NULL, err, NULL); 
			err = func_call(*return_val, arguments, function); // provadi semanticke kontroly a generuje instrukce volani funkce
			if(err != ERR_NONE)
			{
				destroy_data(*return_val);
				arg_list_destroy(arguments);
				return err;
			}
			break;
		case 9 : // f(P)
			function = stack->stk[stack->top-3]->data;
			err = get_func_ret_type(function); 
			if(err == -1)
			{
				arg_list_destroy(stack->stk[stack->top-1]->data);
				return ERR_RUN_UNINIT_VAR;
			}
			*return_val = data_create(NULL, err, NULL); 
			arguments = stack->stk[stack->top-1]->data;
			err = func_call(*return_val, arguments, stack->stk[stack->top-3]->data); // provadi semanticke kontroly a generuje instrukce volani funkce
			if(err != ERR_NONE)
			{
				destroy_data(*return_val);
				arg_list_destroy(arguments);
				return err;
				
			}
			break;
		case 10: // P --> E,E
			arguments = arg_list_new();
			err = arg_add(arguments, stack->stk[stack->top-2]->data );
			if( err != ERR_NONE ){
				arg_list_destroy (arguments);
				return err;
			}
			
			err = arg_add(arguments, stack->stk[stack->top]->data );
			if( err != ERR_NONE ){
				arg_list_destroy (arguments);
				return err;
			}

			*return_val = arguments;
			break;
		case 11: // P --> P,E
			// pridat hodnotu E do seznamu argumentu 
			arguments = stack->stk[stack->top-2]->data;
			err = arg_add( arguments , stack->stk[stack->top]->data);
			if(err != ERR_NONE){
				arg_list_destroy(arguments);
				return err;
			}
			*return_val = arguments;
			break;

			// ---- COMPARSION ----
		case 12: // E>E
			err = type_comparison(stack->stk[stack->top-2]->data, stack->stk[stack->top]->data, GREATER);
			if(err != T_BOOL)
				return ERR_SEM_TYPE_COMP;
			else if ( (*return_val = data_create(NULL , T_BOOL, NULL)) == NULL )
					return ERR_INTERNAL;
			
			// - generovat odpovidajici instrukci

			break;
		case 13: // E<E
			err = type_comparison(stack->stk[stack->top-2]->data, stack->stk[stack->top]->data, LESSER);
			if(err != T_BOOL)
				return ERR_SEM_TYPE_COMP;
			else if ( (*return_val = data_create(NULL , T_BOOL, NULL)) == NULL )
					return ERR_INTERNAL;
			
			// - generovat odpovidajici instrukci
			break;
		case 14: // E>=E
			err = type_comparison(stack->stk[stack->top-2]->data, stack->stk[stack->top]->data, GRTEQL);
			if(err != T_BOOL)
				return ERR_SEM_TYPE_COMP;
			else if ( (*return_val = data_create(NULL , T_BOOL, NULL)) == NULL )
					return ERR_INTERNAL;
			
			// - generovat odpovidajici instrukci
		break;
		case 15: // E<=E
			err = type_comparison(stack->stk[stack->top-2]->data, stack->stk[stack->top]->data, LESEQL);
			if(err != T_BOOL)
				return ERR_SEM_TYPE_COMP;
			else if ( (*return_val = data_create(NULL , T_BOOL, NULL)) == NULL )
					return ERR_INTERNAL;
			
			// - generovat odpovidajici instrukci
			break;
		case 16: // E==E
			err = type_comparison(stack->stk[stack->top-2]->data, stack->stk[stack->top]->data, EQUAL);
			if(err != T_BOOL)
				return ERR_SEM_TYPE_COMP;
			else if ( (*return_val = data_create(NULL , T_BOOL, NULL)) == NULL )
					return ERR_INTERNAL;
			
			// - generovat odpovidajici instrukci
			break;
		case 17: //E!=E
			err = type_comparison(stack->stk[stack->top-2]->data, stack->stk[stack->top]->data, NOTEQL);
			if(err != T_BOOL)
				return ERR_SEM_TYPE_COMP;
			else if ( (*return_val = data_create(NULL , T_BOOL, NULL)) == NULL )
					return ERR_INTERNAL;
			
			// - generovat odpovidajici instrukci
			break; 

	}

	return ERR_NONE;
}




int parse_exp(token_t *token, ilist_t L, symb_table_t *ST, symb_table_t *STlocal, String *class)
{

	struct exp_needs pack;
	pack.STglobal = ST;
	pack.STlocal = STlocal;
	pack.classname = class;

	exp_stack_t *Estack = exp_stack_new();
	exp_item_t *nonterm;
	exp_item_t *item = exp_item_new();
	item->item = END;
	exp_SPush(Estack, item);

	/* stk - top of the stack 
	   in  - input */
	void *reference;
	prec_chars in, stk;
	char decide;
	int err, idx;
	first_go = 1;
	bool reduced = false; // after reduction we don't read_input again 
	
	while (42)
	{
		 print_exp(Estack);
		if (!reduced)
		{

		 err = read_input(token, Estack, &item, &pack);

			in = item->item; 
		}
		else reduced = false;

		if ((in == RPAR) || (in == COMMA))// might be end of expression, LL parser has to check 
				if (last_term(Estack) == END)
					in = END;  


		if (err != ERR_NONE)
		{
			exp_stack_del(Estack);
			return err; 
		}  

		stk = last_term(Estack);
		if (stk == PREC_ERR)  
		{
			exp_stack_del(Estack);
			return ERR_SYNTAX;
		}

		decide = prec_table[stk][in];
		switch (decide)
		{
			case '<':
				insert_handle(Estack);
				exp_SPush(Estack, item);
			break;
			case '>':
				idx = match_rule(Estack);
				if (idx != -1)
				{
					// call function to do semantic actions corresponding to rule idx
					err = do_exp_semantic(&reference, Estack, idx); // ___
					clean_rule(Estack);
					// doplnit do struktury record/args
					nonterm = exp_item_new();
					nonterm->item = prec_rules[idx][0];
					nonterm->data = reference;
					exp_SPush(Estack, nonterm);
	
					reduced = true;
					
				}
				else 
				{
					exp_stack_del(Estack);
					return ERR_SYNTAX;
				}
			break;
			case '=':
				exp_SPush(Estack, item);
			break;
			case 'e':
				if ((exp_STop(Estack))->item == EXP)
				{
					exp_stack_del(Estack);
					return ERR_NONE; // LL pohlida, ze ocekaval ; nebo ) 
				}
			default: 
				exp_stack_del(Estack);
				return ERR_SYNTAX;
		}
	
	}
	return ERR_NONE;
}
