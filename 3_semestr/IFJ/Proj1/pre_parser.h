/**
 * @author Petra Buchtova
 * @date 12/06/2016
 * @file pre_parser.h
 * @brief Header file for pre_parser implementation
 * Copyright (c) 2016
 */

#ifndef PRE_PARSER_H /* This definition has to correspond to file name */
#define PRE_PARSER_H /* . */

#include "scanner.h"
#include "symbol_table.h"
#include "ret_code.h"
#include "string.h"
#include "semantic.h"

enum {
	ST_START,
	ST_EXPECT_CLASS_ID,
	ST_WAIT_FOR_STATIC,
	ST_EXPECT_STATIC_TYPE,
	ST_EXPECT_STATIC_ID,
	ST_EXPECT_RECOGNITION,
	ST_EXPECT_PARAM_TYPE_OR_END,
	ST_EXPECT_PARAM_ID,
	ST_EXPECT_COMMA_OR_END,
	ST_FUNC_SAVE
};
/**
* @brief Entry point to pre_parser
* @param STglobal pointer to  global symbol table
*/
int pre_parse(symb_table_t *STglobal);
#endif
