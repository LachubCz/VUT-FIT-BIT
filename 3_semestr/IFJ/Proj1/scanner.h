 /**
 * @author Marek Dolezel 
 * @file scanner.h
 * @brief hlavickovy soubor pro scanner
 * Copyright (c) 2016
 */ 
#ifndef SCANNER_H /* This definition has to correspond to file name */
#define SCANNER_H /* . */

#include <stdio.h>
#include "token.h"

int token_get(token_t *token);
void file_set(FILE *f);

#endif
