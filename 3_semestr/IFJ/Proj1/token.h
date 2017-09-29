/**
 * @author Marek Dolezel
 * @date 10_01_2016
 * @file token.h
 * @brief header file for token.c
 * Copyright (c) 2016
 */

#ifndef TOKEN_H /* This definition has to correspond to file name */
#define TOKEN_H /* . */

#include <stdlib.h>

#include "string.h"

typedef enum {
	TK_NONE, /* Useless now? */
	TK_ID,

	TK_IF,
	TK_ELSE,

	TK_WHILE,
	TK_FOR,
	TK_DO,

	TK_RETURN,
	TK_CONTINUE,
	TK_BREAK,

	TK_CLASS,
	TK_BOOLEAN,
	TK_DOUBLE,
	TK_DOUBLE_IMM,
	TK_INT,
	TK_INT_IMM,
	TK_STRING,
	TK_STRING_IMM,
	TK_VOID,
	TK_STATIC,

	TK_TRUE,
	TK_FALSE,

	TK_KEYWORD,

	TK_PLUS,
	TK_MINUS,
	TK_MULTI,
	TK_DIV,

	TK_DOT,
	TK_COMMA,
	TK_SEMICOLON,

	TK_ASSIGNMENT,

	TK_EQUAL,
	TK_NONEQUAL,
	TK_EQUALGREATER,
	TK_EQUALLESSER,
	TK_GREATER,
	TK_LESSER,

	TK_LRBRACKET,
	TK_RRBRACKET,
	TK_LCBRACKET,
	TK_RCBRACKET,
	/* TODO: Add more tokens... */
} token_type_t;

/* Token data structure */
typedef struct token_s {
	token_type_t token_type;
	 union  { /* Attribute */
		double d;
		int i;
		String *str;
	};
} token_t;

/*
 * @brief Create new token
 */
token_t *token_new(void);

/*
 * @brief Destroy token
 * @param pointer to token to be destroyed
 */
void token_del(token_t *token);


#endif
