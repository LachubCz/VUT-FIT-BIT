/**
 * @author Pavel Cerny, Petra Buchtova 
 * @file pre_parser.c  
 * @brief Simply preprocesses source file, fills global symbol table, detects lex errors and some syntax and semantic errors   
 * Copyright (c) 2016
 */

#include "pre_parser.h"


void free_before_flee (token_t * input,  String * new_name, String * func_params, symb_table_t * classes_table)
{
	token_del(input);	

	string_del(new_name);
	string_del(func_params);

	symbtab_del(classes_table);
}

/**
 * @brief      Function goes through source file and fills global symbol table. Handles wrong declarations, redefinitions on a global scale and lex errors.   
 *
 * @param      ST     odkaz na tabulku symbolu
 *
 * @return     returns error code (ERR_NONE, ERR_LEX, ERR_SYNTAX or ERR_SEM_UNDEF)
 */
int pre_parse(symb_table_t *STglobal)
{	
	token_t * input = token_new(); // current token 

	int input_type; // type of a current token 
	int state = ST_START;
	int error;
	int new_type; 
	bool is_void = false;

	String * class_name = NULL; // stores name of a class we are currently in 
	String * id_name = NULL; 
	String * new_name = string_new(); 
	String * func_params = string_new();

	table_item_t * found_item;
	data_t * new_item; 
	symb_table_t * classes_table = symbtab_new(); // to store all classes (to easily check redefinition)

	


	while(42)
	{
		if ((error=token_get(input)) != ERR_NONE) 
		{
			if (error == END_FILE) // we got to the end of the source file 
			{
				
				// before leaving check all the requierements 
				// check if Main.run was defined 
				new_name = string_new();
				string_init_str(new_name, "Main.run");
				found_item = symbtab_search(STglobal, new_name);
				if (found_item == NULL) // Main.run doesn't exist 
				{
					free_before_flee (input, new_name, func_params, classes_table);
					return ERR_SEM_UNDEF;
				} 
				
				string_init_str(func_params, "vv");
				if (string_compare(found_item->data.params, func_params) == 0) // wrong signature of Main.run 
				{
					free_before_flee (input, new_name, func_params, classes_table);
					return ERR_SEM_UNDEF; 
				}

				// requirements ok 
				free_before_flee (input, new_name, func_params, classes_table);
				return ERR_NONE;
			}
			else
			{
				free_before_flee (input, new_name, func_params, classes_table);
				return error;
			} 
		}

		input_type = input->token_type; 
		
		switch (state)
		{
			case ST_START:
				if (input_type == TK_CLASS)
				{
					state = ST_EXPECT_CLASS_ID;
				}
				else 
				{
					free_before_flee (input, new_name, func_params, classes_table);
					return ERR_SYNTAX; 
				}
			break;
			
			case ST_EXPECT_CLASS_ID:
				if (input_type == TK_ID)
				{
					class_name = input->str; // remember actual class name 
					
					// check attempts to define class called ifj16 
					new_name = string_new();
					string_init_str(new_name, "ifj16");
					if (string_compare (new_name, class_name) == 1)
					{
						free_before_flee (input, new_name, func_params, classes_table);
						return ERR_SEM_UNDEF; 
					}

					// class id must be a simple identifier 
					if (is_fully_qual(class_name))
					{
						free_before_flee (input, new_name, func_params, classes_table);
						return ERR_SEMANTIC_OTHER;
					}

					// check attempts to redefine class
					found_item = symbtab_search(classes_table, class_name);
					if (found_item != NULL) // item already exists -> redefinition not allowed 
					{
						free_before_flee (input, new_name, func_params, classes_table);
						return ERR_SEM_UNDEF;
					}
					
					// requierements ok, save new class name to classes_table 
					new_item = data_create(class_name, -1, NULL);  
					symbtab_insert(classes_table, new_item);
					
					

					state = ST_WAIT_FOR_STATIC; 
				}
				else // nameless classes not allowed 
				{
					free_before_flee (input, new_name, func_params, classes_table);
					return ERR_SYNTAX; 
				}
			break;

			case ST_WAIT_FOR_STATIC: 
				if (input_type == TK_STATIC)
					state = ST_EXPECT_STATIC_TYPE;
				if (input_type == TK_CLASS)
					state = ST_EXPECT_CLASS_ID;
			break;

			case ST_EXPECT_STATIC_TYPE:
				if (input_type == TK_INT)
					string_add_char(func_params, 'i');
				else if (input_type == TK_DOUBLE)
					string_add_char(func_params, 'd');
				else if (input_type == TK_STRING)
					string_add_char(func_params, 's');
				else if (input_type == TK_VOID)
				{
					string_add_char(func_params, 'v');
					is_void = true;
				}
				else // definition needs a data type 
				{
					free_before_flee (input, new_name, func_params, classes_table);
					return ERR_SYNTAX;  
				}
				state = ST_EXPECT_STATIC_ID;
			break;

			case ST_EXPECT_STATIC_ID:
				if (input_type == TK_ID)
					id_name = input->str;
				else 
				{
					free_before_flee (input, new_name, func_params, classes_table);
					return ERR_SYNTAX; 
				}
				// id must be a simple identifier 
				if (is_fully_qual(id_name))
				{
					free_before_flee (input, new_name, func_params, classes_table);
					return ERR_SEMANTIC_OTHER; 
				}
				
				state = ST_EXPECT_RECOGNITION;
			break;

			case ST_EXPECT_RECOGNITION: // we need to recognize if identifier belongs to a variable or a function 
				if ((input_type == TK_ASSIGNMENT) || (input_type == TK_SEMICOLON)) // id belongs to a variable 
				{
					if (!is_void) // variable type should not be void 
					{
						// make new_name fully qualified 
						string_init_str(new_name, class_name->string);
						string_add_char(new_name, '.');
						string_add_str(new_name, id_name);   

						// check redefinition 
						found_item = symbtab_search(STglobal, new_name);
						if (found_item != NULL) // item already exists -> redefinition not allowed 
						{
							free_before_flee (input, new_name, func_params, classes_table);
							return ERR_SEM_UNDEF;
						}

						// create new item and insert it to the table 
						new_type = str_to_type(func_params, 0); // type of the var is saved in func_params  
						new_item = data_create(new_name, new_type, NULL);
						symbtab_insert(STglobal, new_item);

						 
						// delete not used anymores and create new ones 
						string_del(id_name); 
						id_name = NULL; 
						string_del(func_params);
						func_params = string_new(); 
						new_name = string_new();
						
						state = ST_WAIT_FOR_STATIC; // wait for another definition 
					}
					else // void var; means error 
					{
						free_before_flee (input, new_name, func_params, classes_table);
						return ERR_SYNTAX; 
					}
				}
				else if (input_type == TK_LRBRACKET) // id belongs to a function 
					state = ST_EXPECT_PARAM_TYPE_OR_END;
				else // not a correct syntax 
				{
					free_before_flee (input, new_name, func_params, classes_table);
					return ERR_SYNTAX; 
				}
			break;

			case ST_EXPECT_PARAM_TYPE_OR_END:
				switch (input_type)
				{
					case TK_RRBRACKET:
						string_add_char(func_params, 'v');
						state = ST_FUNC_SAVE;						
					break;
					case TK_INT:
						string_add_char(func_params, 'i');
						state = ST_EXPECT_PARAM_ID;
					break;
					case TK_DOUBLE: 
						string_add_char(func_params, 'd');
						state = ST_EXPECT_PARAM_ID;
					break;
					case TK_STRING:
						string_add_char(func_params, 's');
						state = ST_EXPECT_PARAM_ID;
					break;
					default: // not a proper data type 
						free_before_flee (input, new_name, func_params, classes_table);
						return ERR_SYNTAX; 
					break;
				}
			break;

			case ST_EXPECT_PARAM_ID:
				if (input_type == TK_ID)
				{
					state = ST_EXPECT_COMMA_OR_END;
					// id must be a simple identifier 
					if (is_fully_qual(id_name))  
					{
						free_before_flee (input, new_name, func_params, classes_table);
						return ERR_SEMANTIC_OTHER; 
					}

					string_del(input->str); // we don't care about the name of params 
				}
				else
				{
					free_before_flee (input, new_name, func_params, classes_table);
					return ERR_SYNTAX; 
				}
			break;

			case ST_EXPECT_COMMA_OR_END:
				if (input_type == TK_COMMA)
					state = ST_EXPECT_PARAM_TYPE_OR_END; // there are some other params 
				else if (input_type == TK_RRBRACKET) // no more params 
				{
					state = ST_FUNC_SAVE;
				}
				else
				{
					free_before_flee (input, new_name, func_params, classes_table);
					return ERR_SYNTAX; 
				}
			break;

			case ST_FUNC_SAVE:
				// make new_name fully qualified 
				string_init_str(new_name, class_name->string);
				string_add_char(new_name, '.');
				string_add_str(new_name, id_name);   
				
				// check redefinition  
				found_item = symbtab_search(STglobal, new_name);
				if (found_item != NULL) // item already exists -> redefinition not allowed 
				{
					free_before_flee (input, new_name, func_params, classes_table);
					return ERR_SEM_UNDEF;
				}
				
				// create new item and insert it to the table
				new_item = data_create(new_name, T_FUNCTION, NULL);
				new_item->params = func_params;
				symbtab_insert(STglobal, new_item);


				// delete not used anymores and create new ones
				string_del(id_name);
				id_name = NULL; 
				func_params = string_new();
				new_name = string_new();
				is_void = false;
									
				state = ST_WAIT_FOR_STATIC; // wait for another definition 
			
			break;
		}
	}
	
	free_before_flee (input, new_name, func_params, classes_table);
	return ERR_NONE; 
}