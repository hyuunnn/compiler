%{
#include <stdio.h>
#include <stdlib.h>
#include "list.h"

int yylex();
int yyerror(char *s);

duo table;
%}

%union {
    str sval;
    int ival;
}

%token <ival> NUM
%token <sval> ID '+' '-' '*' '/'
%type <ival> Exp Term Factor

%%
Program: Program Statement '\n'
       | 
       ;
Statement: ID '=' Exp { entry(table, $1, $3); }
         | Exp { printf("(INT, %d)\n", $1); }
         ;
Exp: Exp '+' Term { $$ = $1 + $3; }
   | Exp '-' Term { $$ = $1 - $3; }
   | Term { $$ = $1; }
   ;
Term: Term '*' Factor { $$ = $1 * $3; }
    | Term '/' Factor { $$ = $1 / $3; }
    | Factor { $$ = $1; }
    ;
Factor: '(' Exp ')' { $$ = $2; }
      | '-' Factor { $$ = -$2; }
      | '+' Factor { $$ = $2; }
      | NUM { $$ = $1; }
      | ID { $$ = value(table, $1); }
      ;
%%

int main() { 
  table = (Pair *)malloc(sizeof(Pair));
  yyparse();
  }

int yyerror(char *msg) { fputs(msg, stderr); }
