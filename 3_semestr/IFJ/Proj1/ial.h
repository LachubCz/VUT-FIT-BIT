/**
 * @author Petr Buchal
 * @date 20_11_2016
 * @file ial.h 
 * @brief hlavickovi soubor k ial.c
 * Copyright (c) 2016
 */
 
#ifndef IAL_H /* This definition has to correspond to file name */
#define IAL_H /* . */

//vestavene funkce pro nacitani literalu a vypis termu
char *readString();
int readInt();
double readDouble();
//void print_var(String *s, symb_table_t *T);
void print();

//vestavene funkce peo praci s retezci
int lenght(char *s);
char *substr(char *s, int i, int n);
int compare(char *s1, char *s2);
void ComputeJumps(char *p, int CharJump[256]);  //pomocna funkce pro find
int find(char *s, char *search);
void swap (char *one, char *two);  //pomocna pro partition
void partition(char *s, int left, int right, int *i, int *j);  //pomocna funkce pro quicksort
char *quicksort (char *s, int left, int right);  //implementace sortu - rekurzivni quicksort
char *sort (char *s);

symb_table_t *symbtab_new();
bool symbtab_insert(symb_table_t *T, data_t *var);
table_item_t *symbtab_search(symb_table_t *T, String *s);
bool symbtab_del_sec(table_item_t *fptr);
bool symbtab_del(symb_table_t *T);
unsigned long hashfunction(data_t *var);
data_t *data_create(String *s, int type, instr_t *f_start);
void destroy_data(data_t *var);
void define_var(data_t *var);

#endif
