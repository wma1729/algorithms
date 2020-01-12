#include "maths.h"

/*
 * Finds the floor square root of a number.
 * This is effectively equivalent to floor(sqrt(n)).
 */
long
floor_sqrt(long number)
{
	long lo = 1;
	long hi = number;
	long mid;
	long sq;
	long res = 0;

	if (number < 1)
		return 0;

	while (lo <= hi) {
		mid = (lo + hi) / 2;
		sq = mid * mid;
		if (sq < number) {
			lo = mid + 1;
			res = mid;
		} else if (sq == number) {
			res = mid;
			break;
		} else {
			hi = mid - 1;
		}
	}

	return res;
}
