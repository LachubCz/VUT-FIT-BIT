
CC=gcc
CFLAGS=-Wall  -std=c11 -pedantic -g  

SRC = $(wildcard *.c)
OBJ =$(SRC:.c=.o) 

SOURCES=token.c scanner.c scanner_test.c
OBJECTS=$(SOURCSE:=.o)

default: ifj

%.o: %.c
	$(CC) -c $(CFLAGS) $^ -o $@

scanner_test: token.o scanner.o scanner_test.o string.o
	$(CC) $(CFLAGS) -o $@ $^

scanner_drv: token.o scanner.o scanner_drv.o string.o
	$(CC) $(CFLAGS) -o $@ $^

stack_test: stack_ints_test.o stack_ints.o
	$(CC) $(CFLAGS) -o $@ $^

string_test: string.o string_test.o
	$(CC) $(CFLAGS) -o $@ $^

parser_test: parser.o scanner.o main.o string.o token.o stack_ints.o
	$(CC) $(CFLAGS) -o $@ $^

ifj: parser.o scanner.o main.o string.o stack_ints.o token.o ilist.o exp.o exp_structures.o stack_symbol_table.o symbol_table.o semantic.o pre_parser.c
	$(CC) $(CFLAGS) -o $@ $^
test: ifj	
	bash test.bash

run_scanner_test: scanner_test
	./scanner_test 0 tests/scanner_basic.src
	./scanner_test 1 tests/scanner_program.src
	./scanner_test 2 tests/scanner_ifs_program.src
	./scanner_test 3 tests/scanner_variables.src
	./scanner_test 4 tests/scanner_loops.src
clean:
	rm -rf *.o $(PROGS) *_test pdf ifj
