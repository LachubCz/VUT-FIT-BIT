# @author Petr Buchal(xbucha02)

import sys
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
        duplicate = False
        if self.parameters[6] == False:
            if self.parameters[5] == '-1':
                self.functions.append(functionToPut)
            else:
                if len(functionToPut.parameters) <= int(self.parameters[5]):
                    self.functions.append(functionToPut)
        else:
            for functionToCheck in self.functions:
                if functionToCheck.name == functionToPut.name:
                    duplicate = True
            if duplicate == False:
                if self.parameters[5] == '-1':
                    self.functions.append(functionToPut)
                else:
                    if len(functionToPut.parameters) <= int(self.parameters[5]):
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

    def whiteSpaceStrech(string):
        lenght = len(string)
        i = 0
        newStr = ""
        while (i < lenght):
            i+=1
            newStr += " "
        return newStr

    def readByChar(filename, database, relativepath):
        word = ""
        temp = ""
        whitespace = ""
        ws_temp = ""
        lastChar = '0'
        state = 0
        inFunction = False
        inComment = False
        end = False
        inString = False
        inMacro = False
        first = True
        inBody = False
        with open(filename) as file:
            while True:
                c = file.read(1)
                if not c:
                    break
                #funkce
                if inFunction:
                    #predni cast funkce
                    if state == 3: #cteni prvniho slova
                        if (c.isspace() == False):
                            word += c
                        else:
                            if "inline" in word and database.parameters[4]:
                                state = 15
                                word = ""
                            else:
                                whitespace = c
                                functionToPut = function()
                                functionToPut.put_rettype(word)
                                state = 4
                    elif state == 4: #hledani zacatku druheho/dalsiho slova
                        if (c.isspace() == False):
                            if c == '(':
                                functionToPut.put_name(word)
                                whitespace = ""
                                state = 6
                                word = ""
                                temp = ""
                            else:
                                functionToPut.put_rettype(temp)
                                temp = ""
                                word = c
                                state = 5
                        else:
                            whitespace += c
                    elif state == 5: #cteni druheho/dalsiho slova
                        if (c.isspace() == False and c != '('):
                            word += c
                        else:
                            if c == '(':
                                functionToPut.put_rettype(temp)
                                functionToPut.put_name(word)
                                word = ""
                                temp = ""
                                whitespace = ""
                                state = 6
                            elif c.isspace() == True:
                                if "inline" in word and database.parameters[4]:
                                    state = 15
                                    word = ""
                                else:
                                    if database.parameters[7] != True:
                                        whitespace = parserForFile.whiteSpaceStrech(whitespace)
                                        temp = whitespace + word
                                    else:
                                        temp = " " + word
                                    whitespace = c
                                    state = 4
                    #vnitrni cast funkce
                    elif state == 6: #hledani prvniho slova typu argumentu
                        if (c.isspace() == False):
                            if c == ')':
                                state = 0
                                inFunction = False
                                word = ""
                                temp = ""
                                functionToPut.put_file(relativepath)
                                database.put_function(functionToPut)
                            else:
                                word = c
                                state = 7
                    elif state == 7: #cteni prvniho slova typu argumentu
                        if (c.isspace() == False and c != ')'):
                            word += c
                        else:
                            if word == 'void':
                                functionToPut.put_file(relativepath)
                                database.put_function(functionToPut)
                                word = ""
                                temp = ""
                                inFunction = False
                            elif word == '...':
                                functionToPut.put_varargs()
                                functionToPut.put_file(relativepath)
                                word = ""
                                temp = ""
                                inFunction = False
                                database.put_function(functionToPut)
                            elif c == ')':
                                inFunction = False
                                functionToPut.put_file(relativepath)
                                functionToPut.put_parameter(word)
                                database.put_function(functionToPut)
                                word = ""
                                temp = ""
                            else:
                                temp = word
                                word = ""
                                whitespace = c
                                state = 8
                    elif state == 8: #hledani zacatku druheho/dalsiho slova typu argumentu
                        if (c.isspace() == False):
                            if c == '*':
                                if database.parameters[7] != True:
                                    whitespace = parserForFile.whiteSpaceStrech(whitespace)
                                    temp = temp + whitespace + c
                                else:
                                    temp = temp + " " + c
                                word = ""
                                whitespace = ""
                            else:
                                word = c
                                state = 9
                        else:
                            whitespace += c
                    elif state == 9: #cteni druheho/dalsiho slova typu argumentu
                        if (c.isspace() == False and c != ')'):
                            if c != ',':
                                word += c
                            else:
                                functionToPut.put_parameter(temp)
                                whitespace = ""
                                state = 6
                        else:
                            if c == ')':
                                inFunction = False
                                word = ""
                                functionToPut.put_file(relativepath)
                                functionToPut.put_parameter(temp)
                                temp = ""
                                whitespace = ""
                                database.put_function(functionToPut)
                            else:
                                ws_temp = whitespace
                                whitespace = c
                                state = 11;
                    elif state == 10: #cekani na konec soucasneho argumentu
                        if c == ',':
                            state = 6
                        elif c == ')':
                            inFunction = False
                    elif state == 11: #rozhodovani zda-li je slovo typ argumentu nebo nazev
                        if c.isspace():
                            whitespace += c
                        elif c == ',':
                            state = 6
                            functionToPut.put_parameter(temp)
                            word = ""
                        else:
                            if first == True:
                                if database.parameters[7] != True:
                                    ws_temp = parserForFile.whiteSpaceStrech(ws_temp)
                                    temp = temp + ws_temp + word
                                else:
                                    temp = temp + " " + word
                                first = False
                                ws_temp = ""
                            if c == '*':
                                if database.parameters[7] != True:
                                    whitespace = parserForFile.whiteSpaceStrech(whitespace)
                                    temp = temp + whitespace + c
                                else:
                                    temp = temp + " " + c
                                word = ""
                                whitespace = ""
                            else:
                                word = c
                                state = 9
                                first = True
                    elif state == 15: #cekani na konec funkce
                        if c == ';' or c == ')':
                            inFunction = False
                            word = ""
                            lastChar = '0'
                #komentare
                elif inComment:
                    if state == 1:
                        if c == '\u005C':
                            state = 12
                        if c == '\n':
                            inComment = False
                            lastChar = '0'
                    elif state == 2:
                        if c == '/' and lastChar == '*':
                            inComment = False
                            lastChar = '0'
                        else:
                            lastChar = c
                    elif state == 12:
                        if c == '\n':
                            state = 13
                    elif state == 14:
                        if c == '\n':
                            inComment = False
                            lastChar = '0'
                #retezce
                elif inString:
                    if state == 16:
                        if lastChar == '\u005C' and c == "\"":
                            pass
                        elif c == "\"":
                            inString = False
                            lastChar = '0'
                        else:
                            lastChar = c
                    elif state == 17:
                        if lastChar == '\u005C' and c == "\'":
                            pass
                        elif c == "\'":
                            inString = False
                            lastChar = '0'
                        else:
                            lastChar = c
                #macra
                elif inMacro:
                    if state == 18:
                        if c == '\u005C':
                            state = 22
                        elif lastChar == '/' and c == '*':
                            lastChar = '0'
                            state = 19
                        elif c == '\n':
                            inMacro = False
                            lastChar = '0'
                        else:
                            lastChar = c
                    elif state == 19:
                        if c == '\n':
                            state = 20
                        elif lastChar == '*' and c == '/':
                            state = 18
                            lastChar = '0'
                        else:
                            lastChar = c
                    elif state == 20:
                        if c == '\n':
                            inMacro = False
                            lastChar = '0'
                    elif state == 22:
                        if c == '\n':
                            state = 20
                #telo funkce
                elif inBody:
                    if state == 21:
                        if c == '{':
                            counter += 1
                        elif c == '}':
                            counter -= 1
                            if counter == 0:
                                inBody = False
                #mimo funkci, mimo komentar, mimo retezec, mimo macro, mimo telo funkce
                else:
                    if c == ';' or c == ')' or c == '\n':
                        pass
                    elif c == '\"':
                        inString = True
                        state = 16
                    elif c == '\'':
                        inString = True
                        state = 17
                    elif c == '{':
                        inBody = True
                        state = 21
                        counter = 1
                    elif c == '#':
                        inMacro = True
                        state = 18
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
                        if lastChar.isspace():
                            word = c
                        else:
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
        f1.close()
        sys.exit(0)

def printDatabase(database):
    final = "<?xml version=\"1.0\" encoding=\"UTF-8\"?>"
    if database.parameters[3] != '-1':
        final+='\n'
    if os.path.isdir(database.parameters[1]):
        if database.parameters[1][-1] != '/':
            database.parameters[1] += '/'
        final += "<functions dir=\"" + database.parameters[1] +"\">"
        if database.parameters[3] != '-1':
            final+='\n'
    else:
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
    elif os.path.isfile(name):
        return 1
    elif name == "STDIN":
        return 2
    else:
        return 3

def recursive_gold(act_dir, database, relativepath):
    for fileordir in os.listdir(act_dir):
        if fnmatch.fnmatch(fileordir, '*'):
            temp = fileordir
            fileordir = act_dir + "/" + fileordir
            if os.path.isdir(fileordir):
                if relativepath == '-1':
                    relativepath = temp
                    temp2 = '-1'
                else:
                    temp2 = relativepath
                    relativepath = relativepath + "/" + temp
                recursive_gold(fileordir, database, relativepath)
                relativepath = temp2
            elif os.path.isfile(fileordir):
                ext = os.path.splitext(fileordir)[-1].lower()
                if ext == ".h":
                    if relativepath == '-1':
                        relativepath = temp
                        temp2 = '-1'
                    else:
                        temp2 = relativepath
                        relativepath = relativepath + "/" + temp
                    analysa(fileordir, database, relativepath)
                    relativepath = temp2

def analysa(file, database, relativepath):
    parserForFile.readByChar(file, database, relativepath)

####################################################################################
##########################Telo samotneho mocneho programu###########################
####################################################################################

helpArg = False
no_inline = False
no_duplicates = False
remove_whitespace = False

parser = argparse.ArgumentParser(add_help=False, allow_abbrev=False)

parser.add_argument('--input', action='append', dest='input', default=['STDIN'])
parser.add_argument('--output', action='append', dest='output', default=['STDOUT'])
parser.add_argument('--max-par', action='append', dest='max_par', default=['-1'])
parser.add_argument('--help', action='append_const', dest='help', default=['False'], const='True')
parser.add_argument('--pretty-xml', action='append', dest='pretty', default=['-1'], nargs='?')
parser.add_argument('--no-inline', action='append_const', dest='no_inline', default=['False'], const='True')
parser.add_argument('--no-duplicates', action='append_const', dest='no_duplicates', default=['False'], const='True')
parser.add_argument('--remove-whitespace', action='append_const', dest='remove_whitespace', default=['False'], const='True')

try: 
    results = parser.parse_args()
except SystemExit:
    sys.exit(1)

#input
if len(results.input) != 1 and len(results.input) != 2:
    sys.exit(1)
if len(results.input) == 2:
    results.input[0] = results.input[1]

#output
if len(results.output) != 1 and len(results.output) != 2:
    sys.exit(1)
if len(results.output) == 2:
    results.output[0] = results.output[1] 

#max_par
if len(results.max_par) != 1 and len(results.max_par) != 2:
    sys.exit(1)
if len(results.max_par) == 2:
    results.max_par[0] = results.max_par[1] 

#help
if len(results.help) != 1 and len(results.help) != 2:
    sys.exit(1)
if len(results.help) == 2:
    helpArg = True

#no_inline
if len(results.no_inline) != 1 and len(results.no_inline) != 2:
    sys.exit(1)
if len(results.no_inline) == 2:
    no_inline = True

#no_duplicates
if len(results.no_duplicates) != 1 and len(results.no_duplicates) != 2:
    sys.exit(1)
if len(results.no_duplicates) == 2:
    no_duplicates = True

#remove_whitespace
if len(results.remove_whitespace) != 1 and len(results.remove_whitespace) != 2:
    sys.exit(1)
if len(results.remove_whitespace) == 2:
    remove_whitespace = True

#pretty
if len(results.pretty) != 1 and len(results.pretty) != 2:
    sys.exit(1)
if len(results.pretty) == 2:
    results.pretty[0] = results.pretty[1]

if results.pretty[0] == None:
    results.pretty = '4'

if helpArg == True and ( results.input[0] != 'STDIN' or results.output[0] != 'STDOUT' or results.pretty[0] != '-1' or no_inline != False  or results.max_par[0] != '-1'  or no_duplicates != False or remove_whitespace != False ):
    sys.exit(1)

if results.input[0] == 'STDIN':
    results.input[0] = '.'

if os.path.isdir(results.output[0]):
    sys.exit(3)

if results.output[0] != 'STDOUT':
    if not os.access(os.path.dirname(results.output[0]), os.W_OK):
        sys.exit(3)

if helpArg == True:
    help_str = help_func()
    output_func(results.output[0], help_str)

database = database(helpArg, results.input[0], results.output[0], results.pretty[0], no_inline, results.max_par[0], no_duplicates, remove_whitespace) #inicializace databaze

path = os.path.abspath(results.input[0])

if fileordir(results.input[0]) == 0:
    recursive_gold(path, database, '-1')
elif fileordir(results.input[0]) == 1:
    analysa(path, database, results.input[0])
elif fileordir(results.input[0]) == 2:
    recursive_gold('.', database, '-1')
elif fileordir(results.input[0]) == 3:
    sys.exit(2)

printDatabase(database)
