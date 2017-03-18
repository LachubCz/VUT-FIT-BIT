
/* c201.c *********************************************************************}
{* Téma: Jednosměrný lineární seznam
**
**                     Návrh a referenční implementace: Petr Přikryl, říjen 1994
**                                          Úpravy: Andrea Němcová listopad 1996
**                                                   Petr Přikryl, listopad 1997
**                                Přepracované zadání: Petr Přikryl, březen 1998
**                                  Přepis do jazyka C: Martin Tuček, říjen 2004
**                                            Úpravy: Bohuslav Křena, říjen 2016
**
** Implementujte abstraktní datový typ jednosměrný lineární seznam.
** Užitečným obsahem prvku seznamu je celé číslo typu int.
** Seznam bude jako datová abstrakce reprezentován proměnnou typu tList.
** Definici konstant a typů naleznete v hlavičkovém souboru c201.h.
** 
** Vaším úkolem je implementovat následující operace, které spolu s výše
** uvedenou datovou částí abstrakce tvoří abstraktní datový typ tList:
**
**      InitList ...... inicializace seznamu před prvním použitím,
**      DisposeList ... zrušení všech prvků seznamu,
**      InsertFirst ... vložení prvku na začátek seznamu,
**      First ......... nastavení aktivity na první prvek,
**      CopyFirst ..... vrací hodnotu prvního prvku,
**      DeleteFirst ... zruší první prvek seznamu,
**      PostDelete .... ruší prvek za aktivním prvkem,
**      PostInsert .... vloží nový prvek za aktivní prvek seznamu,
**      Copy .......... vrací hodnotu aktivního prvku,
**      Actualize ..... přepíše obsah aktivního prvku novou hodnotou,
**      Succ .......... posune aktivitu na další prvek seznamu,
**      Active ........ zjišťuje aktivitu seznamu.
**
** Při implementaci funkcí nevolejte žádnou z funkcí implementovaných v rámci
** tohoto příkladu, není-li u dané funkce explicitně uvedeno něco jiného.
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

#include "c201.h"

int solved;
int errflg;

void Error() {
/*
** Vytiskne upozornění na to, že došlo k chybě.
** Tato funkce bude volána z některých dále implementovaných operací.
**/	
    printf ("*ERROR* The program has performed an illegal operation.\n");
    errflg = TRUE;                      /* globální proměnná -- příznak chyby */
}

void InitList (tList *L) {
/*
** Provede inicializaci seznamu L před jeho prvním použitím (tzn. žádná
** z následujících funkcí nebude volána nad neinicializovaným seznamem).
** Tato inicializace se nikdy nebude provádět nad již inicializovaným
** seznamem, a proto tuto možnost neošetřujte. Vždy předpokládejte,
** že neinicializované proměnné mají nedefinovanou hodnotu.
**/
	
	L->Act = NULL;
	L->First = NULL;
}

void DisposeList (tList *L) {
/*
** Zruší všechny prvky seznamu L a uvede seznam L do stavu, v jakém se nacházel
** po inicializaci. Veškerá paměť používaná prvky seznamu L bude korektně
** uvolněna voláním operace free.
***/

	if (L->First != NULL)  //kontrola prazdneho seznamu
	{
		tElemPtr saver = NULL;
		L->Act = L->First;  //aktualni prvek je nastaveny na prvi

		while (L->Act->ptr != NULL)  //seznam se postupne prochazi a prvky se uvolnuji pomoci fce free()
		{
			saver = L->Act;
			L->Act = L->Act->ptr;  //aktualni prvek se posouva na dalsi
			free(saver);
		}

		free(L->Act);  //uvolneni posledniho prvku
		L->Act = NULL;
		L->First = NULL;
	}
}

void InsertFirst (tList *L, int val) {
/*
** Vloží prvek s hodnotou val na začátek seznamu L.
** V případě, že není dostatek paměti pro nový prvek při operaci malloc,
** volá funkci Error().
**/

	tElemPtr New = NULL;

    if((New = malloc(sizeof(struct tElem))) == NULL)  //alokace pameti
    {
    	Error();
    }

    else
    {
	    New->data = val; //predani hodnoty
	    New->ptr = L->First;  //nastaveni odkazu na stary prvni prvek
	    L->First = New;  //nastaveni noveho prvniho prvku
    }
}

void First (tList *L) {
/*
** Nastaví aktivitu seznamu L na jeho první prvek.
** Funkci implementujte jako jediný příkaz, aniž byste testovali,
** zda je seznam L prázdný.
**/
	
	L->Act = L->First;
}

void CopyFirst (tList *L, int *val) {
/*
** Prostřednictvím parametru val vrátí hodnotu prvního prvku seznamu L.
** Pokud je seznam L prázdný, volá funkci Error().
**/

	if (L->First == NULL)
	{
		Error();
	}

	else
	{
		*val = L->First->data;  //nahrani hodnoty prvku do ukazatele
	}
}

void DeleteFirst (tList *L) {
/*
** Zruší první prvek seznamu L a uvolní jím používanou paměť.
** Pokud byl rušený prvek aktivní, aktivita seznamu se ztrácí.
** Pokud byl seznam L prázdný, nic se neděje.
**/
	
	if (L->First != NULL)  //kontrola prazdnoty seznamu
	{
		tElemPtr saver = NULL;

		if (L->Act == L->First)  //varianta kdyz se aktivita seznamu ztraci
		{
			saver = L->First;
			L->First = L->First->ptr;
			free(saver);
			L->Act = NULL;
		}

		else  //varianta kdyz seznam zustava aktivni
		{
			saver = L->First;
			L->First = L->First->ptr;
			free(saver);
		}
	}
}	

void PostDelete (tList *L) {
/* 
** Zruší prvek seznamu L za aktivním prvkem a uvolní jím používanou paměť.
** Pokud není seznam L aktivní nebo pokud je aktivní poslední prvek seznamu L,
** nic se neděje.
**/

	if ((L->First == NULL) || (L->Act->ptr == NULL))  //kontrola podminek pri kterych se nic nedeje
	{
		;
	}

	else
	{
		tElemPtr Temp;
		Temp = L->Act->ptr;
		L->Act->ptr = Temp->ptr;
		free(Temp);
	}
}

void PostInsert (tList *L, int val) {
/*
** Vloží prvek s hodnotou val za aktivní prvek seznamu L.
** Pokud nebyl seznam L aktivní, nic se neděje!
** V případě, že není dostatek paměti pro nový prvek při operaci malloc,
** zavolá funkci Error().
**/
	
	if (L->Act != NULL)  //kontrola podminek pri kterych se nic nedeje
	{
		tElemPtr New = NULL;

	    if((New = malloc(sizeof(struct tElem))) == NULL)  //alokace pameti
	    {
	    	Error();
	    }

	    else  //vkladani noveho prvku
	    {
	    	New->ptr = L->Act->ptr;
	    	New->data = val;
	    	L->Act->ptr = New;
	    }
	}
}

void Copy (tList *L, int *val) {
/*
** Prostřednictvím parametru val vrátí hodnotu aktivního prvku seznamu L.
** Pokud seznam není aktivní, zavolá funkci Error().
**/
	
	if (L->Act == NULL)
	{
		Error();
	}

	else
	{
		*val = L->Act->data;  //nahrani hodnoty prvku do ukazatele
	}
}

void Actualize (tList *L, int val) {
/*
** Přepíše data aktivního prvku seznamu L hodnotou val.
** Pokud seznam L není aktivní, nedělá nic!
**/
	
	if (L->Act != NULL)
	{
		L->Act->data = val;  //predani hodnoty strukture
	}
}

void Succ (tList *L) {
/*
** Posune aktivitu na následující prvek seznamu L.
** Všimněte si, že touto operací se může aktivní seznam stát neaktivním.
** Pokud není předaný seznam L aktivní, nedělá funkce nic.
**/
	
	if (L->Act != NULL)
	{
		L->Act = L->Act->ptr;  //aktualni prvek nastaven na dalsi v poradi
	}
}

int Active (tList *L) {		
/*
** Je-li seznam L aktivní, vrací nenulovou hodnotu, jinak vrací 0.
** Tuto funkci je vhodné implementovat jedním příkazem return. 
**/
	
	return ((L->Act == NULL) ? 0 : 1);  //test aktivity seznamu pres ternarni operator
}

/* Konec c201.c */
