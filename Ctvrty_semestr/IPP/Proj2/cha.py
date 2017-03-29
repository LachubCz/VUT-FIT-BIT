# @author Petr Buchal(xbucha02)

import getopt, sys
#import StringIO

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

#print ("ARGV      :", sys.argv[1:])

options, remainder = getopt.getopt(sys.argv[1:], ':', ['input=', 
                                                         'output',
                                                         'max-par=',
                                                         'help',
                                                         'pretty',
                                                         'no-inline',
                                                         'no-duplicates',
                                                         'remove-whitespace',
                                                         ])
#print ("OPTIONS   :", options)

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

####################################################################################
######################################Funkce########################################
####################################################################################

def output_func(output, final):
    f1 = open(output, 'w')
    f1.write(final)

def help_func():
    help_str = StringIO()

    help_str.write('--help ## Viz spolecne zadani vsech uloh.\n\n')

    help_str.write('--input=fileordir ## Zadany vstupni soubor nebo adresar se zdrojovym kodem v jazyce C.\n')
    help_str.write('Predpokladejte, ze soubory budou v kodovani UTF-8. Je-li zadana cesta k adresari, tak jsou\n')
    help_str.write('postupne analyzovany vsechny soubory s priponou .h v tomto adresari a jeho podadresarich.\n')
    help_str.write('Pokud je zadana primo cesta k souboru (nikoliv k adresari), tak priponu souboru nekontrolujte.\n')
    help_str.write('Pokud nebude tento parametr zadan, tak se analyzuji vsechny hlavickove soubory (opet pouze\n')
    help_str.write('s priponou .h) z aktualniho adresare a vsech jeho podadresaru.\n\n')

    help_str.write('--output=filename ## Zadany vystupni soubor ve formatu XML v kodovani UTF-8 (presny\n')
    help_str.write('format viz nize). Pokud tento parametr neni zadan, tak dojde k vypsani vysledku na standardni vystup.\n\n')

    help_str.write('--pretty-xml=k ## Skript zformatuje vysledny XML dokument tak, ze (1) kazde nove zanoreni\n')
    help_str.write('bude odsazeno o k mezer oproti predchozimu a (2) XML hlavicka bude od korenoveho elementu\n')
    help_str.write('oddelena znakem noveho radku. Pokud k neni zadano, tak se pouzije hodnota 4. Pokud tento\n')
    help_str.write('parametr nebyl zadan, tak se neodsazuje (ani XML hlavicka od korenoveho elementu).\n\n')

    help_str.write('--no-inline ## Skript preskoci funkce deklarovane se specifikatorem inline.\n\n')

    help_str.write('--max-par=n ## Skript bude brat v uvahu pouze funkce, ktere maji n ci mene parametru (n musi\n')
    help_str.write('byt vzdy zadano). U funkci, ktere maji promenny pocet parametru, pocitejte pouze s fixnimi parametry.\n\n')   

    help_str.write('--no-duplicates ## Pokud se v souboru vyskytne vice funkci se stejnym jmenem (napr.\n')
    help_str.write('deklarace funkce a pozdeji jeji definice), tak se do vysledneho XML souboru ulozi pouze prvni\n')
    help_str.write('z nich (uvazujte pruchod souborem shora dolu). Pokud tento parametr neni zadan, tak se do\n')
    help_str.write('vysledneho XML souboru ulozi vsechny vyskyty funkce se stejnym jmenem.\n\n')

    help_str.write('--remove-whitespace ## Pri pouziti tohoto parametru skript odstrani z obsahu atributu\n')
    help_str.write('rettype a type (viz nize) vsechny prebytecne mezery.\n\n')   

    return help_str
    
