all: testsymtablelist testsymtablehash

testsymtablelist: symtablelist.o testsymtable.o
	gcc217 -g symtablelist.o testsymtable.o -o testsymtablelist

testsymtablehash: symtablehash.o testsymtable.o
	gcc217 -g symtablehash.o testsymtable.o -o testsymtablehash

symtablelist.o: symtablelist.c
	gcc217 -g -c symtablelist.c

symtablehash.o: symtablehash.c
	gcc217 -g -c symtablehash.c