#include <iostream>
#include <sstream>
#include <vector>

using namespace std;

static string
v2s(const vector<int> &a)
{
	ostringstream oss;
	for (size_t i = 0; i < a.size(); ++i) {
		if (i > 0)
			oss << ", ";
		oss << a[i];
	}
	return oss.str();
}

/*
 * Invariant for n numbers:
 * - a[0, lo] is white (0)
 * - a[hi + 1, n) is black(1)
 * - a[lo + 1, hi] is unknown
 * Start: lo = 0, hi = n - 1
 * End: lo == hi, a[0, lo] is white, a[hi + 1, n - 1] is black.
 */
static void
sortbw(vector<int> &a)
{
	int white = 0;
	int lo = 0;
	int hi = static_cast<int>(a.size() - 1);

	while (lo < hi) {
		if (a[lo] == white)
			lo++;
		else
			swap(a[lo], a[hi--]);
	} 

	cout << "lo = " << lo << ", a[lo] = " << a[lo] << endl;
	cout << "hi + 1 = " << hi + 1 << ", a[hi + 1] = " << a[hi + 1] << endl;
}

int
main()
{
	vector<int> a { 1, 0, 0, 1, 1, 1, 0, 0, 0, 1, 0, 1, 1, 0, 0 };
	cout << v2s(a) << endl;
	sortbw(a);
	cout << v2s(a) << endl;
	return 0;
}
