import getopt
import sys

version = '1.0'
verbose = False
help = False
output_filename = 'default.out'


print ("ARGV      :", sys.argv[1:])

options, remainder = getopt.getopt(sys.argv[1:], ':', ['output=', 
                                                         'verbose',
                                                         'version=',
                                                         'help',
                                                         ])
print ("OPTIONS   :", options)

for opt, arg in options:
    if opt in '--output':
        output_filename = arg
    elif opt in '--verbose':
        verbose = True
    elif opt == '--version':
        version = arg
    elif opt == '--help':
        version = True

print ("VERSION   :", version)
print ("VERBOSE   :", verbose)
print ("OUTPUT    :", output_filename)
print ("HELP      :", help)
print ("REMAINING :", remainder)