 /**
 * @author Marek Dolezel
 * @date 4_12_2016
 * @file main.c
 * @brief obsahuje implementaci hlavniho rizeni interpretu
 * Copyright (c) 2016
 */
#include <stdio.h>

#include "scanner.h"
#include "ret_code.h"
#include "pre_parser.h"
#include "parser.h"



#include "ilist.h"
#include "symbol_table.h"
#include "stack_symbol_table.h"

void clean_up(ilist_t L, symb_table_t *ST, stack_symbol_table_t *frames) 
{
    ilist_free(L);
    //symbtab_del(ST);
    //stack_del_st(frames);
}
int main(int argc, char **argv)
{

    if (argc == 1) 
        return ERR_INTERNAL;

    FILE *f;
    f = fopen(argv[1], "r");

    if (f == NULL) {
        fclose(f);
        return ERR_INTERNAL;
    }
       
    ilist_t L = ilist_new();
    symb_table_t *ST = symbtab_new();
    stack_symbol_table_t *frames = stack_new_st();

    



    file_set(f); //open file in scanner

    int ret_code = pre_parse(ST);

    if (ret_code != ERR_NONE) {
        clean_up(L,ST,frames);
        return ret_code;
    }

    //TODO: pro parser musi byt symbol_table a instruction list
    ret_code = parser_entry(L, ST, frames);
    
    if (ret_code != ERR_NONE) {
        clean_up(L,ST,frames);
        return ret_code;
    }
    
    //ret_code = interpreter_entry(); //bere symbol_table a instruction list
      //  return ret_code;


}
