#!/usr/bin/env python3
"""
    ISJ - Skriptovaci jazyky
    Projekt 7
    xbucha02, Petr Buchal
"""

class TooManyCallsError(Exception):
    """
    exception vyvolana dekoratorem limit_calls
    """
    def __init__(self, message):
        super().__init__(message)

class limit_calls(object):
    """
    dekorator s parametry max_calls a error_message_tail, 
    ktery limituje pocet volani obecnych funkci
    """
    def __init__(self, max_calls=2, error_message_tail="called too often"):
        """
        inicializace metody
        """
        self.max_calls = max_calls
        self.curr_calls = 0
        self.error_message_tail = error_message_tail

    def __call__(self, function):
        def wrapper(*args, **kwargs):
            """
            wrapper nad volanou funkci
            """
            if self.curr_calls < self.max_calls:
                self.curr_calls += 1
                return function(*args, **kwargs)
            else:
                raise TooManyCallsError("function \"" + function.__name__ + "\" - " + self.error_message_tail)
                return string + limit_return

            return function
        return wrapper

def ordered_merge(*args, selector=[]):
    """
    generatorova funkce je volana s libovolnym poctem
    iterovatelnych objektu a parametrem selector,
    udavajicim, z ktereho iterovatelneho objektu ma prvek na dane
    pozici byt, a bude vybirat prvky v zadanem poradi
    """
    iterators = list()

    try:
        for i, item in enumerate(args):
            iterators.append(iter(item))
    except TypeError:
        print ("TypeError: ", type(item), " object is not iterable")
        return None
    
    for i, item in enumerate(selector):
        yield(next(iterators[item]))

class Log():
    """
    trida pro logovani
    """
    def __init__(self, filename):
        """
        inicializace metody
        """
        self.filename = filename
        self.file = None
        
    def __enter__(self):
        """
        prvni zapis do souboru
        """
        self.file = open(self.filename, "w+")
        self.logging("Begin")
        return self

    def logging(self, string, next_line=True):
        """
        zapisovani do souboru
        """
        if next_line:
            self.file.write(string + "\n")
        else:
            self.file.write(string)
            
    def __exit__(self, type, value, traceback):
        """
        posledni zapis do souboru
        """
        self.logging("End", False)
        self.file.close()
