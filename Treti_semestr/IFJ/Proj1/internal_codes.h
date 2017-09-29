 /**
 * @author Marek Dolezel
 * @date 9_11_2016
 * @file internal_codes.h
 * @brief nadefinovane navratove kody chyb
 * Copyright (c) 2016
 */
 
#ifndef INTERNAL_CODES_H /* This definition has to correspond to file name */
#define INTERNAL_CODES_H /* . */

#define ERROR 1
#define SUCCESS 0
#define STR_LEN 16
#define STACK_SIZE 64

typedef enum {
    I_ERR_OK,
    I_ERR_MALLOC,
    I_ERR_REALLOC,
} internal_codes_t;

#endif
