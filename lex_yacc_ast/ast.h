typedef enum { TINT, TSYM , TBOP, TUOP } Tag;

typedef struct _Node {
    Tag kind;
    union {
        int ival;
        char *sval;
    };

    struct _Node *son, *bro;
} Node;
