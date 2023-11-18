all: main

main: lex.yy.c y.tab.c y.tab.h
	gcc lex.yy.c y.tab.c -o main

lex.yy.c: num.l
	flex num.l

y.tab.c: expr.y
	bison -dy expr.y

y.tab.h: expr.y
	bison -dy expr.y

clean: main lex.yy.c y.tab.c y.tab.h
	rm main lex.yy.c y.tab.c y.tab.h

zip: Makefile num.l expr.y
	zip main.zip Makefile num.l expr.y
