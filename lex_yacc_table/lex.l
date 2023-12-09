%{
#include <stdlib.h>
#include "list.h"
#include "y.tab.h"
%}
%%
[0-9]+ {
  yylval.ival = atoi(yytext); // yylval은 y.tab.h에 정의되어 있음
  return (NUM);
}
[a-zA-Z]+ {
  yylval.sval = strdup(yytext);
  return (ID);
}
\+ return ('+');
\- return ('-');
\* return ('*');
\/ return ('/');
\( return ('(');
\) return (')');
\= return ('=');
\n return ('\n');
[ \t\r] ;
. { printf("'%c': illegal character\n", yytext[0]); exit(-1); }
%%
int yywrap() { return 1; }
