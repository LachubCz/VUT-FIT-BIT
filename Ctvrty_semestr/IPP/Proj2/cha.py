import getopt, sys

help = False
input = 'STDIN'
output = 'STDOUT'
pretty = False
no_inline = False
max_par = 0
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
    exit(1)
