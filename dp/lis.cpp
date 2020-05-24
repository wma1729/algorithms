#include <iostream>
#include <iomanip>
#include <ostream>
#include <vector>
#include <list>
#include <algorithm>

using namespace std;

/*
 * Comparator to sort LIS in the best
 * order for appending the next item.
 */
class best_lis_cmp
{
public:
	/*
	 * The longest subsequence should be first.
	 * If two or more subsequences have the same size,
	 * the subsequence with the smaller last item wins.
	 */
	bool operator() (const vector<int> &seq1, const vector<int> &seq2)
	{
		if (seq1.size() > seq2.size())
			return true;
		else if (seq1.size() == seq2.size())
			return (seq1.back() < seq2.back());
		else
			return false;
	}
};

ostream &
operator << (ostream &os, const vector<int> &seq)
{
	vector<int>::const_iterator it;
	for (it = seq.begin(); it != seq.end(); ++it) {
		if (it != seq.begin())
			os << ", ";
		os << *it;
	}
	return os;
}

/*
 * Add the item to the best LIS.
 */
void
add_to_best_lis(vector<vector<int>> &sequences, int item)
{
	bool inserted = false;
	vector<int> newseq;

	for (size_t i = 0; i < sequences.size(); ++i) {
		vector<int> &seq = sequences[i];
		if (seq.back() < item) {
			// save a copy of existing subsequence
			newseq = seq;

			// add item to this subsequence
			seq.push_back(item);
			inserted = true;
			break;
		}
	}

	if (!inserted)
		newseq.push_back(item);

	if (!newseq.empty())
		sequences.push_back(newseq);

	// sort the subsequences for the next item
	stable_sort(sequences.begin(), sequences.end(), best_lis_cmp());
}

/*
 * Longest increasing subsequence.
 */
vector<int>
lis_v1(const vector<int> &seq)
{
	if (seq.empty())
		return vector<int>();

	vector<vector<int>> sequences;

	for (size_t i = 0; i < seq.size(); ++i)
		add_to_best_lis(sequences, seq[i]);

#if defined(DEBUG)
	// log all subsequences
	for (auto s : sequences)
		cout << s << endl;
#endif

	return sequences[0];
}

/*
 * Longest increasing subsequence (using dynamic programming).
 */
vector<int>
lis_v2(const vector<int> &seq)
{
	vector<int> lis;

	if (seq.empty())
		return lis;

	struct lis_info
	{
		int max = 1;
		int idx = -1;
	} li;

	vector<lis_info> lisinfo(seq.size(), li);

	/*
	 * MAX and IDX helps in constructing the subsequence easily.
	 */
	int MAX = 1;    // Max value of subsequence seen so far.
	int IDX = -1;   // Index of the max (not the previous index).

	for (size_t i = 0; i < seq.size(); ++i) {
#if defined(DEBUG)
		for (size_t k = 0; k < seq.size(); ++k)
			cout << setw(4) << seq[k];
		cout << endl;
		for (size_t k = 0; k < lisinfo.size(); ++k)
			cout << setw(4) << lisinfo[k].max;
		cout << endl;
		for (size_t k = 0; k < lisinfo.size(); ++k)
			cout << setw(4) << lisinfo[k].idx;
		cout << endl;
#endif

		for (size_t j = i + 1; j < seq.size(); ++j) {
			if (seq[j] > seq[i]) {
				if (lisinfo[j].max < lisinfo[i].max + 1) {
					lisinfo[j].max = lisinfo[i].max + 1;
					lisinfo[j].idx = i;
					if (lisinfo[j].max > MAX) {
						MAX = lisinfo[j].max;
						IDX = j;
					}
				}
			}
		}
	}

#if defined(DEBUG)
	for (size_t i = 0; i < seq.size(); ++i)
		cout << "index = " << i <<
			", value = " << seq[i] <<
			", maximum length = " << lisinfo[i].max <<
			", previous index = " << lisinfo[i].idx << endl;
#endif

	while (IDX >= 0) {
		lis.push_back(seq[IDX]);
		IDX = lisinfo[IDX].idx;
	}

	reverse(lis.begin(), lis.end());

	return lis;
}

int
main(int argc, const char **argv)
{
	vector<int> seq;

	for (int i = 1; i < argc; ++i)
		seq.push_back(atoi(argv[i]));

	cout << lis_v2(seq) << endl;

	return 0;
}
