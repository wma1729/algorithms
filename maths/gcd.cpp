#include "maths.h"

long
gcd_v1(long m, long n)
{
	// Find the smaller of the two numbers
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

long
gcd_v2(long m, long n)
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

long
gcd_v3(long m, long n)
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

long
gcd_v4(long m, long n)
{
	while (m != n) {
		if (m > n)
			m = (m - n);
		else
			n = (n - m);
	}

	return m;
}

long
gcd_v5(long m, long n)
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
