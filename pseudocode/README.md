# Pseudocode

## Grammar
```
constant:
  null
  boolean-constant                                                    # 1 byte : false - true
  character-constant                                                  # 1 byte : -128 - 127 (ASCII code set)
  integer-constant                                                    # 8 bytes : -9223372036854775808 - 9223372036854775807
  real-constant                                                       # 8 bytes : -1.79769e+308 - 1.79769e+308

identifier:
  [_a-zA-Z][_a-zA-Z0-9]*

identifier-list:
  identifier
  identifier-list , identifier                                        # x, y

type-specifier:
  boolean
  character
  integer
  real
  array-specifier
  record-specifier

type-specifier-list:
  type-specifier
  type-specifier-list , type-specifier

array-specifier:
  array alias ( type-specifier, constant-expression )                       # array ( real, 10 ) - one-dimensional
  array alias ( type-specifier, constant-expression, constant-expression )  # array ( integer, 10, 10 ) - two-dimensional
                                                 
record-specifier:
  record alias ( declaration-list )                                         # record Point ( integer : x, y; )

alias:
  ''
  identifier

declaration-list:
  declaration
  declaration-list declaration

declaration:
  type-specifer : identifier-list ;                                   # boolean : flag ;
                                                                      # integer : i, j ;
                                                                      # record ( integer : x, y ; ) : point_1
                                                                      # Point : point_1

alg-declarator:                                                       #
  algorithm identifier alg-input alg-output alg-body                  # algorithm shuffle
                                                                      # input: array ( integer, 10 ) : sorted_array;
alg-input:                                                            # output: array ( integer, 10 ) ;
  input : declaration-list                                            # begin
                                                                      # ...
alg-output:                                                           # ...
  output: type-specifier-list ;                                       # ...
                                                                      # ...
alg-body:                                                             # ...
  begin declaration-list statements end                               # end
```
