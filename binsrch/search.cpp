#include <iostream>
#include <vector>

using namespace std;

/*
 * Iterative binary search.
 *
 * @param [in] seq - the input sequence (most be sorted).
 * @param [in] key - the items to search.
 *
 * @return item index if found, -1 otherwise.
 */
template<typename T>
int
binary_search_v1(const vector<T> &seq, const T &key)
{
	if (seq.empty())
		return -1;

	size_t lo = 0;
	size_t hi = seq.size();

	while (lo < hi) {
		size_t mid = (lo + hi) / 2;
		if (seq[mid] < key)
			lo = mid + 1;
		else if (seq[mid] == key)
			return static_cast<int>(mid);
		else
			hi = mid;
	}

	return -1;
}

/*
 * Recursive binary search in seq [lo, hi).
 *
 * @param [in] seq - the input sequence (most be sorted).
 * @param [in] lo  - the start index.
 * @param [in] hi  - the end index (one past the end of the range).
 * @param [in] key - the items to search.
 *
 * @return item index if found, -1 otherwise.
 */
template<typename T>
static int
binary_search_v2(const vector<T> &seq, size_t lo, size_t hi, const T &key)
{
	if (lo >= hi)
		return -1;

	size_t mid = (lo + hi) / 2;
	if (seq[mid] < key)
		lo = mid + 1;
	else if (seq[mid] == key)
		return static_cast<int>(mid);
	else
		hi = mid;

	return binary_search_v1(seq, lo, hi, key);
}

/*
 * Recursive binary search.
 *
 * @param [in] seq - the input sequence (most be sorted).
 * @param [in] key - the items to search.
 *
 * @return item index if found, -1 otherwise.
 */
template<typename T>
int
binary_search_v2(const vector<T> &seq, const T &key)
{
	if (seq.empty())
		return -1;

	return binary_search_v2(seq, 0, seq.size(), key);
}

/*
 * Reverse a sequence seq[lo, hi]
 *
 * @param [inout] seq - the input sequence.
 * @param [in]    lo  - the lower bound (inclusive).
 * @param [in]    hi  - the upper ound (inclusive).
 *
 * @return reversed sequence.
 */
template<typename T>
void
reverse_sequence(vector<T> &seq, size_t lo, size_t hi)
{
	T tmp;

	while (lo < hi) {
		tmp = seq[lo];
		seq[lo] = seq[hi];
		seq[hi] = tmp;
		lo++;
		hi--;
	}
}

/*
 * Rotate sequence 'p' position.
 *
 * @param [inout] seq - the input sequence.
 * @param [in]    p   - how much to rotate? 1 <= p < seq.size()
 *
 * @return the sequence is rotated p position.
 */
template<typename T>
void
rotate_sequence(vector<T> &seq, size_t p)
{
	if (seq.empty())
		return;

	if ((p < 1) || (p >= seq.size()))
		return;

	reverse_sequence(seq, 0, p - 1);
	reverse_sequence(seq, p, seq.size() - 1);
	reverse_sequence(seq, 0, seq.size() - 1);
}

int
main()
{
	vector<int> svec;	// sorted vector
	svec.push_back(0);
	svec.push_back(1);
	svec.push_back(2);
	svec.push_back(3);
	svec.push_back(4);
	svec.push_back(5);
	svec.push_back(6);
	svec.push_back(7);
	svec.push_back(8);
	svec.push_back(9);

	for (int i = 0; i < 10; ++i) {
		int j = binary_search_v1(svec, i);
		if (j != i) {
			cerr << "binary search failed: i = " << i << ", j = " << j << endl;
			return 1;
		}
	}

	if (binary_search_v1(svec, 20) != -1) {
		cerr << "magic!!! found that was not even in the list" << endl;
		return 1;
	}

	for (size_t i = 1; i < svec.size(); ++i) {
		vector<int> tmpvec(svec);
		rotate_sequence(tmpvec, i);
		for (auto j : tmpvec)
			cout << j << " ";
		cout << endl;
	}

	return 0;
}
