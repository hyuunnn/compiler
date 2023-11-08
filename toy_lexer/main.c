#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LENGTH 100

void syntaxError() {
  fputs("Error: Syntax Error", stderr);
  exit(-1);
}

void unknownDataTypeError() {
  fputs("Error: unknown data type", stderr);
  exit(-1);
}

typedef enum _Tag { TINT, TSTR, TNIL, TDUO } Tag;
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

int rddata(Data *result);

Data mkint(int i) { return (Data){.tag = TINT, .ival = i}; }

Data mkstr(str s) { return (Data){.tag = TSTR, .sval = s}; }

Data cons(Data a, Data b) {
  duo pair = (duo)malloc(sizeof(Pair));
  pair->d[0] = a;
  pair->d[1] = b;
  return (Data){.tag = TDUO, .pval = pair};
}

Data car(Data d) { return d.pval->d[0]; }

Data cdr(Data d) { return d.pval->d[1]; }

const Data nil = {.tag = TNIL, .pval = NULL};

char input[MAX_LENGTH + 1];  // 입력 받을 문자열
int pos = 0;                 // 입력 받은 문자열의 위치

Data *d = NULL;    // 입력 받은 데이터들을 담을 배열
int data_len = 0;  // 입력 받은 데이터의 개수

void removeString(char *src, char *dst) {
  while (*src) {
    if (*src != '"') {
      if (*src == '\\' && *(src + 1) == '"') {
        src++;
      }
      *dst = *src;
      dst++;
    }
    src++;
  }
  *dst = '\0';
}

char *sliceString(int start, int end) {
  int inputLength = strlen(input);
  if (start < 0 || start >= inputLength || end < start || end > inputLength) {
    return NULL;
  }

  int sliceLength = end - start;
  char *result = (char *)malloc(sliceLength + 1);

  strncpy(result, input + start, sliceLength);
  result[sliceLength] = '\0';

  removeString(result, result);
  return result;
}

char readChar() {
  if (strlen(input) < pos) {
    return 0;
  } else {
    pos++;
    return input[pos - 1];
  }
}

char peekChar() {
  if (strlen(input) < pos) {
    return 0;
  } else {
    return input[pos];
  }
}

char peekNextChar() {
  if (peekChar() == 0) {
    return 0;
  } else {
    return input[pos + 1];
  }
}

bool matchChar(char c) {
  if (peekChar() != c) return false;
  readChar();
  return true;
}

char *readString() {
  int prev_pos = pos;
  if (readChar() != '"') syntaxError();

  while (peekChar() != '"') {  // 다음 "가 나올 때까지 loop
    // \" 처리를 하지 않으면 위 loop 조건에 의해 종료됨
    if (peekChar() == '\\' && peekNextChar() == '"') {
      readChar();
    }
    // 문자열 끝까지 읽었는데 "가 없으면 에러
    if (strlen(input) <= pos) syntaxError();

    readChar();
  }

  if (readChar() != '"') syntaxError();

  return sliceString(prev_pos, pos);
}

char *readNumber() {
  int prev_pos = pos;

  while (isdigit(peekChar())) {
    readChar();
  }

  return sliceString(prev_pos, pos);
}

int skipWhitespace() {
  int skip_count = 0;
  while (peekChar() == ' ' || peekChar() == '\t' || peekChar() == '\n' ||
         peekChar() == '\r') {
    skip_count++;
    readChar();
  }
  return skip_count;
}

bool findString(char *str) {
  return strncmp(&input[pos], str, strlen(str)) == 0;
}

bool isNil() {
  int prev_pos = pos;
  int skip_count = 0;

  if (findString("NIL") || findString("nil")) {
    for (int i = 0; i < 3; i++) readChar();
    return true;
  }

  // 공백 Duo인지 확인
  if (readChar() == '(') {
    skipWhitespace();
    if (readChar() == ')') {
      return true;
    }
  }

  // 위 조건으로 return true 안하면 readChar 함수로 인해 변경된 pos 복구
  // 다음 조건인 Duo로 동작해야 하기 때문
  pos = prev_pos;
  return false;
}

Data readDuo() {
  // TODO: (1 . 2) 어떻게 처리할지 고민해보기
  if (readChar() != '(') syntaxError();  // '(' 체크

  Data firstData;
  if (rddata(&firstData) == -1) syntaxError();

  skipWhitespace();
  char nextChar = readChar();
  if (nextChar == ')') return cons(firstData, nil);  // 원소가 하나일 때 처리
  if (nextChar != '.') syntaxError();                // '.' 체크
  skipWhitespace();

  Data secondData;
  if (rddata(&secondData) == -1) syntaxError();

  skipWhitespace();
  if (readChar() != ')') syntaxError();  // ')' 체크

  return cons(firstData, secondData);
}

int rddata(Data *result) {
  skipWhitespace();

  if (peekChar() == '"') {
    *result = mkstr(readString());
  } else if (isdigit(peekChar())) {
    int num = atoi(readNumber());
    *result = mkint(num);
  } else if (isNil()) {
    *result = nil;
  } else if (peekChar() == '(') {
    *result = readDuo();
  } else {
    return -1;
  }

  return 0;
}

int prdata(Data d) {
  switch (d.tag) {
    case TINT:
      printf("%d", d.ival);
      break;
    case TSTR:
      printf("'%s'", d.sval);
      break;
    case TNIL:
      printf("NIL");
      break;
    case TDUO:
      printf("(");
      prdata(car(d));
      while (cdr(d).tag != TNIL) {
        printf(" ");
        d = cdr(d);
        prdata(car(d));
      }
      printf(")");
      break;
    default:
      return -1;
  }
  return 0;
}

void freeData(Data d) {
  if (d.tag == TSTR && d.sval != NULL) {
    free(d.sval);
  } else if (d.tag == TDUO && d.pval != NULL) {
    freeData(car(d));
    freeData(cdr(d));
    free(d.pval);
  }
}

void reset() {
  for (int i = 0; i < data_len; i++) {
    freeData(d[i]);
  }
  free(d);
  d = NULL;
  pos = 0;
  data_len = 0;
}

int main() {
  while (fgets(input, MAX_LENGTH + 1, stdin) == input) {
    // fputs(input, stdout);
    input[strlen(input) - 1] = '\0';  // fgets는 개행 문자까지 받아오기 때문에
                                      // 개행 문자를 널 문자로 바꿔줌

    while (strlen(input) > pos) {
      // while (peekChar() != '\0') {
      Data new_data;
      if (rddata(&new_data) == -1) syntaxError();
      d = (Data *)realloc(d, (data_len + 1) * sizeof(Data));
      d[data_len++] = new_data;
    }

    for (int i = 0; i < data_len; i++) {
      if (prdata(d[i]) == -1) unknownDataTypeError();
      printf("\n");
    }

    reset();
  }

  reset();
  return 0;
}
