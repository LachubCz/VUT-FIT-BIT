# @author Petr Buchal(xbucha02)

import getopt, sys
import os, fnmatch
import argparse

####################################################################################
#######################################Tridy########################################
####################################################################################

class database:

    def __init__(self, help, input, output, pretty, no_inline, max_par, no_duplicates, remove_whitespace):
        self.functions = []
        self.parameters = []
        self.parameters.append(help)
        self.parameters.append(input)
        self.parameters.append(output)
        self.parameters.append(pretty)
        self.parameters.append(no_inline)
        self.parameters.append(max_par)
        self.parameters.append(no_duplicates)
        self.parameters.append(remove_whitespace)

    def put_function(self, functionToPut):
        self.functions.append(functionToPut)

    def get_function(self, index):
        return self.functions[index]

class function:

    def __init__(self):
        self.file = ""
        self.name = ""
        self.varargs = "no"
        self.parameters = []
        self.rettype = ""

    def put_parameter(self, string):
        self.parameters.append(string)

    def put_rettype(self, string):
        self.rettype += string

    def put_name(self, string):
        self.name = string

    def put_file(self, string):
        self.file = string

    def put_varargs(self):
        self.varargs = "yes"

class parserForFile:

    def readByChar(filename, database, relativepath):
        word = ""
        temp = ""
        lastChar = '0'
        state = 0
        inFunction = False
        inComment = False
        end = False

        with open(filename) as file:
            while True:
                c = file.read(1)
                if not c:
                    break

                #funkce
                if inFunction:

                    #pred parametry funkce
                    if state == 3: #cteni navratoveho typu
                        if (c.isspace() == False):
                            word += c
                        else:
                            functionToPut = function()
                            functionToPut.put_rettype(word)
                            state = 4
                    elif state == 4: #hledani zacatku nazvu funkce
                        if (c.isspace() == False):
                            word = c
                            state = 5
                    elif state == 5: #cteni nazvu funkce
                        if (c.isspace() == False and c != '('):
                            word += c
                        else:
                            if c == '(':
                                functionToPut.put_name(word)
                                word = ""
                                state = 6
                            elif c.isspace() == True:
                                word = " " + word
                                functionToPut.put_rettype(word)
                                word = ""
                                state = 4

                    #parametry funkce
                    elif state == 6: #hledani zacatku typu argumentu
                        if (c.isspace() == False):
                            if c == ')':
                                state = 0
                                inFunction = False
                                word = ""
                                functionToPut.put_file(relativepath)
                                database.put_function(functionToPut)
                            else:
                                word = c
                                state = 7

                    elif state == 7: #cteni typu argumentu
                        if (c.isspace() == False and c != ')'):
                            word += c
                        else:
                            if word == 'void':
                                functionToPut.put_file(relativepath)
                                database.put_function(functionToPut)
                                word = ""
                                inFunction = False
                            elif word == '...':
                                functionToPut.put_varargs()
                                functionToPut.put_file(relativepath)
                                word = ""
                                inFunction = False
                                database.put_function(functionToPut)
                            else:
                                temp = word
                                word = ""
                                state = 8

                    elif state == 8: #hledani zacatku nazvu argumentu
                        if (c.isspace() == False):
                            if c == '*':
                                temp = temp + " " + c
                                functionToPut.put_parameter(temp)
                                word = ""
                                state = 10
                            else:
                                word = c
                                state = 9

                    elif state == 9: #cteni nazvu argumentu
                        if (c.isspace() == False and c != ')'):
                            if c != ',':
                                word += c
                            else:
                                state = 6
                        else:
                            #functionToPut.put_parameter(word)
                            if c == ')':
                                inFunction = False
                                functionToPut.put_file(relativepath)
                                functionToPut.put_parameter(temp)
                                database.put_function(functionToPut)
                            else:
                                state = 11;

                    elif state == 10: #cteni nazvu argumentu
                        if c == ',':
                            state = 6
                        elif c == ')':
                            inFunction = False
                    
                    elif state == 11:
                        if c.isspace():
                            pass
                        elif c == ',':
                            state = 6
                            functionToPut.put_parameter(temp)
                            word = ""
                        else:
                            temp = temp + " " + word
                            if c == '*':
                                temp = temp + " " + c
                                functionToPut.put_parameter(temp)
                                word = ""
                                state = 10
                            else:
                                word = c
                                state = 9

                #komentare
                elif inComment:
                    if state == 1:
                        if c == '\n':
                            inComment = False
                            lastChar = '0'
                    elif state == 2:
                        if c == '/' and lastChar == '*':
                            inComment = False
                        lastChar = c

                #mimo funkci mimo komentar
                else:
                    if c == ';' or c == ')' or c == '\n':
                        pass
                    elif lastChar == '0':
                        lastChar = c
                    elif lastChar == '/' and c == '/':
                        inComment = True
                        state = 1
                    elif lastChar == '/' and c == '*':
                        inComment = True
                        state = 2
                    elif lastChar.isspace() and c.isspace(): 
                        lastChar = c
                    elif lastChar != '0' and (c.isspace() == False) and c != '/':
                        inFunction = True
                        state = 3
                        word = lastChar + c
                        lastChar = '0'

        
####################################################################################
######################################Funkce########################################
####################################################################################

def output_func(output, final):
    if output == 'STDOUT':
        print(final)
        sys.exit(0)
    else:
        f1 = open(output, 'w')
        f1.write(final)
        sys.exit(0)

def printDatabase(database):
    final = "<?xml version=\"1.0\" encoding=\"UTF-8\"?>"
    if database.parameters[3] != '-1':
        final+='\n'

    final += "<functions dir=\"\">"
    if database.parameters[3] != '-1':
        final+='\n'

    for functionToGet in database.functions:
        e = 0
        while e < int(database.parameters[3]):
            e+=1
            final+=' '
        
        final += "<function file=\"" + functionToGet.file + "\" name=\"" + functionToGet.name + "\" varargs=\"" + functionToGet.varargs + "\" rettype=\"" + functionToGet.rettype + "\">"
        i = 1
        if database.parameters[3] != '-1':
            final+='\n'

        for parameter in functionToGet.parameters:
            e = 0
            while e < (int(database.parameters[3]) * 2):
                final+=' '
                e+=1
            final += "<param number=\"" + str(i) + "\" type=\"" + parameter + "\" />"
            if database.parameters[3] != '-1':
                final+='\n'
            i += 1
        
        e = 0
        while e < int(database.parameters[3]):
            final+=' '
            e+=1
        final += ("</function>")
        if database.parameters[3] != '-1':
            final+='\n'

    final += "</functions>"

    output_func(database.parameters[2], final)

def help_func():
    help_str = '--help ## Viz spolecne zadani vsech uloh.\n'

    help_str += '--input=fileordir ## Zadany vstupni soubor nebo adresar se zdrojovym kodem v jazyce C.'
    help_str += 'Predpokladejte, ze soubory budou v kodovani UTF-8. Je-li zadana cesta k adresari, tak jsou'
    help_str += 'postupne analyzovany vsechny soubory s priponou .h v tomto adresari a jeho podadresarich.'
    help_str += 'Pokud je zadana primo cesta k souboru (nikoliv k adresari), tak priponu souboru nekontrolujte.'
    help_str += 'Pokud nebude tento parametr zadan, tak se analyzuji vsechny hlavickove soubory (opet pouze'
    help_str += 's priponou .h) z aktualniho adresare a vsech jeho podadresaru.\n'

    help_str += '--output=filename ## Zadany vystupni soubor ve formatu XML v kodovani UTF-8 (presny'
    help_str += 'format viz nize). Pokud tento parametr neni zadan, tak dojde k vypsani vysledku na standardni vystup.\n'

    help_str += '--pretty-xml=k ## Skript zformatuje vysledny XML dokument tak, ze (1) kazde nove zanoreni'
    help_str += 'bude odsazeno o k mezer oproti predchozimu a (2) XML hlavicka bude od korenoveho elementu'
    help_str += 'oddelena znakem noveho radku. Pokud k neni zadano, tak se pouzije hodnota 4. Pokud tento'
    help_str += 'parametr nebyl zadan, tak se neodsazuje (ani XML hlavicka od korenoveho elementu).\n'

    help_str += '--no-inline ## Skript preskoci funkce deklarovane se specifikatorem inline.\n'

    help_str += '--max-par=n ## Skript bude brat v uvahu pouze funkce, ktere maji n ci mene parametru (n musi'
    help_str += 'byt vzdy zadano). U funkci, ktere maji promenny pocet parametru, pocitejte pouze s fixnimi parametry.\n'

    help_str += '--no-duplicates ## Pokud se v souboru vyskytne vice funkci se stejnym jmenem (napr.'
    help_str += 'deklarace funkce a pozdeji jeji definice), tak se do vysledneho XML souboru ulozi pouze prvni'
    help_str += 'z nich (uvazujte pruchod souborem shora dolu). Pokud tento parametr neni zadan, tak se do'
    help_str += 'vysledneho XML souboru ulozi vsechny vyskyty funkce se stejnym jmenem.\n'

    help_str += '--remove-whitespace ## Pri pouziti tohoto parametru skript odstrani z obsahu atributu'
    help_str += 'rettype a type (viz nize) vsechny prebytecne mezery.' 

    return help_str

def fileordir(name):
    if os.path.isdir(name):
        return 0
    if os.path.isfile(name):
        return 1
    if name == "STDIN":
        return 2

def recursive_gold(act_dir, database, relativepath):
    for fileordir in os.listdir(act_dir):
        if fnmatch.fnmatch(fileordir, '*'):
            relativepath = relativepath + "/" + fileordir
            fileordir = act_dir + "/" + fileordir
            if os.path.isdir(fileordir):
                recursive_gold(fileordir, database, relativepath)
            elif os.path.isfile(fileordir):
                analysa(fileordir, database, relativepath)

def analysa(file, database, relativepath):
    parserForFile.readByChar(file, database, relativepath)

####################################################################################
###############################Zpracovani argumentu#################################
####################################################################################

parser = argparse.ArgumentParser(add_help=False, allow_abbrev=False)

parser.add_argument('--input', action='store', dest='input', default='STDIN')
parser.add_argument('--output', action='store', dest='output', default='STDOUT')
parser.add_argument('--max-par', action='store', dest='max_par', default='-1')
parser.add_argument('--help', action='store_true', dest='help', default=False)
parser.add_argument('--pretty-xml', action='store', dest='pretty', default='-1', nargs='?')
parser.add_argument('--no-inline', action='store_true', dest='no_inline', default=False)
parser.add_argument('--no-duplicates', action='store_true', dest='no_duplicates', default=False)
parser.add_argument('--remove-whitespace', action='store_true', dest='remove_whitespace', default=False)

try: 
    results = parser.parse_args()
except SystemExit:
    sys.exit(1)

if results.pretty == None:
    results.pretty = 4

if results.help == True and ( results.input != 'STDIN' or results.output != 'STDOUT' or results.pretty != '-1' or results.no_inline != False  or results.max_par != '-1'  or results.no_duplicates != False or results.remove_whitespace != False ):
    sys.exit(1)

if results.input == 'STDIN':
    results.input = '.'

if os.path.isdir(results.output):
    sys.exit(3)

if results.help == True:
    help_str = help_func()
    output_func(results.output, help_str)

database = database(results.help, results.input, results.output, results.pretty, results.no_inline, results.max_par, results.no_duplicates, results.remove_whitespace) #inicializace databaze

path = os.path.abspath(results.input)

if fileordir(results.input) == 0:
    recursive_gold(path, database, results.input)
elif fileordir(results.input) == 1:
    analysa(path, database, results.input)
elif fileordir(results.input) == 2:
    recursive_gold('.', database, '.')

printDatabase(database)
