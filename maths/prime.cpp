#include "maths.h"

bool
is_prime_v1(long number)
{
	if (number < 2)
		return false;

	if ((number == 2) || (number == 3))
		return true;

	if ((number % 2) == 0)
		return false;

	if ((number % 3) == 0)
		return false;

	long fsqrt = floor_sqrt(number);

	for (long i = 5; i <= fsqrt; ++i) {
		if ((number % i) == 0)
			return false;
	}

	return true;
}

bool
is_prime_v2(long number)
{
	if (number < 2)
		return false;

	if ((number == 2) || (number == 3))
	 	return true;

	if ((number % 2) == 0)
		return false;

	if ((number % 3) == 0)
		return false;

	long fsqrt = floor_sqrt(number);

	/*
	 * i is the starting number and ni is the next increment.
	 * Following 6k - 1; 6k + 1 rule. All the prime numbers
	 * follow the 6k - 1; 6k + 1 rule starting with 5.
	 * Not all 6k - 1 and 6k + 1 numbers are prime but knowing
	 * this fact, reduces the checks a lot.
	 *
	 * We increment the number by 2 then 4 then 2 then 4 cycle.
	 * i.e.
	 *     if (ni == 2)
	 *         ni = 4;
	 *     else if (ni == 4)
	 *         ni = 2;
	 * OR
	 *     ni = 6 - ni
	 */

	for (long i = 5, ni = 2; i <= fsqrt; i += ni, ni = 6 - ni) {
		if ((number % i) == 0)
			return false;
	}

	return true;
}

/*
 * Find a number greater than or equal to the given number.
 */
long
prime_ge(long number)
{
	if (is_prime(number))
		return number;

	long k = number / 6;

	if ((k * 6 + 1) < number)
		k++;

	long i = 6 * k - 1;
	long ni = 2;

	if (i < number) {
		i = 6 * k + 1;
		ni = 4;
	}

	while (true) {
		if (is_prime(i))
			return i;
		i += ni;
		ni = 6 - ni;
	}

	return -1;
}
