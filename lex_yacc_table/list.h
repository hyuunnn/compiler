#ifndef _EXPR_H_
#define _EXPR_H_

typedef enum { TNIL, TINT, TSTR, TDUO } Tag;
typedef char *str;
typedef struct _Pair *duo;

typedef struct _Data {
    Tag tag;
    union {
        int ival;
        str sval;
        duo pval;
    };
} Data;

typedef struct _Pair {
    Data d[2];
} Pair;

Data mkint(int i);
Data mkstr(str s);
duo mkVar(str s, int i);
duo car(duo p);
duo cdr(duo p);
void setCar(duo p, duo q);
void setCdr(duo p, duo q);
str pairVar(duo p);
int pairInt(duo p);
duo findVar(duo p, str s);
int value(duo t, str s);
void entry(duo t, str s, int i);

#endif
