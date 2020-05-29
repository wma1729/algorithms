#include <iostream>
#include <cstdlib>
#include <cstring>
#include <vector>
#include <algorithm>

using namespace std;

/*
 * Two largest items in the sequence.
 *
 * @param [in]  seq    - the input sequence.
 * @param [out] large1 - the largest item.
 * @param [out] large2 - the second largest item.
 */
void
two_largest_v1(const vector<int> &seq, int &large1, int &large2)
{
	int ncmp = 0;

	for (size_t i = 0; i < seq.size(); ++i) {
		if (++ncmp && (seq[i] > large1)) {
			large2 = large1;
			large1 = seq[i];
		} else if (++ncmp && (seq[i] > large2)) {
			large2 = seq[i];
		}
	}

#if defined(DEBUG)
	cout << "n = " << seq.size() << ", cmp = " << ncmp << endl;
#endif
}

static void
two_largest_v2(const vector<int> &seq, size_t lo, size_t hi, int &large1, int &large2, int &ncmp)
{
	if (lo >= hi)
		return;

	if ((hi - lo) == 1) {
		large1 = seq[lo];
	} else if ((hi - lo) == 2) {
		if (++ncmp && (seq[lo] > seq[lo + 1])) {
			large1 = seq[lo];
			large2 = seq[lo + 1];
		} else {
			large1 = seq[lo + 1];
			large2 = seq[lo];
		}
	} else {
		int p1, p2;
		int q1, q2;

		size_t mid = (lo + hi) / 2;
		two_largest_v2(seq, lo, mid, p1, p2, ncmp);
		two_largest_v2(seq, mid, hi, q1, q2, ncmp);

		if (++ncmp && (p1 > q1)) {
			large1 = p1;
			if (++ncmp && (p2 > q1))
				large2 = p2;
			else
				large2 = q1;
		} else {
			large1 = q1;
			if (++ncmp && (q2 > p1))
				large2 = q2;
			else
				large2 = p1;
		}
	}
}

/*
 * Two largest items in the sequence using naive
 * divide and conqueuer.
 *
 * @param [in]  seq    - the input sequence.
 * @param [out] large1 - the largest item.
 * @param [out] large2 - the second largest item.
 */
void
two_largest_v2(const vector<int> &seq, int &large1, int &large2)
{
	int ncmp = 0;

	two_largest_v2(seq, 0, seq.size(), large1, large2, ncmp);

#if defined(DEBUG)
	cout << "n = " << seq.size() << ", cmp = " << ncmp << endl;
#endif
}

static void
two_largest_v3(const vector<int> &seq, size_t lo, size_t hi, int &large1, vector<int> &large2_choices, int &ncmp)
{
	if (lo >= hi)
		return;

	if ((hi - lo) == 1) {
		large1 = seq[lo];
	} else if ((hi - lo) == 2) {
		if (++ncmp && (seq[lo] > seq[lo + 1])) {
			large1 = seq[lo];
			large2_choices.push_back(seq[lo + 1]);
		} else {
			large1 = seq[lo + 1];
			large2_choices.push_back(seq[lo]);
		}
	} else {
		int p1; vector<int> p2_choices;
		int q1; vector<int> q2_choices;

		size_t mid = (lo + hi) / 2;
		two_largest_v3(seq, lo, mid, p1, p2_choices, ncmp);
		two_largest_v3(seq, mid, hi, q1, q2_choices, ncmp);

		if (++ncmp && (p1 > q1)) {
			large1 = p1;
			large2_choices.push_back(q1);
			for (auto l2 : p2_choices)
				large2_choices.push_back(l2);
			// q2_choices is completely ignored.
		} else {
			large1 = q1;
			large2_choices.push_back(p1);
			for (auto l2 : q2_choices)
				large2_choices.push_back(l2);
			// p2_choices is completely ignored.
		}
	}
}

/*
 * Two largest items in the sequence using smart
 * divide and conqueuer.
 *
 * @param [in]  seq    - the input sequence.
 * @param [out] large1 - the largest item.
 * @param [out] large2 - the second largest item.
 */
void
two_largest_v3(const vector<int> &seq, int &large1, int &large2)
{
	int ncmp = 0;
	vector<int> large2_choices;

	two_largest_v3(seq, 0, seq.size(), large1, large2_choices, ncmp);

	for (size_t i = 0; i < large2_choices.size(); ++i) {
		if (++ncmp && (large2_choices[i] > large2))
			large2 = large2_choices[i];
	}

#if defined(DEBUG)
        cout << "n = " << seq.size() << ", cmp = " << ncmp << endl;
#endif
}

int
main(int argc, const char **argv)
{
	vector<int> seq;
	int large1 = -1;
	int large2 = -1;

	for (int i = 2; i < argc; ++i)
		seq.push_back(atoi(argv[i]));

	if (strcmp(argv[1], "-v1") == 0)
		two_largest_v1(seq, large1, large2);
	else if (strcmp(argv[1], "-v2") == 0)
		two_largest_v2(seq, large1, large2);
	else if (strcmp(argv[1], "-v3") == 0)
		two_largest_v3(seq, large1, large2);

	cout << "largest: " << large1 << ", second largest: " << large2 << endl;

	return 0;
}
