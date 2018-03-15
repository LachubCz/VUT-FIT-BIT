#!/usr/bin/env python3

from itertools import permutations

def match_permutations(string, words):
    # permutations as a set
    # {'tac', 'act', 'atc', 'tca', 'cat', 'cta'}
    perms = set(map(''.join, permutations(string)))
    # sorted list of matching strings
    matching_perms = list(set(perms).intersection(words)) # vase reseni
    return matching_perms

def plur2sing(singular, plural):
    # {'goose':'geese', 'man':'men', 'child':'children'}
    sg2pl = {s: p for s, p in zip(singular, plural)}
    # inversion using zip
    pl2sg = {p: s for s, p in zip(singular, plural)} # vase reseni
    return pl2sg

def vect2word(word2vect):
    v2w = word2vect.values() # vase reseni
    return v2w


def test():
    assert match_permutations('act', {'cat', 'rat', 'dog', 'act'}) == ['act', 'cat']
    assert plur2sing(['goose', 'man', 'child'], ['geese', 'men', 'children']) == {'geese': 'goose', 'men': 'man', 'children': 'child'}
    assert sum(k[1] for k in vect2word({'king': [3, 1], 'queen': [6, 3], 'uncle': [4, 3], 'aunt': [8, 9]})) == 16


if __name__ == '__main__':
    test()
