/**
 * Function to return ‘n’ bits from ‘x’ beginning at position ‘p’.
 * We assume that bit position 0 is at the right end and that n and p
 * are sensible positive values.
 * For example, getbits(x, 4, 3) returns the three bits in positions
 * 4, 3 and 2, right-adjusted.
 * @param [in]  x   the number specified.
 * @param [in]  p   the bit position in the number ‘x’ counted from
 *                  the rightmost bit.
 * @param [in]  n   the number of bits needed.
 *
 * @return the right adjusted n-bit field of x that begins at
 * position p.
 */ 

unsigned int
getbits(unsigned int x, int p, int n)
{
	return (x >> (p - n + 1)) & ~(~0 << n)
}

/**
 * Function to replace ‘n’ bits from ‘x’ beginning at position ‘p’
 * with ‘n’ bits from ‘y’. We assume that bit position 0 is at the
 * right end and that n and p are sensible positive values.
 *
 *
 *        7   6   5   4   3   2   1   0
 *      +---+---+---+---+---+---+---+---+
 *   x: | 0 | 1 | 0 | 1 | 1 | 0 | 0 | 1 |
 *      +---+---+---+---+---+---+---+---+
 *                p
 *              |<----n---->|
 *
 *        7   6   5   4   3   2   1   0
 *      +---+---+---+---+---+---+---+---+
 *   y: | 0 | 0 | 1 | 1 | 0 | 1 | 0 | 1 |
 *      +---+---+---+---+---+---+---+---+
 *                          |<----n---->|
 *
 *        7   6   5   4   3   2   1   0
 *      +---+---+---+---+---+---+---+---+
 *   r: | 0 | 1 | 1 | 0 | 1 | 0 | 0 | 1 |
 *      +---+---+---+---+---+---+---+---+
 *              |<----n---->|
 *
 * @param [in]  x   the number specified.
 * @param [in]  p   the bit position in the number ‘x’ counted from
 *                  the rightmost bit.
 * @param [in]  n   the number of bits need that should be replaced.
 * @param [in]  y   the number from where new bits are to be taken
 *                  for replacement.
 *
 * @return the new number with bits replaced (r in the above
 * example).
 */ 
unsigned int
setbits(unsigned int x, int p, int n, unsigned int y)
{
	unsigned int x1, x2, x3;

	/* Get the bits left of position p */	
	x1 = x >> (p + 1);
	x1 <<= (p + 1);

	/* Get the bits that are to be replaced */
	x2 = y & ~(~0 << n);
	x2 <<= (p + 1) - n;

	/* Get the remaining bits */
	x3 = x & ~(~0 << ((p + 1) - n));

	return x1 | x2 | x3;
}

/**
 * Function to invert (change 1 to 0 and vice versa) ‘n’ bits
 * starting at position ‘p’ in the number ‘x’.
 * With getbits() and setbits() available, the solution involves
 * using these appropriately.
 * @param [in]  x   the number specified.
 * @param [in]  p   the bit position in the number ‘x’ counted from
 *                  the rightmost bit.
 * @param [in]  n   the number of bits needed.
 *
 * @return the number with ‘n’ bits inverted starting at position
 * ‘p’ in the number ‘x’.
 */ 
unsigned int
invert(unsigned int x, int p, int n)
{
	/* get n bits starting at position p */
	unsigned int i = getbits(x, p, n);

	/* invert the bits */
	i = ~i;

	/* set the inverted bits back into the number */
	return setbits(x, p, n, i);
}

/**
 * Function to rotate ‘n’ bits to the right in the number ‘x’.
 * @param [in]  x   the number specified.
 * @param [in]  n   the number of bits to rotate.
 *
 * @return the number with ‘n’ bits rotated to the right.
 */
unsigned int
rightrot(unsigned int x, int n)
{
	/* get the rightmost n bits from x */
	unsigned int i = x & (~(~0 << n));

	/* right shift the number n bits */
	x >>= n;

	/* left shift i ‘32 - n’ bits */
	i <<= ((sizeof(unsigned int) * 8) - n);

	/* concatenate the two */
	return i | x;
}
