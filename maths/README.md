# Prime numbers
A prime number is a number that is only divisible by 1 and itself (or it cannot be made by multiplying any other whole numbers).
For example: 1, 2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31. The numbers that are not prime are called composite numbers. Many modern texts
do not consider 1 as either a prime or a composite number.

## How to determine if a number, N, is prime?
If N is not divisible by any number in the range [2, √N), it is prime.
For bigger numbers, this could be a lot of calculations. There is a rule that comes handy.
It is called 6k ± 1 rule. As per this rule, all prime numbers follow this rule, starting from 5 (though not perfectly).

k | 6k - 1 | 6k + 1 | Prime?
--|--------|--------|-------
1 | 5 | 7 | Yes
2 | 11 | 13 | Yes
3 | 17 | 19 | Yes
4 | 23 | 25 | Yes/No
5 | 29 | 31 | Yes
6 | 35 | 37 | No/Yes
7 | 41 | 43 | Yes
8 | 47 | 49 | Yes/No
 
Following this rule, we need not check all the numbers in [2, √N) range.
We can only check [2, 3, c1, c2, …, √N) where candidates c1, c2, … are obtained following 6k ± 1 rule. 

# Factors of a number, N
The factors of a number are numbers, m and n, such that m x n = N. There can be many factors of a number.
For example, the factors of 12 are:

> F(12):  1, 2, 3, 4, 6, 12 as 1 x 12 = 12, 2 x 6 = 12, 3 x 4 = 12


## How to find all factors of a number, N?
* 1 and N are alway factors as 1 x n1 = N for n1 = N.
* Try 2 to see if 2 x n2 = N. If n2 exists, 2 and n2 are factors.
* Try 3 to see if 3 x n3 = N. If n3 exists, 3 and n3 are factors,
* Continue this upto √N. Why √N? Think!

All the m x n pairs generated whose product is N gives us the factors of N.

# Prime Factors of a number, N
The prime factors of a number are factors only including prime numbers. For example:

> PF(12): 1, 2, 2, 3 or 1 x 2<sup>2</sup> x 3<br>
> PF(39): 1, 3, 13 or 1 x 3 x 13<br>
> PF(98): 1, 2, 7, 7 or 1 x 2 x 7<sup>2</sup><br>

## How to find prime factors of a number, N?
* 1 is always include (or excluded if you do not consider 1 as a prime number).
* As long as N can be divided by 2, include 2 as many times.
* As long as N can be divided by 3, include 3 as many times.
* Continue this for all prime numbers upto √N.
* If N is a prime number, include it as well.

# Common Factors of two or more numbers
The common factors of two or more numbers are common factors in those numbers. For example:

> F(15): 1, 3, 5, 15<br>
> F(20): 1, 2, 4, 5, 10, 20<br>
> CF(15, 20): 1, 5<br>

> F(16): 1, 2, 4, 8, 16<br>
> F(24): 1, 2, 3, 4, 6, 8, 12, 24<br>
> CF(16, 24): 1, 2, 4, 8<br>

# Highest Common Factor
Amongst the common factors of two or more numbers, the highest number is the highest common factor. In the above example:

> HCF(15, 20): 5<br>
> HFC(16, 24): 8<br>

It is commonly known by the following names:
* Highest Common Factor (HCF)
* Highest Common Divisor (HCD)
* Greatest Common Factor (GCF)
* Greatest Common Divisor (GCD)
