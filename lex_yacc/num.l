%{
#include <stdlib.h>
#include "y.tab.h"
%}
%%
[0-9]+ {
  yylval = atoi(yytext); // yylval은 y.tab.h에 정의되어 있음
  return (NUMBER);
}
[ \t] ;
\+ return ('+');
\- return ('-');
\* return ('*');
\/ return ('/');
\( return ('(');
\) return (')');
\n return (0);
. { printf("'%c': illegal character\n", yytext[0]); exit(-1); }
%%
int yywrap() { return 1; }
