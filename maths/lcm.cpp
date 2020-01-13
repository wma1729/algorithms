#include "maths.h"

long
lcm_v1(long m, long n)
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

long
lcm_v2(long m, long n)
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

long
lcm_v3(long m, long n)
{
	return (m * n) / gcd(m, n);
}
