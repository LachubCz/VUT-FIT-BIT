/**
 * @author Radim Červinka, Petr Buchal
 * @file scanner.c 
 * @brief obsahuje funkci pro nacteni tokenu ze souboru
 * Copyright (c) 2016
 */

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <limits.h>
#include <float.h>
#include <string.h> /* for strcmp */

#include "string.h"
#include "token.h"
#include "ret_code.h"

FILE *source;

void file_set(FILE *f)
{
    source=f;
}

/**
 * @brief      funkce naplni parametr aktualnim tokenem ze souboru 
 *
 * @param      token     ukazatel na strukturu tokenu, kterou ma funkce naplnit aktualnim tokenem ze souboru
 *
 * @return     vraci integerovou hodnotu podle toho, zda došlo k lexikální chybě nebo ne
 */
int token_get(token_t *token)
{
    static int c; // variable for carring last char
    int state = 0;
    static int first_go = 1;
    
    token->str = string_new();
    
    while(1)
    {
        // we ungetc in certain states, so we don't have to test anything here (and we don't have to deal with reading_flag in all situations)
            c = getc(source); // add file ptr
            if (c == EOF)
            { 
                if (first_go)
                {
                    switch(state)
					{
						case 2:
						case 4:
						case 5:
						case 7:
						case 9:
						case 10:
						case 12:
						case 18:
						case 19:
							return ERR_LEX;
							break;
						default:
							rewind(source);
                    		first_go = 0;
							return END_FILE;
							break;
					}
                }
                switch(state)
                {
                    case 2:
                    case 4:
                    case 5:
                    case 7:
                    case 9:
                    case 10:
                    case 12:
                    case 18:
                    case 19:
                        return ERR_LEX;
                        break;
                    default:
                        return END_FILE;
                        break;
                }
                
            }
               
                
        switch(state)
        {
        // pocatecni stav
        case 0:
            if (isspace(c))
            {
                break;
            }
            else if (isalpha(c) || c == '$' || c == '_')
            {
                string_add_char(token->str, (char)c); /* Store char to String */ 
                state = 1;
                break;
            }
            else if (c == '"')
            {
                state = 4;
                break;
            }
            else if (isdigit(c))
            {
                string_add_char(token->str, (char)c); /* Store char to String */ 
                state = 6;
                break;
            }
            else if (c == '/')
            {
                state = 16;
                break;
            }
            else if (c == '<')
            {
                state = 15;
                break;
            }
            else if (c == '>')
            {
                state = 14;
                break;
            }
            else if (c == '=')
            {
                state = 13;
                break;
            }
            else if (c == '!')
            {
                state = 12;
                break;
            }
            else
            {
                switch(c)
                {
                    case EOF:
                    case '+': // return certain token in all cases
                        token->token_type = TK_PLUS;
                        return ERR_NONE;
                    case '-':
                        token->token_type = TK_MINUS;
                        return ERR_NONE;
                    case '*':
                        token->token_type = TK_MULTI;
                        return ERR_NONE;
                    case ';':
                        token->token_type = TK_SEMICOLON;
                        return ERR_NONE;
                    case ',':
                        token->token_type = TK_COMMA;
                        return ERR_NONE;
                    case '(':
                        token->token_type = TK_LRBRACKET;
                        return ERR_NONE;
                    case ')':
                        token->token_type = TK_RRBRACKET;
                        return ERR_NONE;
                    case '{':
                        token->token_type = TK_LCBRACKET;
                        return ERR_NONE;
                    case '}':
                        token->token_type = TK_RCBRACKET;
                        return ERR_NONE;
                    default: // lex_error
                    return ERR_LEX;
                }
            }
        // identifikator
        case 1:
            if (isalnum(c) || c == '$' || c == '_')
            {                
                string_add_char(token->str, (char)c); /* Store char to String */ 
                break;
                
            }
            else if (c == '.')
            {
                string_add_char(token->str, (char)c); /* Store char to String */ 
                state = 2;
                break;
            }
            else
            {
                char *keywords_arr[] = {"if", "else", "while", "for", "do", "return", "continue", "break", "class",
                                        "boolean","String", "double", "int", "void", "static", "true", "false"};
                token_type_t keyword_t_arr[] = { TK_IF, TK_ELSE, TK_WHILE, TK_FOR,  TK_DO, TK_RETURN, TK_CONTINUE,
                TK_BREAK, TK_CLASS, TK_BOOLEAN, TK_STRING, TK_DOUBLE, TK_INT, TK_VOID, TK_STATIC, TK_TRUE, TK_FALSE };

                for (int i = 0; i<16; i++) 
                {
                    if(!strcmp(token->str->string, keywords_arr[i])) 
                    { 
                        token->token_type=keyword_t_arr[i]; 

                        if ( !(isspace(c)) )
                        {
                            ungetc(c,source);
                        }
                        return ERR_NONE; 
                    }
                }
              
                token->token_type = TK_ID; // recognizing id token               
                if ( !(isspace(c)) )
                {
                    ungetc(c,source);
                }
               return ERR_NONE;
            }
              
        // tecka v identifikatoru
        case 2:
            if (isalpha(c) || c == '$' || c == '_')
            {
                string_add_char(token->str, (char)c); /* Store char to String */ 
                state = 3;
                break;
            }
            else // lex_error handling
            {
                return ERR_LEX;
            }
        // plne kvalifikovany identifikator
        case 3:
            if (isalnum(c) || c == '$' || c == '_')
            {
                string_add_char(token->str, (char)c); /* Store char to String */ 
                break;
            }
            else //return token
            {
                token->token_type = TK_ID; // recognizing id token
                if ( !(isspace(c)) )
                {
                    ungetc(c,source);
                }
               return ERR_NONE;
            }
        // zacatek stringu (pripadne prazdny string)
        case 4:
            if (c == '"')
            {
                token->token_type = TK_STRING_IMM;

                return ERR_NONE;
            }
            else if (c == '\n')
            {
            	return ERR_LEX;
            }
            else if (c == '\\')
            {
                state = 20;
                break;
            }
            else
            {
                string_add_char(token->str, (char)c); /* Store char to String */

                state = 5;
                break;
            }
        // konec neprazdneho stringu
        case 5:
            if (c == '"')
            {
                token->token_type = TK_STRING_IMM; // recognizing string token
                return ERR_NONE;
            }
            else if (c == '\n')
            {
            	return ERR_LEX;
            }
            else if (c == '\\')
            {
                state = 20;
                break;
            }
            else
            {
                string_add_char(token->str, (char)c); /* Store char to String */
                break;
            }
        // cele cislo      
        case 6:
            if (isdigit(c))
            {
                string_add_char(token->str, (char)c); /* Store char to String */
                break;

                
            }
            else if (c == '.')
            {
                string_add_char(token->str, (char)c);
                state = 7;
                break;
            }
            else if (c == 'e' || c == 'E')
            {
                string_add_char(token->str, (char)c);
                state = 9;
                break;
            }
            else if (isalpha(c) || c == '$' || c == '_')
            {
                return ERR_LEX;
            }    
            else // return token
            {
                token->token_type = TK_INT_IMM;
                
                char *endptr;
                long int_cnt = strtol(token->str->string, &endptr, 10);

                string_del(token->str);


                if (int_cnt >= INT_MAX || int_cnt <= INT_MIN) //kontrola zda-li je cislo mensi nez int_max
                {
                    return ERR_LEX; // Is this lex error at all?
                }
                token->i = int_cnt;

                // token attr edit
                if ( !(isspace(c)) )
                {
                    ungetc(c,source);
                }
                   
                    return ERR_NONE;
            }
        // desetinna tecka
        case 7:
            if (isdigit(c))
            {
                string_add_char(token->str, (char)c); /* Store char to String */
                state = 8;
                break;
            }
            else
            {
               return ERR_LEX;
            }
        // desetinne cislo
        case 8:
            if (isdigit(c))
            {                
                string_add_char(token->str, (char)c); /* Store char to String */
                break;
            }
            else if (c == 'e' || c == 'E')
            {
                string_add_char(token->str, (char)c); /* Store char to String */
                state = 9;
                break;
            }
            else if (isalpha(c) || c == '$' || c == '_')
            {
                return ERR_LEX;
            }    
            else
            {
                token->token_type = TK_DOUBLE_IMM;
                // token attr edit
                if ( !(isspace(c)) )
                {
                    ungetc(c,source);
                }

                long double ld;
                char *endptr;
                ld = strtold(token->str->string, &endptr);

                string_del(token->str);

                if (ld>=DBL_MAX || ld<=DBL_MIN) {
                    return ERR_LEX;
                }
                token->d = ld;
                return ERR_NONE;
            }
        // cislo s exponentem
        case 9:
            if (isdigit(c))
            {
                string_add_char(token->str, (char)c); /* Store char to String */
                state = 11;
                break;
            }
            else if (c == '+' || c == '-')
            {
                string_add_char(token->str, (char)c); /* Store char to String */
                state = 10;
                break;
            }
            else
            {
                return ERR_LEX;
            }
        // cislo se znamenkem exponentu
        case 10:
            if (isdigit(c))
            {
                string_add_char(token->str, (char)c); /* Store char to String */
                state = 11;
                break;
            }
            else
            {
                return ERR_LEX;
            }
        // desetinne cislo s exponentem
        case 11:
            if (isdigit(c))
            {
                string_add_char(token->str, (char)c); /* Store char to String */
                break;
            }
            else if (isalpha(c) || c == '$' || c == '_')
            {
                return ERR_LEX;
            }    
            else
            {
                token->token_type = TK_DOUBLE_IMM;
                // token attr edit
                if ( !(isspace(c)) )
                {
                    ungetc(c,source);
                }

                long double ld;
                char *endptr;
                ld = strtold(token->str->string, &endptr);

                string_del(token->str);

                if (ld>=DBL_MAX || ld<=DBL_MIN) {
                    return ERR_LEX;
                }
                
                token->d = ld;
                return ERR_NONE;
            }
        // vykricnik
        case 12:
            if (c == '=')
            {
                token->token_type = TK_NONEQUAL; // recognizing != operator
               return ERR_NONE;
            }
            else
            {
                return ERR_LEX;
            }
        // 1x rovna se
        case 13:
            if (c == '=')
            {
                token->token_type = TK_EQUAL; // recognizing == operator
               return ERR_NONE;
            }
            else
            {
                token->token_type = TK_ASSIGNMENT; // recognizing = operator
                if ( !(isspace(c)) )
                {
                    ungetc(c,source);
                }
               return ERR_NONE;
            }
        // vetsi nez (>)
        case 14:
            if (c == '=')
            {
                token->token_type = TK_EQUALGREATER; // recognizing >= operator
               return ERR_NONE;
            }
            else
            {
                token->token_type = TK_GREATER; // recognizing > operator
                if ( !(isspace(c)) )
                {
                    ungetc(c,source);
                }
               return ERR_NONE;
            }
        // mensi nez (<)
        case 15:
            if (c == '=')
            {
                token->token_type = TK_EQUALLESSER; // recognizing <= operator
                return ERR_NONE;
            }
            else
            {
                token->token_type = TK_LESSER; // recognizing < operator
                if ( !(isspace(c)) )
                {
                    ungetc(c,source);
                }
                return ERR_NONE;
            }
        // lomitko
        case 16:
            if (c == '*')
            {
                state = 18;
                break;
            }
            else if (c == '/')
            {
                state = 17;
                break;
            }
            else
            {
                token->token_type = TK_DIV; // recognizing / operator
               if ( !(isspace(c)) )
               {
                    ungetc(c,source);
               }               
                return ERR_NONE;
            }
        // radkovy komentar
        case 17:
            if (c == '\n')
            {
                state = 0;
                break;
            }
            else
            {
                break;
            }
        // blokovy komentar
        case 18:
            if (c == '*')
            {
                state = 19;
                break;
            }
            else
            {
                break;
            }
        // konec blokoveho komentare
        case 19:
            if (c == '*')
            {
                break;
            }
            else if (c == '/')
            {
                state = 0;
                break;
            }
            else
            {
                state = 18;
                break;
            }
        case 20:
            if (isdigit(c))
            {   
                if((c - '0') > 3)
                    return ERR_LEX;
                string_add_char(token->str, (char)c);
                state = 21;
                break;
            }
            else if (c == 't' || c == 'n') 
            {
                state = 5;
                break;
            }
            return ERR_LEX;
        case 21:
            if (isdigit(c))
            {   
                if((c - '0') > 7)
                    return ERR_LEX;
                string_add_char(token->str, (char)c);
                state = 22;
                break;
            }
            else return ERR_LEX;
        case 22:
            if (isdigit(c))
            {
                 if((c - '0') > 7)
                    return ERR_LEX;
                string_add_char(token->str, (char)c);
                state = 5;

                c=fgetc(source);
                if ( (c-'0') >= 0 || (c-'0') <= 9 ) {
                    return ERR_LEX;
                }  
                ungetc(c, source);
                
                break;
            }
            else return ERR_LEX;
        }
    }
}
