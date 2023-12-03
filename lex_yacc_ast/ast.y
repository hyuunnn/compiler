%{
#include <stdio.h>
#include <stdlib.h>
#include "ast.h"

int yylex();
int yyerror(char *s);
Node *Root;

Node *createNode(Tag kind, void *val, Node *son, Node *bro) {
    Node *newNode = malloc(sizeof(Node));
    if (newNode != NULL) {
        newNode->kind = kind;
        newNode->son = son;
        newNode->bro = bro;

        switch (kind) {
            case TINT: 
                newNode->ival = *((int *)val); 
                break;
            case TSYM:
            case TBOP:
            case TUOP: 
                newNode->sval = (char *)val; 
                break;
        }
    }
    return newNode;
}

Node *mkIntNode(int n) {
    return createNode(TINT, &n, NULL, NULL);
}

Node *mkSymNode(char *s) {
    return createNode(TSYM, s, NULL, NULL);
}

Node *mkBopNode(char *op, Node *left, Node *right) {
    return createNode(TBOP, op, left, right);
}

Node *mkUopNode(char *op, Node *son) {
    return createNode(TUOP, op, son, NULL);
}

void printTree(Node *root, int depth) {
    if (root == NULL) return;
    for (int i = 0; i < depth; i++) printf("    ");
    switch (root->kind) {
        case TINT: 
            printf("Int(%d)\n", root->ival); 
            break;
        case TSYM: 
            printf("Sym('%s')\n", root->sval); 
            break;
        case TBOP: 
            printf("Op(%s)\n", root->sval); 
            printTree(root->son, depth+1); 
            printTree(root->bro, depth+1); 
            break;
        case TUOP: 
            printf("Uop(%s)\n", root->sval); 
            printTree(root->son, depth+1); 
            break;
    }
}
%}

%union {
    char *sval;
    int ival;
    Node *pval;
}

%token <ival> NUM
%token <sval> ID '+' '-' '*' '/'
%type <pval> Exp Term Factor

%%
Prg: Exp { Root = $1; }
   ;
Exp: Exp '+' Term { $$ = mkBopNode("+", $1, $3); }
   | Exp '-' Term { $$ = mkBopNode("-", $1, $3); }
   | Term { $$ = $1; }
   ;
Term: Term '*' Factor { $$ = mkBopNode("*", $1, $3); }
    | Term '/' Factor { $$ = mkBopNode("/", $1, $3); }
    | Factor { $$ = $1; }
    ;
Factor: '(' Exp ')' { $$ = $2; }
    | '-' Factor { $$ = mkUopNode("-", $2); }
    | '+' Factor { $$ = mkUopNode("+", $2); }
    | NUM { $$ = mkIntNode($1); }
    | ID { $$ = mkSymNode($1); }
    ;
%%

int main() { yyparse(); printTree(Root, 0); }

int yyerror(char *msg) { fputs(msg, stderr); }
