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
 * - a[0, lo - 1] is 0
 * - a[lo, mid - 1] is 1
 * - a[mid, hi] is unknown
 * - a[hi + 1, n - 1] is 2
 * Start: lo = 0, mid = 0, hi = n - 1
 */
void
sort_dutch_flag(vector<int> &flag)
{
	int lo = 0;
	int mid = 0;
	int hi = static_cast<int>(flag.size() - 1);

	while (mid < hi) {
		switch (flag[mid]) {
			case 0: swap(flag[lo++], flag[mid++]); break;
			case 1: mid++; break;
			case 2: default: swap(flag[mid], flag[hi--]); break;
		}
	}

	cout << "lo = " << lo << ", flag[lo] = " << flag[lo] << endl;
	cout << "mid = " << mid << ", flag[mid] = " << flag[mid] << endl;
	cout << "hi + 1 = " << hi + 1 << ", flag[hi + 1] = " << flag[hi + 1] << endl;
}

int
main()
{
	vector<int> flag { 2, 1, 0, 1, 0, 2, 2, 1, 0, 1, 2, 0 };
	cout << v2s(flag) << endl;
	sort_dutch_flag(flag);
	cout << v2s(flag) << endl;
	return 0;
}
