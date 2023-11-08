## toy_lexer

### Usage

```console
$ gcc main.c -o main
$ ./main
```

### Example

```console
>> 123
123
>> "123"
'123'
>> "qwe"
'qwe'
>> "\"asdfasdf\""
'"asdfasdf"'
>> "qwe"123"\"qweqwe\""
'qwe'
123
'"qweqwe"'
>> ()
NIL
>> (          )
NIL
>> (1)
(1)
>> ( 1)
(1)
>> (1 )
(1)
>> (1 . NIL)
(1)
>> (1 . (   )) 
(1)
>> hihi
Error: Syntax Error
```
