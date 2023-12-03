all: main

main: lex.yy.c y.tab.c y.tab.h
	gcc lex.yy.c y.tab.c -o main

lex.yy.c: token.l
	flex token.l

y.tab.c: ast.y
	bison -dy ast.y

y.tab.h: ast.y
	bison -dy ast.y

clean: main lex.yy.c y.tab.c y.tab.h
	rm main lex.yy.c y.tab.c y.tab.h

zip: Makefile token.l ast.y
	zip main.zip Makefile token.l ast.y
