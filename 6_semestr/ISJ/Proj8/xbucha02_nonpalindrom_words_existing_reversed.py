#!/usr/bin/env python3

import fileinput

words = set()
palindroms = set()
for line in fileinput.input():
    string = line.rstrip()
    words.add(string)
    if string == string[::-1]:
        palindroms.add(string)

result = [w for w in words if w not in palindroms and w[::-1] in words]

print(sorted(result))
