/**
 * @author Pavel Cerny, Petra Buchtova
 * @date 25_11_2016
 * @file exp_data_defs.h
 * @brief definice pravidel pro analyzator vyrazu
 * Copyright (c) 2016
 */
/*
 This will must be included by exp.c 
 Inclusion of this file in other module will cause linker to fail the process of linking .o files.
 Therefore executable will not be generated.__a
 
 WARNING!!

 This file contains variable definitions! (such as int x =3)

 WARNING!!
 */

 typedef enum {
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
	EOR, // end of rule                  (== 20)
	PREC_ERR //                          (== 21)

} prec_chars; 

char *enum2prec_chars[] = {
    "MUL", // *                            (== 0)
	"DIV", // /
	"PLUS", // +
	"MINUS", // -
	"LPAR", // (
	"RPAR", // )
	"ID",  // var id
	"LIT", // value
	"FUNC", // function
	"COMMA", // ,
	"GREATER", // >
	"LESSER", // <
	"GRTEQL", // >=
	"LESEQL", // <=
	"EQUAL", // ==
	"NOTEQL", // !=
	"END", // $                            (== 16)
	
	// nonterminals 
	"EXP", // E - expression               (== 17)
	"ARG", // P - list of func arguments   (== 18)

	// specials 
	"HDL_START", // handle start           (== 19)
	"EOR", // end of rule                  (== 20)
	"PREC_ERR" //        
    };

prec_chars prec_rules[RULES][RULE_MAX_LENGTH] = { 
// { left side (always at idx 0), per partes right side backwards, end of rule }
	{ EXP , EXP, MUL, EXP, EOR },
	{ EXP , EXP, DIV, EXP, EOR },
	{ EXP , EXP, PLUS, EXP, EOR },
	{ EXP , EXP, MINUS, EXP, EOR },
	{ EXP , RPAR, EXP, LPAR, EOR },
	{ EXP , ID, EOR },
	{ EXP , LIT, EOR },
	{ EXP , RPAR, LPAR, FUNC, EOR },
	{ EXP , RPAR, EXP, LPAR, FUNC, EOR },
	{ EXP , RPAR, ARG, LPAR, FUNC, EOR },
	{ ARG , EXP, COMMA, EXP, EOR },
	{ ARG , EXP, COMMA, ARG, EOR },
	{ EXP , EXP, GREATER, EXP, EOR },
	{ EXP , EXP, LESSER, EXP, EOR },
	{ EXP , EXP, GRTEQL, EXP, EOR },
	{ EXP , EXP, LESEQL, EXP, EOR },
	{ EXP , EXP, EQUAL, EXP, EOR },
	{ EXP , EXP, NOTEQL, EXP, EOR }
};

char prec_table[TERMS][TERMS] = { 
/*          *    /    +    -    (    )    i   lit   f    ,    >    <   >=   <=   ==   !=    $ */  
/*  *  */ {'>', '>', '>', '>', '<', '>', '<', '<', '<', '>', '>', '>', '>', '>', '>', '>', '>'},
/*  /  */ {'>', '>', '>', '>', '<', '>', '<', '<', '<', '>', '>', '>', '>', '>', '>', '>', '>'}, 

/*  +  */ {'<', '<', '>', '>', '<', '>', '<', '<', '<', '>', '>', '>', '>', '>', '>', '>', '>'},
/*  -  */ {'<', '<', '>', '>', '<', '>', '<', '<', '<', '>', '>', '>', '>', '>', '>', '>', '>'},

/*  (  */ {'<', '<', '<', '<', '<', '=', '<', '<', '<', '<', '<', '<', '<', '<', '<', '<', ' '},	
/*  )  */ {'>', '>', '>', '>', ' ', '>', ' ', ' ', ' ', '>', '>', '>', '>', '>', '>', '>', '>'},

/*  i  */ {'>', '>', '>', '>', ' ', '>', ' ', ' ', ' ', '>', '>', '>', '>', '>', '>', '>', '>'},
/* lit */ {'>', '>', '>', '>', ' ', '>', ' ', ' ', ' ', '>', '>', '>', '>', '>', '>', '>', '>'},

/*  f  */ {' ', ' ', ' ', ' ', '=', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
/*  ,  */ {'<', '<', '<', '<', '<', '>', '<', '<', '<', '>', '<', '<', '<', '<', '<', '<', ' '},

/*  >  */ {'<', '<', '<', '<', '<', '>', '<', '<', '<', '>', ' ', ' ', ' ', ' ', ' ', ' ', '>'},
/*  <  */ {'<', '<', '<', '<', '<', '>', '<', '<', '<', '>', ' ', ' ', ' ', ' ', ' ', ' ', '>'},

/* >=  */ {'<', '<', '<', '<', '<', '>', '<', '<', '<', '>', ' ', ' ', ' ', ' ', ' ', ' ', '>'},
/* <=  */ {'<', '<', '<', '<', '<', '>', '<', '<', '<', '>', ' ', ' ', ' ', ' ', ' ', ' ', '>'},

/* ==  */ {'<', '<', '<', '<', '<', '>', '<', '<', '<', '>', ' ', ' ', ' ', ' ', ' ', ' ', '>'},
/* !=  */ {'<', '<', '<', '<', '<', '>', '<', '<', '<', '>', ' ', ' ', ' ', ' ', ' ', ' ', '>'},

/*  $  */ {'<', '<', '<', '<', '<', ' ', '<', '<', '<', ' ', '<', '<', '<', '<', '<', '<', 'e'}	
};
// 'e' means end
 
