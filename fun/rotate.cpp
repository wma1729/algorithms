#include <iostream>
#include <cstring>

using namespace std;

int
gcd(int m, int n)
{
	int lo = std::min(m, n);
	int hi = std::max(m, n);
	int r = 1;

	while (r > 0) {
		r = hi % lo;
		hi = lo;
		lo = r;
	}

	return hi;
}

static void
rotate(char x[], int n, int rotdist)
{
	for (int index = 0; index < gcd(n, rotdist); ++index) {
		int t = x[index];
		int current = index;
		while (true) {
			int next = current + rotdist;
			if (next >= n)
				next -= n;
			if (next == index)
				break;
			x[current] = x[next];
			current = next;
		}
		x[current] = t;
	}
}

int
main(int argc, char **argv)
{
	char *x = argv[1];
	rotate(x, int(strlen(x)), atoi(argv[2]));
	cout << x << endl;
	return 0;
}
