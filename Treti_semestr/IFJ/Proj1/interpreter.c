/**
 * @author Radim Cervinka
 * @date 8_12_2016
 * @file interpreter.c
 * @brief implementace intepretacni funkce
 * Copyright (c) 2016
 */

#include <stdio.h>
#include <stdbool.h>
#include "ret_code.h"
#include "symbol_table.h"
#include "ilist.h"
#include "instr.h"
#include "interpreter.h"
#include "semantic.c"
#include "semantic.h"
int interpret(symb_table_t *T, ilist_t ilist)
{
  //ilist_first(ilist);  // aktivita na prvni polozce seznamu
  instr_t *I;
  while (1)
  {
    I = ilist_data_get(ilist);

    switch (I->instrType)
    {
	case I_HALT:
	// konec programu
		return ERR_NONE;
	break;
		    
	case I_JMP:
	// nepodmineny skok
	break;

	case I_JNC:
	// jump condition (podmineny skok)
	break;

	case I_ADD:
	// scitani

		*(I->addr3->value) = (*(I->addr1->value)) + (*(I->addr2->value));
		break;

	case I_SUB:
	// odcitani
		*(I->addr3->value) = (*(I->addr1->value)) - (*(I->addr2->value));
		break;

	case I_MUL:
	// nasobeni
		*(I->addr3->value) = (*(I->addr1->value)) * (*(I->addr2->value));					
		break;

	case I_DIV:
	// deleni
		*(I->addr3->value) = (*(I->addr1->value)) / (*(I->addr2->value));
		break;

	case I_ASS:
	// prirazeni
		*(I->addr3->value) = *(I->addr1->value);   
		break;
	
	case I_MORE:
	// more than >
		if ((*(I->addr1->value)) > (*(I->addr2->value)))
			*(I->addr3->value) = true;
		else
			*(I->addr3->value) = false;
		break;

	case I_LESS:
	// less then <
		if ((*(I->addr1->value)) < (*(I->addr2->value)))
			*(I->addr3->value) = true;
		else
			*(I->addr3->value) = false;
		break;

	case I_MEQ:
	// more then or equal ME >=
		if ((*(I->addr1->value)) >= (*(I->addr2->value)))
			*(I->addr3->value) = true;
		else
			*(I->addr3->value) = false;
		break;

	case I_LEQ:
	// less than or equal LE <=
		if ((*(I->addr1->value)) <= (*(I->addr2->value)))
			*(I->addr3->value) = true;
		else
			*(I->addr3->value) = false;
		break;

	case I_CMP:
	// porovnani
		if ((*(I->addr1->value)) == (*(I->addr2->value)))
			*(I->addr3->value) = true;
		else
			*(I->addr3->value) = false;
		break;

	case I_NCMP:
	// not equal

		if ((*(I->addr1->value)) == (*(I->addr2->value)))
			*(I->addr3->value) = false;
		else
			*(I->addr3->value) = true;
		break;

/*	case I_NEG:
	// negace
		break;
*/
	case I_LABEL:
	// label pro skoky
		break;

	case I_READ:
	// cteni ze vstupu
		break;

	case I_WRITE:
	// vypis na vystup
		break;

    }

    // prejdeme na dalsi instrukci
    // POZOR! Pokud byl proveden skok, nemelo by se posouvat na dalsi instrukci!
    // Protoze ale vime, ze skok mohl byt proveden pouze na navesti a v ramci
    // navesti se nic neprovadi, muzeme si dovolit prejit na dalsi instrukci i
    // v tomto pripade, pouze tim urychlime beh programu
    ilist_next(ilist);

  }
}

