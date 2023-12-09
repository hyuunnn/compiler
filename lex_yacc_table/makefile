all: main

main: list.c lex.yy.c y.tab.c y.tab.h
	gcc list.c lex.yy.c y.tab.c -o main

lex.yy.c: lex.l
	flex lex.l

y.tab.c: grammar.y
	bison -dy grammar.y

y.tab.h: grammar.y
	bison -dy grammar.y

clean: main lex.yy.c y.tab.c y.tab.h
	rm main lex.yy.c y.tab.c y.tab.h

zip: Makefile lex.l grammar.y
	zip main.zip Makefile lex.l grammar.y
