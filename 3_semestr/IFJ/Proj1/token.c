/**
 * @author Marek Dolezel 
 * @date 3_10_2016
 * @file token.c 
 * @brief functions for operations with token
 * Copyright (c) 2016
 */
#include "token.h"
#include "string.h"
#include "ret_code.h"
#include <stdlib.h>

token_t *token_new(void)
{
    token_t *token = (token_t *)malloc(sizeof(token_t));
   

    if (token == NULL)
       ; // handle error
    else
         token->token_type=TK_NONE;
    return token;
}
void token_del(token_t *token)
{
    free(token);
}

