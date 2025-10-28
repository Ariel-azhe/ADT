all: testsymtablelist testsymtablehash

testsymtablelist: symtablelist.o testsymtable.o
	gcc217 symtablelist.o testsymtable.o 0o testsymtablelist

testsymtablehash: symtablehash.o testsymtable.o
	gcc217 symtablehash.o testsymtable.o -o testsymtablehash

symtablelist.o: symtablelist.c
	gcc217 -c symtablelist.c

symtablehash.o: symtablehash.c
	gcc217 -c symtablehash.c