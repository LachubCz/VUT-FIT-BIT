# @author Petr Buchal(xbucha02)

import getopt, sys
import os, fnmatch

####################################################################################
#######################################Tridy########################################
####################################################################################

class database:

    def __init__(self):
        self.functions = []

    def put_function(self, functionToPut):
        self.functions.append(functionToPut)

    def get_function(self, index):
        return self.functions[index]

class function:

    def __init__(self):
        self.file = ""
        self.name = ""
        self.varargs = {}
        self.rettype = ""

    def put_varargs(self, key, value):
        self.varargs[key] = value

    def put_rettype(self, string):
        self.rettype = string

    def put_name(self, string):
        self.name = string

    def put_file(self, string):
        self.file = string

    def get_info(self, type):
        if type == 'file':
            return self.file
        elif type == 'name':
            return self.name
        elif type == 'varargs':
            return self.varargs
        elif type == 'rettype':
            return self.rettype

class parser:

    def readByChar(filename, database):
        word = ""
        temp = ""
        lastChar = '0'
        state = 0
        inFunction = False
        inComment = False
        pointer = False
        with open(filename) as file:
            while True:
                c = file.read(1)
                if not c:
                    #print ("End of file")
                    if inFunction:
                        #vkladani argumentu
                        database.put_function(functionToPut)
                    break
                #rozhodovani zdali se ctou slova z funkce
                if inFunction:
                    if state == 3: #cteni navratoveho typu
                        if (c.isspace() == False):
                            word += c
                        else:
                            functionToPut = function() #mozna se budou muset funke cislovat
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
                            functionToPut.put_name(word)
                            word = ""
                            state = 6
                    elif state == 6: #hledani zacatku typu argumentu
                        if (c.isspace() == False):
                            if c == ')':
                                state = 0
                                inFunction = False
                                functionToPut.put_file(filename)
                                database.put_function(functionToPut)
                            else:
                                word = c
                                state = 7
                    elif state == 7: #cteni typu argumentu
                        if (c.isspace() == False):
                            word += c
                        else:
                            temp = word
                            word = ""
                            state = 8
                    elif state == 8: #hledani zacatku nazvu argumentu
                        if (c.isspace() == False):
                            if c == '*':
                                pointer = True
                                word = c + " "
                            else:
                                word = c
                            state = 9
                    elif state == 9: #cteni nazvu argumentu
                        if (c.isspace() == False and c != ')'):
                            if c != ',':
                                word += c
                            pointer = False
                        else:
                            if pointer == True:
                                pass
                            else:
                                #print(word, " - ", temp)
                                functionToPut.put_varargs(word, temp)
                                word = ""
                                if (c == ')'):
                                    state = 0
                                    inFunction = False
                                    functionToPut.put_file(filename)
                                    database.put_function(functionToPut)
                                else:
                                    state = 6;
                elif inComment:
                    if state == 1:
                        if c == '\n':
                            inComment = False
                    elif state == 2:
                        if c == '/' and lastChar == '*':
                            inComment = False
                        lastChar = c
                else:
                    if lastChar == '0':
                        lastChar = c

                    if lastChar == '/' and c == '/':
                        inComment = True
                        state = 1

                    if lastChar == '/' and c == '*':
                        inComment = True
                        state = 2

                    if lastChar.isspace() and c.isspace(): 
                        lastChar = c

                    if lastChar != '0' and (c.isspace() == False) and c != '/':
                        inFunction = True
                        state = 3
                        word = lastChar

                #print ("Read a character: ", c, " State: ", state)
        
####################################################################################
######################################Funkce########################################
####################################################################################

def output_func(output, final):
    if output == 'STDOUT':
        print(final)
    else:
        f1 = open(output, 'w')
        f1.write(final)

def printDatabase(database):
    final = "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
    final += "<functions dir=\"\">\n"
    for functionToGet in database.functions:
        final += "  <function file=\"" + functionToGet.file + "\" name=\"" + functionToGet.name + "\" varargs=\"no\" rettype=\"" + functionToGet.rettype + "\">\n"
        for parameter in functionToGet.varargs.keys():
            final += "      <param number=\"\" type=\"" + functionToGet.varargs[parameter] + "\" />\n"
    final += "</functions>"
    output_func('STDOUT', final)

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

def recursive_gold(act_dir):
    for fileordir in os.listdir(act_dir):
        if fnmatch.fnmatch(fileordir, '*'):
            fileordir = act_dir + "/" + fileordir
            if os.path.isdir(fileordir):
                #print(fileordir, "<-- FOLDER\n")
                recursive_gold(fileordir)
            elif os.path.isfile(fileordir):
                #print(fileordir, "<-- FILE\n")
                analysa(fileordir)

def analysa(file):
    pass

####################################################################################
###############################Zpracovani argumentu#################################
####################################################################################

help = False
input = 'STDIN'
output = 'STDOUT'
pretty = False
no_inline = False
max_par = -1
no_duplicates = False
remove_whitespace = False

options, remainder = getopt.getopt(sys.argv[1:], ':', ['input=', 
                                                         'output',
                                                         'max-par=',
                                                         'help',
                                                         'pretty',
                                                         'no-inline',
                                                         'no-duplicates',
                                                         'remove-whitespace',
                                                         ])

for opt, arg in options:
    if opt in '--input':
        input = arg
    elif opt in '--output':
        output = arg
    elif opt in '--max-par':
        max_par = arg
    elif opt == '--help':
        help = True
    elif opt == '--pretty':
        pretty = True
    elif opt == '--no-inline':
        no_inline = True
    elif opt == '--no-duplicates':
        no_duplicates = True
    elif opt == '--remove-whitespace':
        remove_whitespace = True

#testovaci vypis argumentu
#print ("ARGV      :", sys.argv[1:])
#print ("OPTIONS   :", options)
#print ("HELP              :", help)
#print ("INPUT             :", input)
#print ("OUTPUT            :", output)
#print ("PRETTY            :", pretty)
#print ("NO-INLINE         :", no_inline)
#print ("MAX-PAR           :", max_par)
#print ("NO-DUPLICATES     :", no_duplicates)
#print ("REMOVE-WHITESPACE :", remove_whitespace)
#print ("REMAINING         :", remainder)

if len(remainder) != 0:
    sys.exit(1)

if help == True and ( input != 'STDIN' or output != 'STDOUT' or pretty != False or no_inline != False or max_par != -1 or no_duplicates != False or remove_whitespace != False ):
    sys.exit(1)

if help == True:
    help_str = help_func()
    output_func(output, help_str)

database = database() #inicializace databaze

path = os.path.abspath(input)
"""
if fileordir(input) == 0:
    recursive_gold(path)
elif fileordir(input) == 1:
    analysa(path)
elif fileordir(input) == 2:
    recursive_gold('.')

"""
parser.readByChar(path, database)
functionToGet = database.get_function(0)
#print("Rettype: ", functionToGet.rettype, " Name: ", functionToGet.name, "File: ", functionToGet.file)

#for x in functionToGet.varargs.keys():
#    print (x, " => ", functionToGet.varargs[x])

#for key, value in functionToGet.varargs.items():
#    print("{}: {}".format(key, value))
printDatabase(database)
