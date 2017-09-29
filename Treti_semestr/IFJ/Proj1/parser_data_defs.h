 /**
  * @author Marek Dolezel
  * @date 10/2/2016
  * @file parser_data_defs.h
  * @brief Interface for syntax analyzer submodule
  * Copyright (c) 2016
  */

 #ifndef PARSER_DATA_DEFS_H /* This definition has to correspond to file name */
 #define PARSER_DATA_DEFS_H
  /* . */

/* Non-terminals */
 enum non_terminals_t {
    NT_CLASS_LIST = 42,
    NT_STATIC_LIST,
    NT_DEF,
    NT_TYPE_NVOID,
    NT_DEF_NVOID,
    NT_PARAM_LIST,
    NT_PARAMS,
    NT_FUNC_BODY,
    NT_COMP_STAT,
    NT_ID_STAT,
    NT_ASSIGN,
    NT_CALL,
    NT_ARG_LIST,
    NT_ARGS,
    NT_LOC_VARDEF,
    NT_EXP

};

#define TRAP_CLASSID 101

/* Debugging strings for tokens */
char *enum2str[] = {
    "TK_NONE", /* Useless now? */
  	"TK_ID",

  	"TK_IF",
  	"TK_ELSE",

  	"TK_WHILE",
  	"TK_FOR",
  	"TK_DO",

  	"TK_RETURN",
  	"TK_CONTINUE",
  	"TK_BREAK",

  	"TK_CLASS",
  	"TK_BOOLEAN",
  	"TK_DOUBLE",
  	"TK_DOUBLE_IMM",
  	"TK_INT",
  	"TK_INT_IMM",
  	"TK_STRING",
  	"TK_STRING_IMM",
  	"TK_VOID",
  	"TK_STATIC",

  	"TK_TRUE",
	  "TK_FALSE",

  	"TK_KEYWORD",

  	"TK_PLUS",
  	"TK_MINUS",
  	"TK_MULTI",
  	"TK_DIV",

  	"TK_DOT",
  	"TK_COMMA",
  	"TK_SEMICOLON",

  	"TK_ASSIGNMENT",

  	"TK_EQUAL",
  	"TK_NONEQUAL",
  	"TK_EQUALGREATER",
  	"TK_EQUALLESSER",
  	"TK_GREATER",
  	"TK_LESSER",

  	"TK_LRBRACKET",
  	"TK_RRBRACKET",
  	"TK_LCBRACKET",
  	"TK_RCBRACKET",
    "$",
    "NT_CLASS_LIST",
    "NT_STATIC_LIST",
    "NT_DEF",
    "NT_TYPE_NVOID",
    "NT_DEF_NVOID",
    "NT_PARAM_LIST",
    "NT_PARAMS",
    "NT_FUNC_BODY",
    "NT_COMP_STAT",
    "NT_ID_STAT",
    "NT_ASSIGN",
    "#NT_CALL#",
    "#NT_ARG_LIST#",
    "NT_ARGS",

    "NT_LOCAL_VARDEF",
    "NT_EXP"
};
/* First sheet in excel */

int T[X_MAX][CT_MAX] =  {
        /* n means TK_NONE
		 * W means TK_WHILE
         * R means TK_RETURN
         * C means TK_CLASS
         * D means TK_DOUBLE
         * I means TK_INT
         * S means TK_STRING
         * IM .... TK_INT_IMM
         * DM .... TK_DOUBLE_IMM
         * SM .... TK_STRING_IMM
         */
/* NONTERMINAL       n  id  if  el   W for do   R con brea    C bool  D   DM  I IM   S  SM voi stat  tru fal key   +   -  *   /   .   ,   ;     =  ==  !=  >=  <=  >   <   (   )   {   }  $ */
/* CLASS_LIST  */ {  0,  0,  0,  0,  0, 0,  0,  0,  0,  0,    1,  0,  0,  0,  0, 0,  0,  0,  0,  0,    0,  0,  0,  0,  0, 0,  0,  0,  0,  0,    0,  0,  0,  0,  0, 0,  0,  0,  0,  0,  0, 2 },
/* STATIC_LIST */ {  0,  0,  0,  0,  0, 0,  0,  0,  0,  0,    0,  0,  0,  0,  0, 0,  0,  0,  0,  3,    0,  0,  0,  0,  0, 0,  0,  0,  0,  0,    0,  0,  0,  0,  0, 0,  0,  0,  0,  0,  4, 0 },
/* DEF         */ {  0,  0,  0,  0,  0, 0,  0,  0,  0,  0,    0,  0,  5,  0,  5, 0,  5,  0,  6,  0,    0,  0,  0,  0,  0, 0,  0,  0,  0,  0,    0,  0,  0,  0,  0, 0,  0,  0,  0,  0,  0, 0 },
/* TYPE_NVOID  */ {  0,  0,  0,  0,  0, 0,  0,  0,  0,  0,    0,  0,  8,  0,  7, 0,  9,  0,  0,  0,    0,  0,  0,  0,  0, 0,  0,  0,  0,  0,    0,  0,  0,  0,  0, 0,  0,  0,  0,  0,  0, 0 },
/* DEF_NVOID   */ {  0,  0,  0,  0,  0, 0,  0,  0,  0,  0,    0,  0,  0,  0,  0, 0,  0,  0,  0,  0,    0,  0,  0,  0,  0, 0,  0,  0,  0, 12,   10,  0,  0,  0,  0, 0,  0, 11,  0,  0,  0, 0 },
/*---------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* PARAM_LIST  */ {  0,  0,  0,  0,  0, 0,  0,  0,  0,  0,    0,  0, 14,  0, 14, 0, 14,  0,  0,  0,    0,  0,  0,  0,  0, 0,  0,  0,  0,  0,    0,  0,  0,  0,  0, 0,  0,  0, 13,  0,  0, 0 },
/* PARAMS      */ {  0,  0,  0,  0,  0, 0,  0,  0,  0,  0,    0,  0,  0,  0,  0, 0,  0,  0,  0,  0,    0,  0,  0,  0,  0, 0,  0,  0, 15,  0,    0,  0,  0,  0,  0, 0,  0,  0, 16,  0,  0, 0 },
/* FUNC_BODY   */ {  0, 35, 18,  0, 18, 0,  0, 18,  0,  0,    0,  0, 17,  0, 17, 0, 17,  0,  0,  0,    0,  0,  0,  0,  0, 0,  0,  0,  0,  0,    0,  0,  0,  0,  0, 0,  0,  0,  0,  0, 19, 0 },
/* COMP_STAT   */ {  0, 23, 21,  0, 22, 0,  0, 20,  0,  0,    0,  0,  0,  0,  0, 0,  0,  0,  0,  0,    0,  0,  0,  0,  0, 0,  0,  0,  0,  0,    0,  0,  0,  0,  0, 0,  0,  0,  0,  0, 24, 0 },
/* ID_STAT     */ {  0,  0,  0,  0,  0, 0,  0,  0,  0,  0,    0,  0,  0,  0,  0, 0,  0,  0,  0,  0,    0,  0,  0,  0,  0, 0,  0,  0,  0,  0,   25,  0,  0,  0,  0, 0,  0, 26,  0,  0,  0, 0 },
/*---------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* ASSIGN      */ {  0,  0,  0,  0,  0, 0,  0,  0,  0,  0,    0,  0,  0,  0,  0, 0,  0,  0,  0,  0,    0,  0,  0,  0,  0, 0,  0,  0,  0,  0,   27,  0,  0,  0,  0, 0,  0,  0,  0,  0,  0, 0 },
/* CALL        */ {  0,  0,  0,  0,  0, 0,  0,  0,  0,  0,    0,  0,  0,  0,  0, 0,  0,  0,  0,  0,    0,  0,  0,  0,  0, 0,  0,  0,  0,  0,    0,  0,  0,  0,  0, 0,  0, 28,  0,  0,  0, 0 },
/* ARG_LIST    */ {  0, 30,  0,  0,  0, 0,  0,  0,  0,  0,    0,  0,  0, 30,  0,30,  0, 30,  0,  0,    0,  0,  0,  0,  0, 0,  0,  0,  0,  0,    0,  0,  0,  0,  0, 0,  0, 30, 29,  0,  0, 0 },
/* ARGS        */ {  0,  0,  0,  0,  0, 0,  0,  0,  0,  0,    0,  0,  0,  0,  0, 0,  0,  0,  0,  0,    0,  0,  0,  0,  0, 0,  0,  0, 31,  0,    0,  0,  0,  0,  0, 0,  0,  0, 32,  0,  0, 0 },
/* LOC_VARDEF  */ {  0,  0,  0,  0,  0, 0,  0,  0,  0,  0,    0,  0,  0,  0,  0, 0,  0,  0,  0,  0,    0,  0,  0,  0,  0, 0,  0,  0,  0, 34,   33,  0,  0,  0,  0, 0,  0,  0,  0,  0,  0, 0 },
/*---------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/* EXP         */ {  0,  0,  0,  0,  0, 0,  0,  0,  0,  0,    0,  0,  0,  0,  0, 0,  0,  0,  0,  0,    0,  0,  0,  0,  0, 0,  0,  0,  0,  0,    0,  0,  0,  0,  0, 0,  0,  0,  0,  0,  0, 0 } // zero line for EXP :D


    };
/* RULES_CERNI_funexp sheet in excel
   This array defines what has to be pushed onto the stack. */

int ReplaceArr[50][50] = {
    /* R00 */ { -1 },
    /* R01 */ { NT_CLASS_LIST, TK_RCBRACKET,NT_STATIC_LIST, TK_LCBRACKET, TK_ID, TRAP_CLASSID, TK_CLASS, -1},
    /* R02 */ { -1 },
    /* R03 */ { NT_STATIC_LIST, NT_DEF, TK_STATIC, -1 },
    /* R04 */ { -1 },
    /* R05 */ { NT_DEF_NVOID, TK_ID, NT_TYPE_NVOID, -1 },
    /* R06 */ { TK_RCBRACKET, NT_FUNC_BODY, TK_LCBRACKET, TK_RRBRACKET, NT_PARAM_LIST, TK_LRBRACKET, TK_ID, TK_VOID, -1 },
    /* R07 */ { TK_INT, -1},
    /* R08 */ { TK_DOUBLE, -1},
    /* R09 */ { TK_STRING, -1},
    /* R10 */ { NT_ASSIGN, -1},
    /* R11 */ { TK_RCBRACKET, NT_FUNC_BODY, TK_LCBRACKET, TK_RRBRACKET, NT_PARAM_LIST, TK_LRBRACKET, -1 },
    /* R12 */ { TK_SEMICOLON, -1 },
    /* R13 */ { -1 },
    /* R14 */ { NT_PARAMS, TK_ID, NT_TYPE_NVOID, -1 },
    /* R15 */ { NT_PARAMS, TK_ID, NT_TYPE_NVOID,TK_COMMA, -1},
    /* R16 */ { -1 },
    /* R17 */ { NT_FUNC_BODY, NT_LOC_VARDEF, TK_ID, NT_TYPE_NVOID, -1 },
    /* R18 */ { NT_FUNC_BODY, NT_COMP_STAT, -1 },
    /* R19 */ { -1 },
    /* R20 */ { TK_SEMICOLON, NT_EXP, TK_RETURN, -1 },
    /* R21 */ { TK_RCBRACKET, NT_COMP_STAT, TK_LCBRACKET, TK_ELSE, TK_RCBRACKET, NT_COMP_STAT, TK_LCBRACKET, TK_RRBRACKET, NT_EXP, TK_LRBRACKET, TK_IF, -1 },
    /* R22 */ { TK_RCBRACKET, NT_COMP_STAT, TK_LCBRACKET, TK_RRBRACKET, NT_EXP, TK_LRBRACKET, TK_WHILE, -1},
    /* R23 */ { NT_COMP_STAT, NT_ID_STAT, TK_ID, -1 },
    /* R24 */ { -1 },
    /* R25 */ { NT_ASSIGN, -1 },
    /* R26 */ { NT_CALL, -1},
    /* R27 */ { TK_SEMICOLON, NT_EXP, TK_ASSIGNMENT, -1 },
    /* R28 */ { TK_SEMICOLON, TK_RRBRACKET, NT_ARG_LIST, TK_LRBRACKET, -1 },
    /* R29 */ { -1 },
    /* R30 */ { NT_ARGS, NT_EXP,  -1 },
    /* R31 */ { NT_ARGS, NT_EXP, TK_COMMA, -1 },
    /* R32 */ { -1 },
    /* R33 */ { NT_ASSIGN, -1 },
    /* R34 */ { TK_SEMICOLON, -1},
    /* R35 */ { NT_FUNC_BODY, NT_ID_STAT, TK_ID, -1 }
};

/**
 * Array of terminals for function isTerminal()
 */
int terminals[] = {
  TK_NONE,  /* Useless now? */
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
    -1
};

#endif
