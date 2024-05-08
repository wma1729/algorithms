#include <iostream>
#include <ostream>
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

	int lo = 0;
	int hi = static_cast<int>(seq.size());
	int mid;

	while (lo < hi) {
		mid = (lo + hi) / 2;
		if (seq[mid] < key)
			lo = mid + 1;
		else if (seq[mid] == key)
			return mid;
		else
			hi = mid;
	}

	return -1;
}

/*
 * Recursive binary search in seq [lo, hi].
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
binary_search_v2(const vector<T> &seq, int lo, int hi, const T &key)
{
	if (lo > hi)
		return -1;

	int mid = (lo + hi) / 2;
	if (seq[mid] < key)
		lo = mid + 1;
	else if (seq[mid] == key)
		return mid;
	else
		hi = mid;

	return binary_search_v2(seq, lo, hi, key);
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

	return binary_search_v2(seq, 0, static_cast<int>(seq.size()), key);
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
	while (lo < hi)
		std::swap(seq[lo++], seq[hi--]);
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

/*
 * Find the inflection point in a cyclic sequence.
 * 
 * @param [in] seq - the input cyclic sequence with unique items.
 *
 * @return index of the inflection point.
 */
template<typename T>
int
inflection_point_in_cyclic_sequence(const vector<T> &seq)
{
	if (seq.empty())
		return -1;

	int lo = 0;
	int hi = static_cast<int>(seq.size() - 1);
	int mid;

	while (lo < hi) {
		mid = (lo + hi) / 2;

		if (seq[mid] < seq[hi]) {
			/*
			 * seq[mid, hi] is sorted; inflection point not here.
			 * Do not exclude mid yet. Why? Try the algorithm
			 * with sequence 3, 0, 1, 2 in mind or on paper.
			 */
			hi = mid;
		} else {
			/* seq[lo, mid] is sorted; inflection point not here */
			lo = mid + 1;
		}
	}

	return lo;
}

/*
 * Find if key falls in the range of seq[lo, hi]. seq[lo, hi] is
 * expected to be sorted.
 *
 * @param [in] seq - the input sequence (most be sorted).
 * @param [in] lo  - the start index.
 * @param [in] hi  - the end index (one past the end of the range).
 * @param [in] key - the items to search.
 *
 * @return true if key is in range, false otherwise.
 */
template<typename T>
inline bool
in_range(const vector<T> &seq, int lo, int hi, const T &key)
{
	return ((lo < hi) && (key >= seq[lo]) && (key <= seq[hi]));
}

/*
 * Find key in a sorted sequence that is rotated at an unknow position.
 *
 * @param [in] seq - the input sequence (most be sorted).
 * @param [in] key - the items to search.
 *
 * @return item index if found, -1 otherwise.
 */
template<typename T>
int
binary_search_rotated_sequence(const vector<T> &seq, const T &key)
{
	if (seq.empty())
		return -1;

	int lo = 0;
	int hi = static_cast<int>(seq.size() - 1);
	int mid;

	while (lo <= hi) {
		mid = (lo + hi) / 2;

		/* handle the obvious case */
		if (seq[mid] == key)
			return static_cast<int>(mid);

		if (lo == mid) {
			/* and we know that seq[mid] != key */
			lo = mid + 1;
		} else if (mid == hi) {
			/* and we know that seq[mid] != key */
			hi = mid - 1;
		} else if (seq[lo] < seq[mid]) {
			/* seq[lo, mid] is sorted */
			if (in_range(seq, lo, mid - 1, key))
				hi = mid - 1;
			else
				lo = mid + 1;
		} else {
			/* seq[mid, hi] is sorted */
			if (in_range(seq, mid + 1, hi, key))
				lo = mid + 1;
			else
				hi = mid - 1;
		} 
	}

	return -1;
}

template<typename T>
static std::ostream &
operator<< (std::ostream &os, const std::vector<T> &seq)
{
	typename std::vector<T>::const_iterator it;
	for (it = seq.begin(); it != seq.end(); ++it) {
		if (it != seq.begin())
			os << ", ";
		os << *it;
	}
	return os;
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
		} else {
			cout << i << " found" << endl;
		}
	}

	for (int i = 0; i < 10; ++i) {
		int j = binary_search_v2(svec, i);
		if (j != i) {
			cerr << "binary search failed: i = " << i << ", j = " << j << endl;
			return 1;
		} else {
			cout << i << " found" << endl;
		}
	}

	if (binary_search_v1(svec, 20) != -1) {
		cerr << "magic!!! found that was not even in the list" << endl;
		return 1;
	}

	cout << "input: " << svec << endl;

	for (size_t i = 1; i < svec.size(); ++i) {
		vector<int> tmpvec(svec);
		rotate_sequence(tmpvec, i);
		cout << "rotate(" << i << "): " << tmpvec << endl;
		cout << "inflection point = " << inflection_point_in_cyclic_sequence(tmpvec) << endl;
		int j = binary_search_rotated_sequence(tmpvec, 9);
		cout << "input[" << j << "] = " << 9 << endl;
	}

	return 0;
}
