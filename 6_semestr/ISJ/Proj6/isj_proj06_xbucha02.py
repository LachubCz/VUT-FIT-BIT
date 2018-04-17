#!/usr/bin/env python3
"""
    ISJ - Skriptovaci jazyky
    Projekt 5
    xbucha02, Petr Buchal
"""

import copy

def first_nonrepeating(string):
    parsed = list(string)
    unique = []
    duplicate = []
    result = []

    for i in parsed:
        if i not in unique:
            unique.append(i)
        else:
            duplicate.append(i)

    result = copy.copy(unique)

    for i, item in enumerate(unique):
        for e, elem in enumerate(duplicate):
            if elem == item:
                result.remove(elem)

    if result == []:
        return None
    else:
        return result[0]
