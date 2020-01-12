#include <algorithm>
#include "maths.h"

/*
 * Finds all prime factors of a number.
 */
std::vector<long>
prime_factors(long number)
{
	std::vector<long> factors;

	if (number <= 0) {
		return factors;
	} else if (number == 1) {
		return factors;
	} else {
		for (long i = 2; i <= 3; ++i) {
			while ((number % i) == 0) {
				factors.push_back(i);
				number /= i;
			}
		}

		long fsqrt = floor_sqrt(number);

		for (long i = 5, ni = 2; i <= fsqrt; i += ni, ni = 6 - ni) {
			while ((number % i) == 0) {
				factors.push_back(i);
				number /= i;
			}
			fsqrt = floor_sqrt(number);
		}

		if ((number > 1) && is_prime(number))
			factors.push_back(number);
	}

	return factors;
}

/*
 * Find all common factors of a number.
 */
std::vector<long>
common_factors(long number)
{
	std::vector<long> factors;

	if (number <= 0) {
		return factors;
	} else if (number == 1) {
		return factors;
	} else {
		factors.push_back(1);
		factors.push_back(number);

		long fsqrt = floor_sqrt(number);

		for (long i = 2; i <= fsqrt; ++i) {
			if ((number % i) == 0) {
				factors.push_back(i);
				factors.push_back(number / i);
			}
		}
	}

	std::sort(factors.begin(), factors.end());
	return factors;
}
