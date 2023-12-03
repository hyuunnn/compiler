## lex_yacc_ast

### Usage

```console
$ make
$ ./main
```

### Example

```console
$ ./main
11 + 12 * 13
Op(+)
    Int(11)
    Op(*)
        Int(12)
        Int(13)

$ ./main
(1+x) / -12
Op(/)
    Op(+)
        Int(1)
        Sym('x')
    Uop(-)
        Int(12)
```
