	
/* c206.c **********************************************************}
{* Téma: Dvousměrně vázaný lineární seznam
**
**                   Návrh a referenční implementace: Bohuslav Křena, říjen 2001
**                            Přepracované do jazyka C: Martin Tuček, říjen 2004
**                                            Úpravy: Bohuslav Křena, říjen 2016
**
** Implementujte abstraktní datový typ dvousměrně vázaný lineární seznam.
** Užitečným obsahem prvku seznamu je hodnota typu int.
** Seznam bude jako datová abstrakce reprezentován proměnnou
** typu tDLList (DL znamená Double-Linked a slouží pro odlišení
** jmen konstant, typů a funkcí od jmen u jednosměrně vázaného lineárního
** seznamu). Definici konstant a typů naleznete v hlavičkovém souboru c206.h.
**
** Vaším úkolem je implementovat následující operace, které spolu
** s výše uvedenou datovou částí abstrakce tvoří abstraktní datový typ
** obousměrně vázaný lineární seznam:
**
**      DLInitList ...... inicializace seznamu před prvním použitím,
**      DLDisposeList ... zrušení všech prvků seznamu,
**      DLInsertFirst ... vložení prvku na začátek seznamu,
**      DLInsertLast .... vložení prvku na konec seznamu, 
**      DLFirst ......... nastavení aktivity na první prvek,
**      DLLast .......... nastavení aktivity na poslední prvek, 
**      DLCopyFirst ..... vrací hodnotu prvního prvku,
**      DLCopyLast ...... vrací hodnotu posledního prvku, 
**      DLDeleteFirst ... zruší první prvek seznamu,
**      DLDeleteLast .... zruší poslední prvek seznamu, 
**      DLPostDelete .... ruší prvek za aktivním prvkem,
**      DLPreDelete ..... ruší prvek před aktivním prvkem, 
**      DLPostInsert .... vloží nový prvek za aktivní prvek seznamu,
**      DLPreInsert ..... vloží nový prvek před aktivní prvek seznamu,
**      DLCopy .......... vrací hodnotu aktivního prvku,
**      DLActualize ..... přepíše obsah aktivního prvku novou hodnotou,
**      DLSucc .......... posune aktivitu na další prvek seznamu,
**      DLPred .......... posune aktivitu na předchozí prvek seznamu, 
**      DLActive ........ zjišťuje aktivitu seznamu.
**
** Při implementaci jednotlivých funkcí nevolejte žádnou z funkcí
** implementovaných v rámci tohoto příkladu, není-li u funkce
** explicitně uvedeno něco jiného.
**
** Nemusíte ošetřovat situaci, kdy místo legálního ukazatele na seznam 
** předá někdo jako parametr hodnotu NULL.
**
** Svou implementaci vhodně komentujte!
**
** Terminologická poznámka: Jazyk C nepoužívá pojem procedura.
** Proto zde používáme pojem funkce i pro operace, které by byly
** v algoritmickém jazyce Pascalovského typu implemenovány jako
** procedury (v jazyce C procedurám odpovídají funkce vracející typ void).
**/

#include "c206.h"

int solved;
int errflg;

void DLError() {
/*
** Vytiskne upozornění na to, že došlo k chybě.
** Tato funkce bude volána z některých dále implementovaných operací.
**/	
    printf ("*ERROR* The program has performed an illegal operation.\n");
    errflg = TRUE;             /* globální proměnná -- příznak ošetření chyby */
    return;
}

void DLInitList (tDLList *L) {
/*
** Provede inicializaci seznamu L před jeho prvním použitím (tzn. žádná
** z následujících funkcí nebude volána nad neinicializovaným seznamem).
** Tato inicializace se nikdy nebude provádět nad již inicializovaným
** seznamem, a proto tuto možnost neošetřujte. Vždy předpokládejte,
** že neinicializované proměnné mají nedefinovanou hodnotu.
**/
    
	L->First = NULL;
	L->Act = NULL;
	L->Last = NULL;
}

void DLDisposeList (tDLList *L) {
/*
** Zruší všechny prvky seznamu L a uvede seznam do stavu, v jakém
** se nacházel po inicializaci. Rušené prvky seznamu budou korektně
** uvolněny voláním operace free. 
**/
	if (L->First != NULL)  //kontrola prazdneho listu
	{
		tDLElemPtr saver = NULL;

		while (L->First->rptr != NULL)  //postupne prochazeni seznamu od prvniho prvku
		{
			saver = L->First;
			L->First = L->First->rptr;  //skok na dalsi prvek
			free(saver);
		}

		free(L->First);
		L->First = NULL;  //pocatecni inicializace
		L->Act = NULL;
		L->Last = NULL;
	}
}

void DLInsertFirst (tDLList *L, int val) {
/*
** Vloží nový prvek na začátek seznamu L.
** V případě, že není dostatek paměti pro nový prvek při operaci malloc,
** volá funkci DLError().
**/
	
	tDLElemPtr New = NULL;

    if((New = malloc(sizeof(struct tDLElem))) == NULL)  //alokace pameti pro strukturu
    {
    	DLError();
    }

    else
    {
	    if (L->First == NULL)  //pripad pokud je seznam prazdny
	    {
		    New->rptr = NULL;
		    New->lptr = NULL;
		    New->data = val;
		    L->First = New;
		    L->Last = New;
	    }

	    else  //ostatni pripady
	    {
		    New->rptr = L->First;
		    New->lptr = NULL;
		    New->data = val;
			L->First->lptr = New;
	    	L->First = New;
	    }
    }
}

void DLInsertLast(tDLList *L, int val) {
/*
** Vloží nový prvek na konec seznamu L (symetrická operace k DLInsertFirst).
** V případě, že není dostatek paměti pro nový prvek při operaci malloc,
** volá funkci DLError().
**/ 	
	
	tDLElemPtr New = NULL;

    if((New = malloc(sizeof(struct tDLElem))) == NULL)  //alokace pameti pro strukturu
    {
    	DLError();
    }

    else
    {
	    if (L->First == NULL)  //pripad pokud je seznam prazdny
	    {
		    New->rptr = NULL;
		    New->lptr = NULL;
		    New->data = val;
		    L->First = New;
		    L->Last = New;
	    }

	    else  //ostatni priklady
	    {
		    New->rptr = NULL;
		    New->lptr = L->Last;
		    New->data = val;
			L->Last->rptr = New;
	    	L->Last = New;
	    }
	}
}

void DLFirst (tDLList *L) {
/*
** Nastaví aktivitu na první prvek seznamu L.
** Funkci implementujte jako jediný příkaz (nepočítáme-li return),
** aniž byste testovali, zda je seznam L prázdný.
**/
	
	L->Act = L->First;  //nastaveni aktivity na prvni prvek
}

void DLLast (tDLList *L) {
/*
** Nastaví aktivitu na poslední prvek seznamu L.
** Funkci implementujte jako jediný příkaz (nepočítáme-li return),
** aniž byste testovali, zda je seznam L prázdný.
**/
	
	
	L->Act = L->Last;  //nastaveni aktivity na posledni prvek
}

void DLCopyFirst (tDLList *L, int *val) {
/*
** Prostřednictvím parametru val vrátí hodnotu prvního prvku seznamu L.
** Pokud je seznam L prázdný, volá funkci DLError().
**/

	if (L->First == NULL)
	{
		DLError();
	}

	else
	{
		*val = L->First->data;  //kopirovani hodnoty pres pointer
	}
}

void DLCopyLast (tDLList *L, int *val) {
/*
** Prostřednictvím parametru val vrátí hodnotu posledního prvku seznamu L.
** Pokud je seznam L prázdný, volá funkci DLError().
**/
	
	if (L->First == NULL)
	{
		DLError();
	}

	else
	{
		*val = L->Last->data;  //kopirovani hodnoty pres pointer
	}
}

void DLDeleteFirst (tDLList *L) {
/*
** Zruší první prvek seznamu L. Pokud byl první prvek aktivní, aktivita 
** se ztrácí. Pokud byl seznam L prázdný, nic se neděje.
**/
	
	if (L->First == NULL)  //osetreni prazdneho seznamu
	{
		;
	}

	else
	{
		if (L->First == L->Act)  //osetreni ztraty aktivity seznamu
		{
			L->Act = NULL;
		}

		tDLElemPtr saver = NULL;
		saver = L->First;

		if (L->First->rptr == NULL)  //prvni prvek je jediny prvek
		{
			saver = NULL;
			free(saver);
			L->First = NULL;
			L->Last = NULL;
		}

		else  //prvni prvek neni jediny prvek
		{
			L->First = L->First->rptr;
			L->First->lptr = NULL;
			saver = NULL;
			free(saver);
		}
	}
}	

void DLDeleteLast (tDLList *L) {
/*
** Zruší poslední prvek seznamu L. Pokud byl poslední prvek aktivní,
** aktivita seznamu se ztrácí. Pokud byl seznam L prázdný, nic se neděje.
**/ 
	
	if (L->First == NULL)  //osetreni prazdneho seznamu
	{
		;
	}

	else
	{
		if (L->Last == L->Act)  //osetreni ztraty aktivity seznamu
		{
			L->Act = NULL;
		}

		tDLElemPtr saver = NULL;
		saver = L->Last;

		if (L->Last->lptr == NULL)  //posledni prvek je jediny prvek
		{
			saver = NULL;
			free(saver);
			L->First = NULL;
			L->Last = NULL;
		}

		else  //posledni prvek neni jediny prvek
		{
			L->Last = L->Last->lptr;
			L->Last->rptr = NULL;
			saver = NULL;
			free(saver);
		}
	}
}

void DLPostDelete (tDLList *L) {
/*
** Zruší prvek seznamu L za aktivním prvkem.
** Pokud je seznam L neaktivní nebo pokud je aktivní prvek
** posledním prvkem seznamu, nic se neděje.
**/
	
	if ((L->Act == NULL) || (L->Act==L->Last))  //osetreni neaktivity seznamu a aktivity na poslednim prvku
	{
		;
	}

	else
	{
		tDLElemPtr saver = NULL;
		saver = L->Act->rptr;

		if (saver->rptr != NULL)  //osetreni zda-li prvek 2 mista za aktualnim neni posledni
		{
			L->Act->rptr = saver->rptr;
			free(saver);
			saver = NULL;
			saver = L->Act->rptr;
			saver->lptr = L->Act;
		}

		else  //normalni pripad
		{
			L->Act->rptr = NULL;
			free(saver);
			L->Last = L->Act;
		}
	}
}

void DLPreDelete (tDLList *L) {
/*
** Zruší prvek před aktivním prvkem seznamu L .
** Pokud je seznam L neaktivní nebo pokud je aktivní prvek
** prvním prvkem seznamu, nic se neděje.
**/
	
	if ((L->Act == NULL) || (L->Act==L->First))  //osetreni neaktivity seznamu a aktivity na prvnim prvku
	{
		;
	}

	else
	{
		tDLElemPtr saver = NULL;
		saver = L->Act->lptr;

		if (saver->lptr != NULL)  //osetreni zda-li prvek 2 mista pred aktualnim neni prvni
		{
			L->Act->lptr = saver->lptr;
			free(saver);
			saver = NULL;
			saver = L->Act->lptr;
			saver->rptr = L->Act;
		}

		else  //normalni pripad
		{
			L->Act->lptr = NULL;
			free(saver);
			L->First = L->Act;
		}
	}
}

void DLPostInsert (tDLList *L, int val) {
/*
** Vloží prvek za aktivní prvek seznamu L.
** Pokud nebyl seznam L aktivní, nic se neděje.
** V případě, že není dostatek paměti pro nový prvek při operaci malloc,
** volá funkci DLError().
**/
	
	tDLElemPtr New = NULL;

    if((New = malloc(sizeof(struct tDLElem))) == NULL)  //alokace pameti 
    {
    	DLError();
    }

    else
    {
	    if (L->Act == NULL)  //osetreni aktivity seznamu
	    {
	    	;
	    }

	    else
	    {
	    	if (L->Act == L->Last)  //pripad kdy je aktualni prvek zaroven posledni
			{
		    	New->data = val;
		    	New->rptr = NULL;
		    	New->lptr = L->Act;

		    	L->Act->rptr = New;
		    	L->Last = New;
			}

			else  //ostatni pripady
			{
		    	tDLElemPtr saver = NULL;
		    	saver = L->Act->rptr;

		    	New->data = val;
		    	New->rptr = L->Act->rptr;
		    	New->lptr = L->Act;

		    	L->Act->rptr = New;
		    	saver->lptr = New;
			}
	    }
    }
}

void DLPreInsert (tDLList *L, int val) {
/*
** Vloží prvek před aktivní prvek seznamu L.
** Pokud nebyl seznam L aktivní, nic se neděje.
** V případě, že není dostatek paměti pro nový prvek při operaci malloc,
** volá funkci DLError().
**/
	
	tDLElemPtr New = NULL;

    if((New = malloc(sizeof(struct tDLElem))) == NULL)  //alokace pameti
    {
    	DLError();
    }

    else
    {
	    if (L->Act == NULL)  //osetreni aktivity seznamu
	    {
	    	;
	    }

	    else
	    {
	    	if (L->Act == L->First)  //pripad kdy je aktualni prvek zaroven prvni
	    	{
		    	New->data = val;
		    	New->rptr = L->Act;
		    	New->lptr = NULL;

		    	L->Act->lptr = New;
		    	L->First = New;
	    	}

	    	else  //ostatni priklady
	    	{
		    	tDLElemPtr saver = NULL;
		    	saver = L->Act->lptr;
		    	
		    	New->data = val;
		    	New->lptr = L->Act->lptr;
		    	New->rptr = L->Act;

		    	L->Act->lptr = New;
		    	saver->rptr = New;
	    	}
	    }
    }
}

void DLCopy (tDLList *L, int *val) {
/*
** Prostřednictvím parametru val vrátí hodnotu aktivního prvku seznamu L.
** Pokud seznam L není aktivní, volá funkci DLError ().
**/
		
	if (L->Act == NULL)
	{
		DLError ();
	}

	else
	{
		*val = L->Act->data;  //kopirovani hodnoty struktury pres ukazatel
	}
}

void DLActualize (tDLList *L, int val) {
/*
** Přepíše obsah aktivního prvku seznamu L.
** Pokud seznam L není aktivní, nedělá nic.
**/
	
	if (L->Act == NULL)
	{
		;
	}

	else
	{
		L->Act->data = val;  //aktualizace hodnoty struktury
	}
}

void DLSucc (tDLList *L) {
/*
** Posune aktivitu na následující prvek seznamu L.
** Není-li seznam aktivní, nedělá nic.
** Všimněte si, že při aktivitě na posledním prvku se seznam stane neaktivním.
**/
	
	if (L->Act == NULL)
	{
		;
	}

	else
	{
		L->Act = L->Act->rptr;  //nastaveni aktivity seznamu na dalsi prvek
	}
}


void DLPred (tDLList *L) {
/*
** Posune aktivitu na předchozí prvek seznamu L.
** Není-li seznam aktivní, nedělá nic.
** Všimněte si, že při aktivitě na prvním prvku se seznam stane neaktivním.
**/

	if (L->Act == NULL)
	{
		;
	}

	else
	{
		L->Act = L->Act->lptr;  //nastaveni aktivity na predchozi prvek
	}
}

int DLActive (tDLList *L) {
/*
** Je-li seznam L aktivní, vrací nenulovou hodnotu, jinak vrací 0.
** Funkci je vhodné implementovat jedním příkazem return.
**/
	
	
	return ((L->Act == NULL) ? 0 : 1);  //test aktivity seznamu
}

/* Konec c206.c*/