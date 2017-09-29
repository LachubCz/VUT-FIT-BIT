/**
 * @author Pavel Cerny
 * @date 1_12_2016
 * @file semantic.c
 * @brief modul for semantic analysis functions
 * Copyright (c) 2016
 */
 
 
#include<stdio.h>
#include "symbol_table.h"
#include "semantic.h"

enum {
	// terminals 
	MUL, // *                            (== 0)
	DIV, // /
	PLUS, // +
	MINUS, // -
	LPAR, // (
	RPAR, // )
	ID,  // var id
	LIT, // value
	FUNC, // function
	COMMA, // ,
	GREATER, // >
	LESSER, // <
	GRTEQL, // >=
	LESEQL, // <=
	EQUAL, // ==
	NOTEQL, // !=
	END, // $                            (== 16)
	
	// nonterminals 
	EXP, // E - expression               (== 17)
	ARG, // P - list of func arguments   (== 18)

	// specials 
	HDL_START, // handle start           (== 19)
	EOR // end of rule                  (== 20)
//	PREC_ERR   (== 21) hidden to avoid multiple definition error

} ; 

// funkce porovna datove typy dvou promennych
int type_comparison(data_t *add1, data_t *add2, int operation)
{
	switch(operation)
	{
		case PLUS : // scitani
		case MINUS : // odecitani
		case MUL : // nasobeni
		case DIV : // deleni
			if((add1->type == add2->type) && (add1->type == T_INT)) return T_INT;
			if((add1->type == T_DOUBLE) || (add2->type == T_DOUBLE)) return T_DOUBLE;
			   else return -1;   
			break;
		case GREATER : // more than >
		case LESSER : // less than <
		case GRTEQL : // more then or equal ME >=
		case LESEQL : // less than or equal LE <=
			if (((add1->type == T_INT) || (add1->type == T_DOUBLE)) && ((add2->type == T_INT) || (add2->type == T_DOUBLE)))
			return T_BOOL;
			else
			return -1;
			    
			break;
		
		case EQUAL : // porovnani			
		case NOTEQL : // negativni porovnani
			return T_BOOL;
			break;
		default: return -1;
			
	}
	return -1;
}
/* ----- USELESS ------
bool define_control (stack_symbol_table_t *frames, symb_table_t *global)
{
	int top = frames->top;

	for (frames->top; frames->top != -1; frames->top--)
	{
		if (symbtab_control(frames->stk[frames->top]) == false)
			return false;
	}

	if (symbtab_control(global) == false)
		return false;

	frames->top = top;
	return true;
}

bool symbtab_control_sec(table_item_t *fptr)
{
	if (fptr != NULL)
	{
		if(symbtab_control_sec (fptr->LPtr) == false)
			return false;
		if(symbtab_control_sec (fptr->RPtr) == false)
			return false;
		if (fptr->data.defined == false)
		{
			return false;
		}
		else
		{
			return true;
		}
	}
	return true;
}

bool symbtab_control(symb_table_t *T)
{
	if ((symbtab_control_sec (T->first)) == true)
	{
		return true;
	}
	else
	{
		return false;
	}
}
* ---- USELESS ----- */

int get_func_ret_type(data_t *function)
{
	switch(*function->params->string)
	{
		case 'i' : return T_INT;
		case 'd' : return T_DOUBLE;
		case 's' : return T_STRING; 
		default : return -1;
	}
	return -1;
}

/*
err = func_call(, ,); // provadi semanticke kontroly a generuje instrukce volani funkce
			^-- kontroluje pocet a typy argumentu  
*/
int func_call(data_t *result, struct arg_list *arguments, data_t *function)
{
	return ERR_NONE;
}


// checks wheter is identifier fully qualified or not 
bool is_fully_qual(String * id_name)
{
	int i = 0;
	while (i < id_name->length)
	{
		if (id_name->string[i] == '.') 
			return true;
		i++;
	}
	return false;
}

// gets func_parameters (such as 'visd') and transforms char at idx it to enum 
// get_func_ret_type ? :D 
int str_to_type(String * params, int idx)
{
	if (idx >= params->length)
		return -1;
	
	switch (params->string[idx])
	{
		case 'v': return T_VOID;
		case 'i': return T_INT;
		case 's': return T_STRING;
		case 'd': return T_DOUBLE;
		default: return -1;
	} 
}
