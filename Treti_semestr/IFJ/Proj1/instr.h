/**
 * @author Radim Cervinka
 * @date 11/22/2016
 * @file instr.h
 * @brief structures for instructions
 * Copyright (c) 2016
 */

#ifndef INSTR_H /* This definition has to correspond to file name */
#define INSTR_H /* . */

#define I_HALT 0 // konec programu <I_HALT, _ , _ , _ >
#define I_JMP 1 // nepodmineny skok <I_JMP, _ , _ , adresa labelu>
#define I_JNC 2 // podmineny skok, skoci se na label, pokud cond == false <I_JNC, condition, _ , adresa labelu>
#define I_ADD 3 // scitani <I_ADD, prvni operand, druhy operand, vysledek>
#define I_SUB 4 // odcitani <I_SUB, mensenec, mensitel, vysledek>
#define I_MUL 5 // nasobeni <I_MUL, prvni operand, druhy operand, vysledek>
#define I_DIV 6 // deleni <I_DIV, delenec, delitel, vysledek>
#define I_ASSIGN 7 // prirazeni <I_ASSIGN, co priradit, _ , kam to pripradit>
#define I_MORE 8 // more than > I_MORE, prvni operand, druhy operand, vysledek>
#define I_LESS 9 // less then < <I_LESS, prvni operand, druhy operand, vysledek>
#define I_MEQ 10 // more then or equal ME >= <I_MEQ, prvni operand, druhy operand, vysledek>
#define I_LEQ 11 // less than or equal LE <= <I_LEQ, prvni operand, druhy operand, vysledek>
#define I_CMP 12 // porovnani <I_CMP, prvni operand, druhy operand, vysledek>
#define I_NCMP 13 // not equal <I_NCMP, prvni operand, druhy operand, vysledek>
#define I_NEG 14 // negace NEPOTREBNA???
#define I_LABEL 15 // label pro skoky <label, _ , _ , _ >
#define I_READ 16 // cteni ze vstupu <I_READ, _ , _ , kam to ulozit>
#define I_WRITE 17 // vypis na vystup <I_WRITE, retezec, _ , _ >

/**
 * @brief data structure for instruction
 */
typedef struct instr_s {
    int instrType; /**< Type of instruction */
    void *addr1;  /**< Address of 1st operand */
    void *addr2;  /**< Address of 2nd operand */
    void *addr3;  /**< Address of 3rd operand */
    struct instr_s *next; /**< Address of next instruction */
} instr_t;

#endif
