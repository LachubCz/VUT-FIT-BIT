#!/usr/bin/env python3
"""
    ISJ - Skriptovaci jazyky
    Projekt 6
    xbucha02, Petr Buchal
"""
import itertools
import operator

def first_nonrepeating(string):
    """
    funkcedostane na vstup retezec a vrati prvni neopakujici se znak ze
    vstupniho retezce
    """
    if type(string) is not str:
        return None
    
    parsed = list(string)
    unique = []
    duplicate = []
    result = []
    
    for i in parsed:
        if i not in unique:
            unique.append(i)
        else:
            duplicate.append(i)

    result = list(unique)

    for i, item in enumerate(unique):
        for e, elem in enumerate(duplicate):
            if elem == item:
                if elem in result:
                    result.remove(elem)

    if not result:
        return None
    else:
        bad_chars = set()
        bad_chars.update(' ', '\t')
        if result[0] in bad_chars:
            return None
        else:
            return result[0]

def get_res_down(num0, num1, num2, num3, op0, op1, op2, res):
    """
    pomocna funkce pro funkci combine4, ktera prochazi kombinace hodnot
    od zacatku po konec
    """
    oprs = {'+' : operator.add, '-' : operator.sub, '*' : operator.mul, '/' : operator.__truediv__}

    if num1 == 0 and op0 == '/':
        return None
    first = oprs[op0](num0, num1)
    if num2 == 0 and op1 == '/':
        return None
    second = oprs[op1](first, num2)
    if num3 == 0 and op2 == '/':
        return None
    third = oprs[op2](second, num3)
    if third == res:
        return '(('+str(num0)+op0+str(num1)+')'+op1+str(num2)+')'+op2+str(num3)
    
    
def get_res_up(num0, num1, num2, num3, op0, op1, op2, res):
    """
    pomocna funkce pro funkci combine4, ktera prochazi kombinace hodnot
    od konce po zacatek
    """
    oprs = {'+' : operator.add, '-' : operator.sub, '*' : operator.mul, '/' : operator.__truediv__}
    
    if num3 == 0 and op2 == '/':
        return None
    third = oprs[op2](num2, num3)
    if third == 0 and op1 == '/':
        return None
    second = oprs[op1](num1, third)
    if second == 0 and op0 == '/':
        return None
    first = oprs[op0](num0, second)
    if first == res:
        return str(num0)+op0+'('+str(num1)+op1+'('+str(num2)+op2+str(num3)+'))'
    
def combine4(*arguments):
    """
    funkce dostane ctverici 4 kladnych celych cisel a 
    ocekavany vysledek a vrati setrideny seznam  unikatnich reseni 
    matematickych hadanek s vysledkem operaci +, -, *, /
    """
    if len(arguments) != 2:
        return None
    
    if type(arguments[0]) == list and type(arguments[1]) == int:
        arr = arguments[0]
        res = arguments[1]
    elif type(arguments[0]) == int and type(arguments[1]) == list:
        res = arguments[0]
        arr = arguments[1]
    else:
        return None

    ops = ['+', '-', '*', '/']
    results = list()
    
    numbers = list(itertools.permutations(arr, 4))
    operations = list(itertools.permutations(ops, 3))
    
    for i, item in enumerate(numbers):
        num0 = item[0]
        num1 = item[1]
        num2 = item[2]
        num3 = item[3]
        for e, elm in enumerate(operations):
            op0 = elm[0]
            op1 = elm[1]
            op2 = elm[2]

            result = get_res_down(num0, num1, num2, num3, op0, op1, op2, res)
            if result:
                results.insert(-1,result)
            
            result = get_res_up(num0, num1, num2, num3, op0, op1, op2, res)
            if result:
                results.insert(-1,result)

    return list(set(results))
