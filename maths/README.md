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
The factors of a number are numbers, m and n, such that m * n = N. There can be many factors of a number.
For example, the factors of 12 are:

> F(12):  1, 2, 3, 4, 6, 12 as 1 * 12 = 12, 2 * 6 = 12, 3 * 4 = 12


## How to find all factors of a number, N?
* 1 and N are alway factors as 1 * n1 = N for n1 = N.
* Try 2 to see if 2 * n2 = N. If n2 exists, 2 and n2 are factors.
* Try 3 to see if 3 * n3 = N. If n3 exists, 3 and n3 are factors,
* Continue this upto √N. Why √N? Think!

All the m * n pairs generated whose product is N gives us the factors of N.

# Prime Factors of a number, N
The prime factors of a number are factors only including prime numbers. For example:

> PF(12): 1, 2, 2, 3 or 1 * 2<sup>2</sup> * 3<br>
> PF(39): 1, 3, 13 or 1 * 3 * 13<br>
> PF(98): 1, 2, 7, 7 or 1 * 2 * 7<sup>2</sup><br>

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

## How to find HCF of two numbers, m and n
### Method 1 (using simple division)
```C++
// Find the smaller of the two numbers
long gcd_v1(long m, long)
{
    long i = (m < n) ? m : n;

    for (; i > 1; --i) {
        /*
         * We are done as soon as we find a number that can
         * divide both m and n with no remainder.
         */
        if (((m % i) == 0) && ((n % i) == 0))
            break;
    }

    return i;
}
```
### Method 2 (using factors)
```C++
long gcd_v2(long m, long)
{
    // Get all factors for m and n.
    std::vector<long> f_m = std::move(factors(m));
    std::vector<long> f_n = std::move(factors(n));

    std::vector<long>::reverse_iterator m_it = f_m.rbegin();
    std::vector<long>::reverse_iterator n_it = f_n.rbegin();

    // Find the highest common factors
    while ((m_it != f_m.rend()) && (n_it != f_n.rend())) {
        if (*m_it > *n_it)
            ++m_it;
        else if (*m_it < *n_it)
            ++n_it;
        else /* if (*m_it == *n_it) */
            break;
    }

    return (m_it != f_m.rend()) ? *m_it : 1;
}
```
### Method 3 (using prime factors)
```C++
long gcd_v3(long m, long)
{
    // Get all prime factors for m and n.
    std::vector<long> f_m = std::move(prime_factors(m));
    std::vector<long> f_n = std::move(prime_factors(n));

    long result = 1;

    std::vector<long>::const_iterator m_it = f_m.begin();
    std::vector<long>::const_iterator n_it = f_n.begin();

    // We need the product of all common prime factors.
    while ((m_it != f_m.end()) && (n_it != f_n.end())) {
        if (*m_it < *n_it) {
            ++m_it;
        } else if (*m_it > *n_it) {
            ++n_it;
        } else /* if (*m_it == *n_it) */ {
            result *= *m_it;
            ++m_it;
            ++n_it;
        }
    }

    return result;
}
```
### Method 4
Euclid noticed that if a number k divides both m and n, then it divides their difference. What this means is:
> HCF(m, n) == HCF(m - n, n) for m > n.
```C++
long gcd_v4(long m, long)
{
    while (m != n) {
        if (m > n)
            m = (m - n);
        else
            n = (n - m);
    }

    return m;
}
```
### Method 5 (Optimized Euclid's algorithm)
Assume the two numbers, m and n, are 10000 and 24. We will be subtracting 24 from 10000 for a long time. The following algorithm replaces subtraction with division.
> HCF(m, n) == HCF(m % n, n) for m > n.
```C++
long gcd_v5(long m, long)
{
    long lo = std::min(m, n);
    long hi = std::max(m, n);
    long r = 1;

    while (r > 0) {
        r = hi % lo;
        hi = lo;
        lo = r;
    }

    return hi;
}
```

# Multiples of a number, N
The multiples of a number, N, are numbers obtained by multiplying N by other non-zero numbers.
> M(3): 3, 6, 9, 12, 15, 18, 21, 24, 27, 30, 33, ...<br>
> M(5): 5, 10, 15, 20, 25, 30, 35, ...<br>

The common multiples of two or more numbers are the multiples common to both the numbers.
> CM(3, 5): 15, 30, ...<br>

## Least Common Multiple
The least common multiplier is the smallest of the common multiples.
> LCM(3, 5): 15<br>

## How to find LCM of two numbers, m and n
### Method 1 (using simple multiplication)
```C++
long lcm_v1(long m, long n)
{
    long mm = m, mi = 2;
    long nn = n, ni = 2;

    while (mm != nn) {
        if (mm < nn)
            mm = m * mi++;
        else
            nn = n * ni++;
    }

    return mm; // or nn
}
```

### Method 2 (using prime factors)
* Get the prime factors of m and n
* Multiply each factor the greatest number of times it occurs in the prime factors of each number.
Examples:
> PF(9): 3, 3 [3 has 2 occurrences]<br>
> PF(21): 3, 7 [3 has 1 and 7 has 1 occurrence each]<br>
> LCM(9, 21): 3 * 3 * 7 = 63<br>
> <br>
> PF(36): 2, 2, 3, 3 [2 has 2 and 3 has 2 occurences]<br>
> PF(48): 2, 2, 2, 2, 3 [2 has 4 occurrences and 3 has 1 occurence]<br>
> LCM(36, 48): 2 * 2 * 2 * 2 * 3 * 3 = 144<br>

```C++
long lcm_v2(long m, long n)
{
    // Get all prime factors for m and n.
    std::vector<long> f_m = std::move(prime_factors(m));
    std::vector<long> f_n = std::move(prime_factors(n));

    std::vector<long>::iterator m_it = f_m.begin();
    std::vector<long>::iterator n_it = f_n.begin();

    while ((m_it != f_m.end()) && (n_it != f_n.end())){
        if (*m_it < *n_it) {
            ++m_it;
        } else if (*m_it > *n_it) {
            ++n_it;
        } else {
            /*
             * Set it to 1 in one vector. It doesn't
             * matter which one we chose.
             */
            *m_it = 1;
            ++m_it;
            ++n_it;
        }
    }

    long result = 1;
    for (auto x : f_m)
        result *= x;
    for (auto x : f_n)
        result *= x;

    return result;
}
```
### Method 3 (using GCD)
> m * n = LCM(m, n) * GCD(m, n)<br>
> LCM(m, n) = (m * n) / GCD(m, n)<br>

We can calculate GCD really fast using optimized Euclid's algorithm.
```C++
long lcm_v3(long m, long n)
{
    return (m * n) / gcd(m, n);
}
```

# Properties of power (exponents)
Expression | Equivalent | Comments
-----------|------------|---------
b<sup>0</sup>               | 1                             |
b<sup>1</sup>               | b                             |
(a * b)<sup>x</sup>         | a<sup>x</sup> * b<sup>x</sup> |
b<sup>x + y</sup>           | b<sup>x</sup> * b<sup>y</sup> |
b<sup>x - y</sup>           | b<sup>x</sup> / b<sup>y</sup> |
(b<sup>x</sup>)<sup>y</sup> | b<sup>x * y</sup>             |
b<sup>-x</sup>              | 1 / b<sup>x</sup>             | b<sup>-x</sup> = b<sup>0 - x</sup> = b<sup>0</sup> / b<sub>x</sup>
b<sup>1/x</sup>             | <sup>x</sup>&#8730;b          | (b<sup>1/x</sup>)<sup>x</sup> = (<sup>x</sup>&#8730;b)<sup>x</sup><br>b<sup>x/x</sup> = b<br>b<sup>1</sup> = b
b<sup>x/y</sup>             | (<sup>y</sup>&#8730;b)<sup>x</sup> |

## How to determine n<sup>k</sup> for two positive numbers?
### Method 1 (Using simple multiplication)
We know that n<sup>k</sup> = n * n<sup>k - 1</sup>. This method involves k - 1 multiplication operations. This can be too expensive for larger values of k.
```C++
long
power_v1(int n, int k)
{
    if (k == 0)
        return 1;

    long result = n;

    for (int i = 1; i < k; ++i)
        result *= n;

    return result;
}
```

### Method 2 (Using one of the properties of power).
We know that n<sup>k</sup> = (n<sup>k / 2</sup>)<sup>2</sup> when k is even. Using this, we can keep reducing the value of k until k is 1 or 0. Let's see an example. We want to compute 2<sup>32</sup>. We can break it as following:
> 2<sup>32</sup> = 2<sup>16</sup> * 2<sup>16</sup><br>
> 2<sup>16</sup> = 2<sup>8</sup> * 2<sup>8</sup><br>
> 2<sup>8</sup> = 2<sup>4</sup> * 2<sup>4</sup><br>
> 2<sup>4</sup> = 2<sup>2</sup> * 2<sup>2</sup><br>
> 2<sup>2</sup> = 2<sup>1</sup> * 2<sup>1</sup><br>

How do we have cases when k is odd? Simple. n<sup>k</sup> = n * (n<sup>(k - 1) / 2</sup>)<sup>2</sup>. With this approach, the number of multiplications are at most 2 * log<sub>2</sub>k.
```C++
long
power_v2(int n, int k)
{
    if (k == 0)
        return 1;

    if (k == 1)
        return n;

    long r = power_v2(n, k / 2);

    if ((k % 2) == 0) {
        // even
        return r * r;
    } else {
        // odd
        return n * r * r;
    }
}
```

# Logarithms
x = b<sup>y</sup> is same as log<sub>b</sub>(x) = y where b is the base, y is the exponent or power.
> log<sub>b</sub>(x) = y means the power to which b must be raised to get x.

## Properties of logarithms

1. log<sub>b</sub>(1) = 0 for any b.
2. log<sub>b</sub>(b) = 1 for any b.
3. log<sub>b</sub>(b<sup>x</sup>) = x
4. log<sub>b</sub>(m * n) = log<sub>b</sub>(m) + log<sub>b</sub>(n)
5. log<sub>b</sub>(m / n) = log<sub>b</sub>(m) - log<sub>b</sub>(n)
6. log<sub>b</sub>(m<sup>n</sup>) = n * log<sub>b</sub>(m)

# Polynomial Functions
> y = f(x) = a<sub>n</sub>x<sup>n</sup> + a<sub>n - 1</sub>x<sup>n - 1</sup> + ... + a<sub>2</sub>x<sup>2</sup> + a<sub>1</sub>x + a<sub>0</sub><br>

The above is a polynomial function of n-degrees. x is an indendent variable and y, being dependent on x, is the dependent variable. a<sub>0</sub> is the constant (often denoted as c and called y-intercept).

Function | Description
---------|------------
Constant function | A polynomial function when x = 0 or f(0) = c. Represents a point in a x-y axis graph.
Linear function   | A polynomial function of degree 1 (n = 1).<br>f(x) = ax + c<br> The graph of a linear function is a straight line. a is the coefficient of the variable x (also called the slope/gradient). It is the rate of change of the dependent variable.
Quadratic function | A polynomial function of degree 2 (n = 2).<br>f(x) = ax<sup>2</sup> + bx + c<br>The graph of a quadratic equation is a curve where y increases rapidly in proportion to x. The coefficient of x dilates (stretches or compresses) the graph along the y-axis.
Cubic function | A polynomial of degree 3 (n = 3).<br>ax<sup>3</sup> + bx<sup>2</sup> + cx + d
Exponential function | y = a * b<sup>x</sup><br>b is the base. y grows exponentially as x increases. If 0 < b < 1, y decreases with increase in x. The graph is a curve.
Logarithmic function | y = log<sub>b</sub>(x)<br> The graph is a slope but y increase very slowly with increase of x.
Factorial | y = x! = x * (x -1) * (x - 2) * ... * 3 * 2 * 1<br> The graph is a curve. Very rapid growth.

x | 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 | 9 | 10
--|---|---|---|---|---|---|---|---|---|----
x<sup>2</sup> | 1 | 4 | 9 | 16 | 25 | 36 | 49 | 64 | 81 | 100
x<sup>3</sup> | 1 | 8 | 27 | 64 | 125 | 216 | 343 | 512 | 729 | 1000
2<sup>x</sup> | 2 | 4 | 8 | 16 | 32 | 64 | 128 | 256 | 512 | 1024
x! | 1 | 2 | 6 | 24 | 120 | 720 | 5040 | 40320 | 36288 | 362880
 log<sub>2</sub>(x) | 0 | 1 | 1.585 | 2 | 2.32 | 2.585 | 2.8 | 3 | 3.169 | 3.322

## Algorithm to solve polynomial function

Lets's assume that the polynomial function is:
> a<sub>3</sub>x<sup>3</sup> + a<sub>2</sub>x<sup>2</sup> + a<sub>1</sub>x + a<sub>0</sub><br>

The function to solve the polynomial function is passed two arguments:
- The variable x
- A vector of coefficients: a<sub>3</sub>, a<sub>2</sub>, a<sub>1</sub>, a<sub>0</sub>

```C++
long
polynomial_v1(int x, const vector<int> &a)
{
	if (a.empty())
		return 0;

	if (a.size() == 1)
		return a[0];

	long result = a[0];

	for (size_t i = 1; i < a.size(); ++i) {
		long var = x;
		for (size_t j = i; j > 1; --j)
			var *= x;

		result += (a[i] * var);
	}

	return result;
}
```
This function involves *n * (n + 1) / 2* multiplications and *n* additions where *n* is the degree of polynomial function.

```C++
long
polynomial_v2(int x, const vector<int> &a)
{
	if (a.empty())
		return 0;

	if (a.size() == 1)
		return a[0];

	long result = a[0];
	long var = x;

	for (size_t i = 1; i < a.size(); ++i) {
		result += (a[i] * var);
		var = var * x;
	}

	return result;
}
```
This version stores the values of the variable x in *var* and recalculates it for the next iteration with just 1 multiplication. With this optimization, we have reduced the number of multiplications to *2n*. The number of additions remain the same, *n*.

Can we do better?<br>
> a<sub>3</sub>x<sup>3</sup> + a<sub>2</sub>x<sup>2</sup> + a<sub>1</sub>x + a<sub>0</sub><br>
> ((a<sub>3</sub>x + a<sub>2</sub>)x + a<sub>1</sub>)x + a<sub>0</sub><br>

```C++
long
polynomial_v3(int x, const vector<int> &a)
{
	if (a.empty())
		return 0;

	if (a.size() == 1)
		return a[0];

	size_t i = a.size() - 1;

	long result = a[i] * x + a[i - 1];
	i--;

	for (; i > 0; --i)
		result = (result * x) + a[i - 1];

	return result;
}
```
This function involves only *n* multiplications and *n* additions.
