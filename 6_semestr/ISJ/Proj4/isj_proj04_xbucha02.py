#!/usr/bin/env python3
"""
    ISJ - Skriptovací jazyky
    Projekt 4
    xbucha02, Petr Buchal
"""

def can_be_a_set_member_or_frozenset(item):
    """
    vrati vstupni polozku item,
    pokud tato muze byt prvkem mnoziny v Pythonu,
    v opacnem pripade frozenset(item)
    """
    lst = set() #vytvoreni mnoziny
    try: #pokus o vlozeni itemu do mnoziny
        lst.add(item)
        return(item)
    except: #pokud nelze vlozit vrati se frozenset(item)
        return frozenset(item)

def all_subsets(lst):
    """
    na vstupu dostane seznam a vytvori
    z nej vytvori seznam,
    odpovidajici mnozine vsech podmnozin
    """
    if len(lst) == 1: #pokud ma seznam delku 1 (rekurzivni zarazka)
        return [lst] #vraceni prvniho a jedineho prvku seznamu

    subsets = all_subsets(lst[:-1]) #rekurzivni volani funkce, s listem, ktery ma o jeden prvek mene

    subsets.append([lst[-1]]) #pridani prvku, ktery chybel v predchozim volani

    for sub in subsets[:(len(subsets)-1)]: #tento prvek se postupne pripeni ke vsem prvkum mimo sama sebe
        if sub != []: # a mimo prvek na zacatku
            subsets.append(sub+[lst[-1]]) #pripevneni prvku

    #(pocet mnozin - 1 == delka pole mnozin)
    if (2 ** len(lst) - 1) == len(subsets): #pri zpracovani dvou prvku se na zacatek seznamu pripne prazdna mnozina
        subsets.insert(0, []) #vlozeni prazdne mnoziny

    return subsets

def all_subsets_excl_empty(*arguments, exclude_empty=True):
    """
    pri volani dostane prvky seznamu primo jako argumenty,
    ze kterych vytvori seznam, odpovidajici mnozine vsech,
    prazdna podmnozina ma volitelny parametr exclude_empty, ktery,
    kdyz neni ve volani uveden, nebo je jeho hodnota True,
    vrati vysledek bez prazdneho seznamu
    """
    lst = [] #vytvoreni seznamu
    if len(arguments) > 1: #pokud je volitelnych argumentu vice, spoj je do jednoho seznamu
        for arg in arguments:
            lst.append(arg)
    else:
        lst = list(arguments[0]) #orezani nejhornejsi dimenze seznamu

    if len(lst) == 1: #pokud ma seznam delku 1 (rekurzivni zarazka)
        return [lst] #vraceni prvniho a jedineho prvku seznamu

    subsets = all_subsets_excl_empty(lst[:-1], exclude_empty=exclude_empty) #rekurzivni volani funkce, s listem, ktery ma o jeden prvek mene

    subsets.append([lst[-1]]) #pridani prvku, ktery chybel v predchozim volani

    for sub in subsets[:(len(subsets)-1)]: #tento prvek se postupne pripeni ke vsem prvkum mimo sama sebe
        if sub != []: # a mimo prvek na zacatku
            subsets.append(sub+[lst[-1]]) #pripevneni prvku

    #(pocet mnozin - 1 == delka pole mnozin)
    if (2 ** len(lst) - 1) == len(subsets) and exclude_empty is not True: #pri zpracovani dvou prvku se na zacatek seznamu pripne prazdna mnozina
        subsets.insert(0, []) #vlozeni prazdne mnoziny

    return subsets
