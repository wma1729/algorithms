#include <iostream>
#include <ostream>
#include <vector>
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
	 * The longest sequence should be first.
	 * If two or more sequences have the same size,
	 * the sequence with the smaller last item wins.
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
			// save a copy of existing sequence
			newseq = seq;

			// add item to this sequence
			seq.push_back(item);
			inserted = true;
			break;
		}
	}

	if (!inserted)
		newseq.push_back(item);

	if (!newseq.empty())
		sequences.push_back(newseq);

	// sort the sequence for the next item
	sort(sequences.begin(), sequences.end(), best_lis_cmp());
}

/*
 * Longest increasing sequence.
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
	// log all sequences
	for (auto s : sequences)
		cout << s << endl;
#endif

	return sequences[0];
}

int
main(int argc, const char **argv)
{
	vector<int> seq;

	for (int i = 1; i < argc; ++i)
		seq.push_back(atoi(argv[i]));

	cout << lis_v1(seq) << endl;

	return 0;
}
