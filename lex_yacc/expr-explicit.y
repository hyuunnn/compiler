%{
#include <stdio.h>
int yylex();
int yyerror(char *s);
extern int yylval; // yylval 변수를 외부에서 참조

// 명시적인(explicit) 방법
%}
%token NUMBER
%left '+' '-'
%left '*' '/'
%%
Exp : Exp '+' Exp { $$ = $1 + $3; 
                    printf("E(%d) -> E(%d) + E(%d)\n", $$, $1, $3); }
    | Exp '-' Exp { $$ = $1 - $3;
                    printf("E(%d) -> E(%d) - E(%d)\n", $$, $1, $3); }
    | Exp '*' Exp { $$ = $1 * $3; 
                    printf("E(%d) -> E(%d) * E(%d)\n", $$, $1, $3); }
    | Exp '/' Exp { if ($3 == 0) yyerror("divide by zero");
                    else $$ = $1 / $3;
                    printf("E(%d) -> E(%d) / E(%d)\n", $$, $1, $3); }
    | NUMBER { $$ = $1; 
               printf("E(%d) -> n(%d)\n", $$, $1); }
    ;
%%
int main() {
  yyparse();
}

int yyerror(char *s) {
  printf("%s\n", s);
}
