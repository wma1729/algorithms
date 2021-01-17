# Bitwise operators

Operator | What? | Comments
---------|-------|---------
<<       | Left shift. Shifts bit(s) to the left. | The vacated bits are zero filled. N << n = N * 2<sup>n<sup>.
\>>       | Right shift. Shifts bit(s) to the right. | For unsigned numbers, the vacated bits are zero filled. The behavior is dependent of the hardware platform in use for signed numbers. N >> n = N / 2<sup>n</sup>.
&        | And | 0 & 0 = 0<br>0 & 1 = 0<br>1 & 0 = 0<br>1 & 1 = 1
\|       | Or | 0 \| 0 = 0<br>0 \| 1 = 1<br>1 \| 0 = 1<br>1 \| 1 = 1
~        | One's complement | Flips every bit. 1 becomes 0 and vice-versa.
^        | Exclusive Or. Sets a 1 in each bit position where its operands have different bits, and 0 where they are the same. | 0 ^ 0 = 0<br>0 ^ 1 = 1<br>1 ^ 0 = 1<br>1 ^ 1 = 0

### How to determine if n<sup>th</sup> bit is set in an integer number, N?
> N & (1 << n)

### How to set n<sup>th</sup> bit in an integer number, N?
> N |= (1 << n)

### How to clear n<sup>th</sup> bit in an integer number, N?
> N &= ~(1 << n)

### How to form an integer number, N, with first n bits sets?
> N = ~(~0 << n)

### How many bits are required to store a number with n bits set?
> ceiling(log<sub>2</sub>n)
