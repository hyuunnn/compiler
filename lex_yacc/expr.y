%{
#include <stdio.h>
int yylex();
int yyerror(char *s);
extern int yylval; // yylval 변수를 외부에서 참조

// 암시적인(implicit) 방법
%}
%token NUMBER
%%
Exp : Exp '+' Term { $$ = $1 + $3; 
                     printf("E(%d) -> E(%d) + T(%d)\n", $$, $1, $3); }
    | Exp '-' Term { $$ = $1 - $3;
                     printf("E(%d) -> E(%d) - T(%d)\n", $$, $1, $3); }
    | Term { $$ = $1;
             printf("E(%d) -> T(%d)\n", $$, $1); }
    ;
Term : Term '*' Factor { $$ = $1 * $3;
                         printf("T(%d) -> T(%d) * F(%d)\n", $$, $1, $3); }
     | Term '/' Factor { if ($3 == 0) yyerror("divide by zero");
                         else $$ = $1 / $3;
                         printf("T(%d) -> T(%d) / F(%d)\n", $$, $1, $3); }
     | Factor { $$ = $1; 
                printf("T(%d) -> F(%d)\n", $$, $1); }
     ;
Factor: NUMBER { $$ = $1; 
                 printf("F(%d) -> n(%d)\n", $$, $1); }
      ;
%%
int main() {
  yyparse();
}

int yyerror(char *s) {
  printf("%s\n", s);
}
