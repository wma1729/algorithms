# Bitwise operators

Operator | What? | Comments
---------|-------|---------
<<       | Left shift. Shifts bit(s) to the left. | The vacated bits are zero filled. N << n = N * 2<sup>n<sup>.
\>>       | Right shift. Shifts bit(s) to the right. | For unsigned numbers, the vacated bits are zero filled. The behavior is dependent of the hardware platform in use for signed numbers. N >> n = N / 2<sup>n</sup>.
&        | And | 0 & 0 = 0<br>0 & 1 = 0<br>1 & 0 = 0<br>1 & 1 = 1
\|       | Or | 0 \| 0 = 0<br>0 \| 1 = 1<br>1 \| 0 = 1<br>1 \| 1 = 1
~        | One's complement | Flips every bit. 1 becomes 0 and vice-versa.
^        | Exclusive Or. Sets a 1 in each bit position where its operands have different bits, and 0 where they are the same. | 0 ^ 0 = 0<br>0 ^ 1 = 1<br>1 ^ 0 = 1<br>1 ^ 1 = 0

### How to determine if *n<sup>th</sup>* bit is set in an integer number, *N*?
> N & (1 << n)

### How to set *n<sup>th</sup>* bit in an integer number, *N*?
> N |= (1 << n)

### How to clear *n<sup>th</sup>* bit in an integer number, *N*?
> N &= ~(1 << n)

### How to form an integer number, *N*, with first *n* bits sets?
> N = ~(~0 << n)

### How to swap two numbers, *N1* and *N2*, without a temporary variable?
> N1 = N1 ^ N2; N2 = N1 ^ N2; N1 = N1 ^2;

### How many bits are required to store a number with *n* bits set?
> ceiling(log<sub>2</sub>n)

### How to find *abs(N)*?
> Negative of a number, *n*, is one's complement of *n* + 1 i.e *-n = ~n + 1*. To get positive number from the negative number, we can do the reverse: subtract 1 from number and get its one's complement i.e. *n = ~(-n - 1)*. Alternatively, it can be done as following without caring if *n* is +ve or -ve:<br>
> mask = n >> 31; // Assuming n is 32-bit integer; mask will be all 0s for +ve n and all 1s for -ve number.<br>
> n = mask + n;   // For +ve n, mask is 0 so n remains unchanged. For -ve number, mask is -1 so we have subtracted 1 from the number.<br>
> n = n ^ mask;   // For +ve n, mask is 0 so n remains unchanged. For -ve number, mask is -1 so we get one's complement of n.<br>

### How to find if two integers, *n1* and *n2*, have opposite signs?
> (n1 ^ n2) < 0

### **n1 % n2** where n2 is a power of 2
> n1 % n2 = n1 & (n2 - 1)
