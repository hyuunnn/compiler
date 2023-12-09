#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "list.h"

Data mkint(int i) { return (Data){.tag = TINT, .ival = i}; }

Data mkstr(str s) { return (Data){.tag = TSTR, .sval = s}; }

duo mkVar(str s, int i) {
  duo p = (duo)malloc(sizeof(Pair));
  p->d[0] = mkstr(s);
  p->d[1] = mkint(i);
  return p;
}

duo car(duo p) { return p->d[0].pval; }

duo cdr(duo p) { return p->d[1].pval; }

void setCar(duo p, duo q) {
  p->d[0].pval = q;
}

void setCdr(duo p, duo q) {
  p->d[1].pval = q;
}

str pairVar(duo p) {
  return car(p)->d[0].sval;
}

int pairInt(duo p) {
  return car(p)->d[1].ival;
}

duo findVar(duo p, str s) {
  while (car(p) != NULL) {
    if (strcmp(pairVar(p), s) == 0) {
      return p;
    }
    p = cdr(p);
  }
  return p;
}

int value(duo t, str s) {
    duo p = findVar(t, s);
    return car(p) != NULL ? pairInt(p) : 0;
}

void entry(duo t, str s, int i) {
    duo p = findVar(t, s);

    // 변수가 이미 존재하면 값만 변경한다.
    if (car(p) != NULL) {
        p->d[0].pval->d[1] = mkint(i);
        return;
    }

    // 변수가 존재하지 않으면 새로 만든다.
    setCar(p, mkVar(s, i));
    setCdr(p, (duo)malloc(sizeof(Pair)));
}
