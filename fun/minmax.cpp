#include <iostream>
#include <cstdlib>
#include <cstring>
#include <vector>

using namespace std;

/*
 * Scan the sequence twice: once to find the minumum
 * and the next time to find the maximum. Each scan
 * incurs n - 1 comparisons. So we need:
 * (n - 1) + (n - 1) = ~2n comparisons.
 */
void
minmax_v1(const vector<int> &seq, int &min, int &max)
{
	int ncmp = 0;

	if (seq.empty())
		return;

	min = max = seq[0];

	for (size_t i = 1; i < seq.size(); ++i) {
		if (++ncmp && (seq[i] < min))
			min = seq[i];
	}

	for (size_t i = 1; i < seq.size(); ++i) {
		if (++ncmp && (seq[i] > max))
			max = seq[i];
	}

#if defined(DEBUG)
	cout << "n = " << seq.size() << ", cmp = " << ncmp << endl;
#endif
}

/*
 * Scan the sequence only once.
 * If an item is larger than the maximum, then it cannot be
 * less than the minumum. This fact helps us to knock out a
 * few comparison. But overall it is not much better.
 */
void
minmax_v2(const vector<int> &seq, int &min, int &max)
{
	int ncmp = 0;

	if (seq.empty())
		return;

	min = max = seq[0];

	for (size_t i = 1; i < seq.size(); ++i) {
		if (++ncmp && (seq[i] > max))
			max = seq[i];
		else if (++ncmp && (seq[i] < min))
			min = seq[i];
	}

#if defined(DEBUG)
	cout << "n = " << seq.size() << ", cmp = " << ncmp << endl;
#endif

	return;
}

/*
 * Scan the sequence only once.
 * Instead of processing 1 item at a time, process 2 items
 * at once. Compare and find local min and max. Compare the
 * local max with global max and find the new global max.
 * Compare the local min with global min and find the new
 * global min. So by scanning 2 items at a time, we need
 * 3 comparisons to find the new maximum and the new minimum.
 * In general, we need 3n/2 comparisons.
 */
void
minmax_v3(const vector<int> &seq, int &min, int &max)
{
	int ncmp = 0;

	if (seq.empty())
		return;

	min = max = seq[0];

	for (size_t i = 1; i < seq.size(); i += 2) {
		if ((i + 1) == seq.size()) {
			if (++ncmp && (seq[i] > max))
				max = seq[i];
			else if (++ncmp && (seq[i] < min))
				min = seq[i];
		} else {
			int mx, mn;

			++ncmp;
			if (seq[i] < seq[i + 1]) {
				mn = seq[i];
				mx = seq[i + 1];
			} else {
				mn = seq[i + 1];
				mx = seq[i];
			}

			if (++ncmp && (mx > max))
				max = mx;
			if (++ncmp && (mn < min))
				min = mn;
		}
	}

#if defined(DEBUG)
	cout << "n = " << seq.size() << ", cmp = " << ncmp << endl;
#endif

	return;
}

static void
minmax_v4(const vector<int> &seq, size_t lo, size_t hi, int &min, int &max, int &ncmp)
{
	if (lo >= hi)
		return;

	if ((hi - lo) == 1) {
		min = max = seq[lo];
	} else if ((hi - lo) == 2) {
		ncmp++;
		if (seq[lo] < seq[hi]) {
			min = seq[lo];
			max = seq[lo + 1];
		} else {
			max = seq[lo];
			min = seq[lo + 1];
		}	
	} else {
		int min_l, max_l;
		int min_r, max_r;
		size_t mid = (lo + hi) / 2;

		minmax_v4(seq, lo, mid, min_l, max_l, ncmp);
		minmax_v4(seq, mid, hi, min_r, max_r, ncmp);

		max = max_l;
		if (++ncmp && (max < max_r))
			max = max_r;

		min = min_l;
		if (++ncmp && (min > min_r))
			min = min_r;
	}

	return;
}

/*
 * Divide and Conquer.
 */
void
minmax_v4(const vector<int> &seq, int &min, int &max)
{
	int ncmp = 0;

	minmax_v4(seq, 0, seq.size(), min, max, ncmp);

#if defined(DEBUG)
	cout << "n = " << seq.size() << ", cmp = " << ncmp << endl;
#endif

	return;
}

int
main(int argc, const char **argv)
{
	vector<int> seq;
	int min = -1;
	int max = -1;

	for (int i = 2; i < argc; ++i)
		seq.push_back(atoi(argv[i]));

	if (strcmp(argv[1], "-v1") == 0)
		minmax_v1(seq, min, max);
	else if (strcmp(argv[1], "-v2") == 0)
		minmax_v2(seq, min, max);
	else if (strcmp(argv[1], "-v3") == 0)
		minmax_v3(seq, min, max);
	else if (strcmp(argv[1], "-v4") == 0)
		minmax_v4(seq, min, max);

	cout << "min: " << min << ", max: " << max << endl;

	return 0;
}
