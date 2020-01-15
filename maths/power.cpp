#include "maths.h"

/*
 * Very basic power function for +ve n and k.
 * This method requires k multiplications.
 */
long
power_v1(int n, int k)
{
	if (k == 0)
		return 1;

	long result = n;

	for (int i = 1; i < k; ++i) {
		result *= n;
	}

	return result;
}

/*
 * Improvized version. It tries to minimize the
 * number of multiplications.
 */
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
