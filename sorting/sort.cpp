#include <iostream>
#include <ostream>
#include <sstream>
#include <fstream>
#include <iomanip>
#include <vector>
#include <queue>
#include <array>
#include <string>
#include <cstring>

using namespace std;

template<typename T>
static ostream &
operator<< (ostream &os, const vector<T> &values)
{
	typename vector<T>::const_iterator it;
	for (it = values.begin(); it != values.end(); ++it) {
		if (it != values.begin())
			os << ", ";
		os << *it;
	}
	return os;
}

template<typename T>
static ostream &
operator<< (ostream &os, const queue<T> &values)
{
	bool first = true;
	queue<T> tmpq(values);

	while (!tmpq.empty()) {
		if (!first) {
			os << ", ";	
		} else {
			first = false;
		}
		os << tmpq.front();
		tmpq.pop();
	}
	return os;
}

/*
 * Get the character at the specified index in the
 * given string.
 *
 * @param [in] s - the given string.
 * @param [in] i - the given index.
 *
 * @return character at the specified index in the string.
 * If the index is beyond the string length, return 0.
 */
static int
char_at(const std::string &s, size_t i)
{
	return (i < s.length()) ? s[i] : 0;
}

/*
 * LSD radix sort. This is a straight-forward  implementation
 * using:
 * - 1 main queue, initialized with input items.
 * - 256 auxiliary queues, one for every possible character [0-255].
 *
 * @param [inout] elements  - the vector to sort.
 * @paran [in]    max_width - the maximum size of string across
 *                            all elements of the vector.
 *
 * @return elements are sorted on return.
 */
void
lsd_radix_sort_v1(vector<string> &elements, size_t max_width)
{
	constexpr int N = 256;
	queue<string> mainq;

	/*
	 * 256 auxiliary queues: one for each valid character.
	 */
	array<queue<string>, N> auxq;

	/*
	 * Move all the strings to the main queue.
	 * This queue is processed (sorted) at every valid
	 * index position.
	 */
	for (auto e : elements)
		mainq.push(e);
	elements.clear();

	/*
	 * Repeat max_width times from least significant digit
	 * (LSD) to most significant digit (MSD) sorting strings
	 * based on the character at index w - 1.
	 */
	for (size_t w = max_width; w > 0; --w) {
		/*
		 * Read the strings from the main queue and
		 * push the strings in the correct queue.
		 */
		while (!mainq.empty()) {
			string s = mainq.front();
			mainq.pop();
			auxq[char_at(s, w - 1)].push(s);
		}

		/*
		 * Copy the strings back from auxiliary queues
		 * to the main queue.
		 */
		for (size_t j = 0; j < N; ++j) {
			while (!auxq[j].empty()) {
				mainq.push(auxq[j].front());
				auxq[j].pop();
			}
		}

#if defined(DEBUG)
		cout << "w = " << w << " : " << mainq << endl;
#endif // DEBUG
	}

	/*
	 * The main queue is sorted now!
	 * Copy the strings from the main queue back to the
	 * vector.
	 */
	while (!mainq.empty()) {
		elements.push_back(mainq.front());
		mainq.pop();
	}

	return;
}

/*
 * LSD radix sort. The sort uses key index count sorting.
 *
 * @param [inout] elements  - the vector to sort.
 * @paran [in]    max_width - the maximum size of string across
 *                            all elements of the vector.
 *
 * @return elements are sorted on return.
 */
void
lsd_radix_sort_v2(vector<string> &elements, size_t max_width)
{
	constexpr int N = 256;
	vector<string> aux;         // auxiliary vector of strings
	array<int, N + 1> count;    // count of valid characters: one for each character.

	/*
	 * Repeat max_width times from least significant digit
	 * (LSD) to most significant digit (MSD) sorting strings
	 * based on the character at index w - 1.
	 */
	for (size_t w = max_width; w > 0; --w) {
		// character index we are dealing with.
		int idx = w - 1;

		// copy all elements to the auxiliary vector.
		for (auto e : elements)
			aux.push_back(e);

		// initialize all the counters to 0.
		for (size_t c = 0; c <= N; ++c)
			count[c] = 0;

		/*
		 * Set the count of each character. There is a trick here.
		 * Instead of setting the count[char_at(a, idx)], it sets
		 * count[char_at(a, idx) + 1].
		 */
		for (auto a : aux)
			count[char_at(a, idx) + 1]++;

		// Calculated the cumulative count.
		for (size_t c = 0; c <= N; ++c)
			count[c + 1] += count[c];

		/*
		 * Now count[char_at(a, idx)] represents the index where the
		 * string should be copied into the main vector. count[char_at(a, idx)]
		 * is incremented to represent the next index in case the character
		 * is seen again.
		 */
		for (auto a : aux)
			elements[count[char_at(a, idx)]++] = a;

		aux.clear();

#if defined(DEBUG)
		cout << "w = " << w << " : " << elements << endl;
#endif // DEBUG
	}

	return;
}

/*
 * MSD radix sort. The sort uses key index count sorting.
 *
 * @param [inout] elements  - the vector to sort.
 * @param [in]    aux       - an auxiliary vector.
 * @param [in]    lo        - process elements starting from lo index.
 * @param [in]    hi        - process elements upto hi index (non inclusive).
 * @param [in]    w         - position at which the character is to be processed.
 * @paran [in]    max_width - the maximum size of string across
 *                            all elements of the vector.
 * @return elements are sorted on return.
 */
static void
msd_radix_sort(vector<string> &elements, vector<string> &aux, size_t lo, size_t hi, size_t w, size_t max_width)
{
	constexpr int N = 256;
	array<int, N + 1> count;    // count of valid characters: one for each character <== trick
	array<int, N> count2;       // count of valid characters: one for each character <== no trick

	// have we scanned all the elements
	if (w == max_width)
		return;

	// check if there is no elements or a single element to process.
	if ((lo == hi) || ((lo + 1) == hi))
		return;

	// copy all elements to the auxiliary vector.
	for (size_t i = lo; i < hi; ++i)
		aux.push_back(elements[i]);

	// initialize all the counters to 0.
	for (size_t c = 0; c <= N; ++c)
		count[c] = 0;

	for (size_t c = 0; c < N; ++c)
		count2[c] = 0;

	/*
	 * Set the count of each character. There is a trick here.
	 * Instead of setting the count[char_at(a, idx)], it sets
	 * count[char_at(a, idx) + 1].
	 */
	for (auto a : aux) {
		int idx = char_at(a, w);
		count[idx + 1]++;   // trick
		count2[idx]++;      // no-trick
	}

	// set the cumulative count.
	for (size_t c = 0; c < N; ++c)
		count[c + 1] += count[c];

	/*
	 * Now lo + count[char_at(a, idx)] represents the index where the
	 * string should be copied in the main vector. count[char_at(a, idx)]
	 * is incremented to represent the next index in case the character
	 * is seen again.
	 */
	for (auto a : aux)
		elements[lo + count[char_at(a, w)]++] = a;

	aux.clear();

#if defined(DEBUG)
	cout << "w = " << w + 1 << ", lo = " << lo << ", hi = " << hi << " : ";
	for (size_t i = 0; i < elements.size(); ++i) {
		if (i != 0)
			cout << ", ";
		if ((i < lo) || (i >= hi))
			cout << "-";
		else
			cout << elements[i];
	}
	cout << endl;
#endif // DEBUG

	/*
	 * The elements are now sorted based on character at position w.
	 * Dive deep to sort elements at character position w + 1.
	 * count2 tells us how many string to process.
	 */
	while (lo < hi) {
		int num_of_elem = count2[char_at(elements[lo], w)];
		msd_radix_sort(elements, aux, lo, lo + num_of_elem, w + 1, max_width);
		lo += num_of_elem;
	}

	return;
}

/*
 * MSD radix sort.
 *
 * @param [inout] elements  - the vector to sort.
 * @paran [in]    max_width - the maximum size of string across
 *                            all elements of the vector.
 *
 * @return elements are sorted on return.
 */
void
msd_radix_sort(vector<string> &elements, size_t max_width)
{
	vector<string> aux;     // auxiliary vector of strings
	msd_radix_sort(elements, aux, 0, elements.size(), 0, max_width);
}

/*
 * Print statistics.
 */
template<typename T>
void
print_stats(size_t iter, size_t ncmp, size_t nswap, const vector<T> &elements)
{
#if defined(DEBUG)
	cout << "iteration = " << setw(2) << iter << " ( " <<  elements <<
		" ) comparisons = " << setw(2) << ncmp << ", swap = " << setw(2) << nswap << endl;
#endif
}

/*
 * Perform bubble sort.
 *
 * @param [inout] elements  - the vector to sort.
 *
 * @return elements are sorted on return.
 */
template<typename T>
void
bubble_sort(vector<T> &elements)
{
	size_t length = elements.size();
	size_t ncmp;
	size_t nswap;

	for (size_t i = 0; i < length - 1; ++i) {
		ncmp = 0;
		nswap = 0;

		for (size_t j = 0; j < length - i - 1; ++j) {
			ncmp++;
			if (elements[j] > elements[j + 1]) {
				nswap++;
				swap(elements[j], elements[j + 1]);
			}
		}

		print_stats(i + 1, ncmp, nswap, elements);
	}
}

/*
 * Perform selection sort.
 *
 * @param [inout] elements  - the vector to sort.
 *
 * @return elements are sorted on return.
 */
template<typename T>
void
selection_sort(vector<T> &elements)
{
	size_t ncmp;
	size_t nswap;

	for (size_t i = 0; i < elements.size() - 1; ++i) {
		size_t min_idx = i;

		ncmp = 0;
		nswap = 0;

		for (size_t j = i + 1; j < elements.size(); ++j) {
			ncmp++;
			if (elements[j] < elements[min_idx]) {
				min_idx = j;
			}
		}

		if (min_idx != i) {
			nswap++;
			swap(elements[i], elements[min_idx]);
		}

		print_stats(i + 1, ncmp, nswap, elements);
	}
}

/*
 * Perform insertion sort.
 *
 * @param [inout] elements  - the vector to sort.
 *
 * @return elements are sorted on return.
 */
template<typename T>
void
insertion_sort(vector<T> &elements)
{
	size_t ncmp;
	size_t nswap;

	/*
	 * All items to the left of i are sorted and items
	 * to the right of i, including i, are unsorted.
	 */
	for (size_t i = 1; i < elements.size(); ++i) {
		ncmp = 0;
		nswap = 0;

		for (size_t j = i; j > 0; --j) {
			ncmp++;
			if (elements[j] < elements[j - 1]) {
				nswap++;
				swap(elements[j], elements[j - 1]);
			} else {
				/*
				 * No point in proceeding further as the elements
				 * to the left of index i are already sorted.
				 */
				break;
			}
		}

		print_stats(i, ncmp, nswap, elements);
	}
}

/*
 * Print statistics.
 */
template<typename T>
void
print_stats(size_t iter, size_t lo, size_t hi, size_t ncmp, size_t ncopy, const vector<T> &elements)
{
#if defined(DEBUG)
	cout << "iteration = " << setw(2) << iter <<
		", lo = " << setw(2) << lo << ", hi = " << setw(2) << hi <<
		" ( " <<  elements << " ) comparisons = " << setw(2) << ncmp <<
		", copies = " << setw(2) << ncopy << endl;
#endif
}

/*
 * Merge two sorted arrays: elements[lo, mid) and elements[mid, hi).
 * Items from elements[lo, hi) are first copied into an auxiliary array.
 * Items from auxiliary[lo, mid) and auxiliary[mid, hi) are then
 * merged back into the elements[lo, hi).
 *
 * @param [inout] elements  - the vector to sort.
 * @param [inout] auxiliary - the auxiliary vector to aid sorting.
 * @param [in]    lo        - the starting index.
 * @param [in]    mid       - the middle index.
 * @param [in]    hi        - the ending index (1 past the last item).
 *
 * @return elements[lo, hi) are merged correctly on return.
 */
template<typename T>
static void
merge(vector<T> &elements, vector<T> &auxiliary, size_t lo, size_t mid, size_t hi, size_t iter)
{
	if ((lo == mid) || (mid == hi)) {
		// at least one of the sub-sequence is empty
		return;
	}

	for (size_t k = lo; k < hi; ++k) {
		// copy concerned elements to auxiliary vector
		auxiliary[k] = elements[k];
	}

	size_t ncmp = 0;
	size_t ncopy = 0;

	size_t i = lo;
	size_t j = mid;

	if (auxiliary[mid - 1] < auxiliary[mid]) {
		/*
		 * Special case:
		 * auxiliary[lo, mid) = ( 2, 3, 4, 5 )
		 * auxiliary[mid, hi) = ( 7, 9, 10, 11 )
		 * Nothing to do.
		 */
		ncmp++;
	} else if (auxiliary[lo] > auxiliary[hi - 1]) {
		/*
		 * Special case:
		 * auxiliary[lo, mid) = ( 7, 9, 10, 11 )
		 * auxiliary[mid, hi) = ( 2, 3, 4, 5 )
		 * First copy auxiliary[mid, hi), then copy auxiliary[lo, mid).
		 */
		ncmp++;
		for (size_t k = mid; k < hi; ++k) {
			ncopy++;
			elements[i++] = auxiliary[k];
		}
		for (size_t k = lo; k < mid; ++k) {
			ncopy++;
			elements[i++] = auxiliary[k];
		}
	} else {
		/*
		 * Normal case:
		 * Do normal comparison and pick element either from
		 * auxiliary[lo, mid) or auxiliary[mid, hi).
		 */
		for (size_t k = lo; k < hi; ++k) {
			ncopy++;
			if (i >= mid) {
				// all elements from auxiliary[lo, mid) are consumed
				elements[k] = auxiliary[j++];
			} else if (j >= hi) {
				// all elements from auxiliary[mid, hi) are consumed
				elements[k] = auxiliary[i++];
			} else if (auxiliary[i] < auxiliary[j]) {
				elements[k] = auxiliary[i++];
				ncmp++;
			} else {
				elements[k] = auxiliary[j++];
				ncmp++;
			}
		}
	}

	print_stats(iter, lo, hi, ncmp, ncopy, elements);
}

/*
 * Perform merge sort.
 *
 * @param [inout] elements  - the vector to sort.
 * @param [inout] auxiliary - the auxiliary vector to aid sorting.
 * @param [in]    lo        - the starting index.
 * @param [in]    ho        - the ending index (1 past the last item).
 * @param [in]    iter      - the iteration number.
 *
 * @return elements[lo, hi) are storted on return.
 */
template<typename T>
static void
merge_sort_v1(vector<T> &elements, vector<T> &auxiliary, size_t lo, size_t hi, size_t &iter)
{
	if ((hi - lo) <= 1) {
		// there is only one element left
		return;
	}

	// get the partition point
	size_t mid = (lo + hi) / 2;

	// sort the lhs sequence
	merge_sort_v1(elements, auxiliary, lo, mid, iter);

	// sort the rhs sequence
	merge_sort_v1(elements, auxiliary, mid, hi, iter);

	// merge lhs & rhs
	merge(elements, auxiliary, lo, mid, hi, iter++);
}

/*
 * Perform merge sort using recursion.
 *
 * @param [inout] elements  - the vector to sort.
 *
 * @return elements are sorted on return.
 */
template<typename T>
void
merge_sort_v1(vector<T> &elements)
{
	// create an auxiliary sequence
	vector<T> auxiliary(elements.size());

	size_t iter = 1;

	// make the recursive call
	merge_sort_v1(elements, auxiliary, 0, elements.size(), iter);
}

/*
 * Perform merge sort without using recursion.
 *
 * @param [inout] elements  - the vector to sort.
 *
 * @return elements are sorted on return.
 */
template<typename T>
void
merge_sort_v2(vector<T> &elements)
{
	// create an auxiliary sequence
	vector<T> auxiliary(elements.size());

	size_t iter = 0;

	/*
	 * Keep doubling the merge size, starting with 1.
	 * Do not use the stop condition as sz < elements.size() / 2.
	 * This will work only if the number of elements are 2 * power(n).
	 */
	for (size_t sz = 1; sz < elements.size(); sz *= 2) {
		size_t n = 2 * sz;

		for (size_t lo = 0; lo < elements.size() - sz; lo += n) {
			size_t mid = lo + sz;
			size_t hi = lo + n;
			if (hi > elements.size())
				hi = elements.size();
			merge(elements, auxiliary, lo, mid, hi, ++iter);
		}
	}
}

/*
 * Partitions the vector into two. The first element elements[lo] is the pivot element.
 * After partition, array[lo] is moved to its correct position. All the elements to
 * left of the pivot are less than array[lo] and all the elements to the right of
 * the pivot are greater than array[lo]. Returns the index of the pivot element position.
 *
 * @param [inout] elements  - the input vector to partition.
 * @param [in]    lo        - the starting index.
 * @param [in]    hi        - the ending index.
 * @param [in]    iter      - the current iteration.
 *
 * @return the index of the pivot element after the vector is partitioned.
 */
template<typename T>
size_t
partition(vector<T> &elements, size_t lo, size_t hi, size_t iter)
{
	size_t i = lo + 1;
	size_t j = hi;

	size_t ncmp = 0;
	size_t nswap = 0;

	while (i <= j) {
		ncmp++;
		if (elements[i] < elements[lo]) {
			i++;
		} else if (elements[j] > elements[lo]) {
			j--;
		} else {
			swap(elements[i], elements[j]);
			i++;
			j--;
			nswap++;
		}
	}

	if (lo != j) {
		swap(elements[lo], elements[j]);
		nswap++;
	}

	print_stats(iter, ncmp, nswap, elements);

	return j;
}

/*
 * Select kth smallest element.
 *
 * @param [inout] elements - the input sequence.
 * @param [in]    k        - the kth smallest element to find.
 *
 * @return the kth smallest element.
 */
template<typename T>
T
select_kth(vector<T> &elements, size_t k)
{
	size_t lo = 0;
	size_t hi = elements.size() - 1;
	size_t iter = 0;

	while (lo <= hi) {
		size_t p = partition(elements, lo, hi, ++iter);
		if (p == k)
			break;
		else if (p < k)
			lo = p + 1;
		else
			hi = p - 1;
	}

	return elements[k];
}

/**
 * Quick Sort. Consider shuffling the items if the sequence is not randomly
 * distributed. Find a pivot element and move it to its correct location
 * using partition(). Recursively sort the first half and the second half.
 *
 * @param [inout] elements  - the vector to sort.
 * @param [in]    lo        - the starting index.
 * @param [in]    hi        - the ending index.
 * @param [in]    iter      - the current iteration.
 *
 * @return elements are sorted on return.
 */
template<typename T>
void
quick_sort_v1(vector<T> &elements, size_t lo, size_t hi, size_t &iter)
{
	if (lo >= hi)
		return;

	size_t p = partition(elements, lo, hi, ++iter);

	if (p > lo)
		quick_sort_v1(elements, lo, p - 1, iter);

	if (p < hi)
		quick_sort_v1(elements, p + 1, hi, iter);
}

/*
 * Perform quick sort.
 *
 * @param [inout] elements  - the vector to sort.
 *
 * @return elements are sorted on return.
 */
template<typename T>
void
quick_sort_v1(vector<T> &elements)
{
	if (elements.empty())
		return;

	size_t iter = 0;

	quick_sort_v1(elements, 0, elements.size() - 1, iter);
}

template<typename T>
void
quick_sort_v2(vector<T> &elements, size_t lo, size_t hi, size_t &iter)
{
	if (lo >= hi)
		return;

	size_t p = lo;
	size_t i = lo + 1;
	size_t j = hi;

	size_t ncmp = 0;
	size_t nswap = 0;

	while (i <= j) {
		ncmp++;
		if (elements[i] < elements[p]) {
			swap(elements[i], elements[p]);
			i++;
			p++;
			nswap++;
		} else if (elements[i] == elements[p]) {
			i++;
		} else /* if (elements[i] > elements[p]) */ {
			swap(elements[i], elements[j]);
			j--;
			nswap++;
		}
	}

	print_stats(++iter, ncmp, nswap, elements);

	if (p > lo)
		quick_sort_v2(elements, lo, p - 1, iter);

	if (j < hi)
		quick_sort_v2(elements, j + 1, hi, iter);
}

/*
 * Perform quick sort.
 *
 * @param [inout] elements  - the vector to sort.
 *
 * @return elements are sorted on return.
 */
template<typename T>
void
quick_sort_v2(vector<T> &elements)
{
	if (elements.empty())
		return;

	size_t iter = 0;

	quick_sort_v2(elements, 0, elements.size() - 1, iter);
}

static int
usage(const char *progname)
{
	cerr << progname << " -in <file> [-string]" << endl
		<< "    -lsd_radix_v1               Perform LSD radix sort (using queues)." << endl
		<< "    -lsd_radix_v2               Perform LSD radix sort (using key index count)." << endl
		<< "    -msd_radix                  Perform MSD radix sort." << endl
		<< "    -bubble                     Perform bubble sort." << endl
		<< "    -selection                  Perform selection sort." << endl
		<< "    -insertion                  Perform insertion sort." << endl
		<< "    -merge_v1                   Perform merge sort recursively." << endl
		<< "    -merge_v2                   Perform merge sort non-recursively." << endl
		<< "    -kth <item>                 Find k-th smallest item." << endl
		<< "    -qsort_v1                   Perform basic quick sort." << endl
		<< "    -qsort_v2                   Perform 3-way quick sort." << endl;
	return 1;
}

enum sort_algo
{
	NONE,
	LSD_RADIX_V1,
	LSD_RADIX_V2,
	MSD_RADIX,
	BUBBLE,
	SELECTION,
	INSERTION,
	MERGE_V1,
	MERGE_V2,
	KTH,
	QSORT_V1,
	QSORT_V2
};

int
main(int argc, const char **argv)
{
	sort_algo       algo = NONE;
	string          file;
	string          tmpstr;
	int             tmpval = -1;
	size_t          k = -1;
	size_t          len = 0;
	bool            is_string = false;
	vector<int>     ivalues;
	vector<string>  svalues;

	for (int i = 1; i < argc; ++i) {
		if (strcmp(argv[i], "-in") == 0) {
			++i;
			if (argv[i]) {
				file = argv[i];
			} else {
				cerr << "missing argument for " << argv[i - 1] << endl;
				return 1;
			}
		} else if (strcmp(argv[i], "-string") == 0) {
			is_string = true;
		} else if (strcmp(argv[i], "-lsd_radix_v1") == 0) {
			algo = LSD_RADIX_V1;
		} else if (strcmp(argv[i], "-lsd_radix_v2") == 0) {
			algo = LSD_RADIX_V2;
		} else if (strcmp(argv[i], "-msd_radix") == 0) {
			algo = MSD_RADIX;
		} else if (strcmp(argv[i], "-bubble") == 0) {
			algo = BUBBLE;
		} else if (strcmp(argv[i], "-selection") == 0) {
			algo = SELECTION;
		} else if (strcmp(argv[i], "-insertion") == 0) {
			algo = INSERTION;
		} else if (strcmp(argv[i], "-merge_v1") == 0) {
			algo = MERGE_V1;
		} else if (strcmp(argv[i], "-merge_v2") == 0) {
			algo = MERGE_V2;
		} else if (strcmp(argv[i], "-kth") == 0) {
			algo = KTH;
			i++;
			if (argv[i]) {
				int n = atoi(argv[i]);
				if (n < 0) {
					cerr << "invalid argument for " << argv[i - 1] << endl;
					return 1;
				}
				k = n;
			} else {
				cerr << "missing argument for " << argv[i - 1] << endl;
				return 1;
			}
		} else if (strcmp(argv[i], "-qsort_v1") == 0) {
			algo = QSORT_V1;
		} else if (strcmp(argv[i], "-qsort_v2") == 0) {
			algo = QSORT_V2;
		} else {
			return usage(argv[0]);
		}
	}

	if (algo == NONE) {
		cerr << "sorting algorithm is not specified" << endl;
		return usage(argv[0]);
	}

	if (file.empty()) {
		cerr << "input file is not specified using -in." << endl;
		return usage(argv[0]);
	}

	size_t max_width = 0;

	fstream fin(file, ios_base::in);
	while (fin) {
		if (is_string) {
			fin >> tmpstr;
			if (!tmpstr.empty()) {
				max_width = max(max_width, tmpstr.length());
				svalues.push_back(tmpstr);
				tmpstr.clear();
			}
		} else {
			fin >> tmpval;
			if (tmpval != -1) {
				ivalues.push_back(tmpval);
				tmpval = -1;
			}
		}
	}
	fin.close();

	cout << "input : ";
	if (is_string) {
		len = svalues.size();
		cout << svalues;
	} else {
		len = ivalues.size();
		cout << ivalues;
	}

	cout << endl;


	if (algo == KTH) {
		if (k >= len) {
			cerr << "kth value, " << k << ", is beyond range, " << len << endl;
			return 1;
		}

		if (len == 0) {
			cerr << "empty sequence" << endl;
			return 1;
		}

		if (is_string)
			cout << select_kth(svalues, k) << endl;
		else
			cout << select_kth(ivalues, k) << endl;

		return 0;
	}

	switch (algo) {
		case LSD_RADIX_V1:
			if (!is_string)
				cerr << "LSD radix sort is applicable to strings only." << endl;
			else
				lsd_radix_sort_v1(svalues, max_width);
			break;

		case LSD_RADIX_V2:
			if (!is_string)
				cerr << "LSD radix sort is applicable to strings only." << endl;
			else
				lsd_radix_sort_v2(svalues, max_width);
			break;

		case MSD_RADIX:
			if (!is_string)
				cerr << "MSD radix sort is applicable to strings only." << endl;
			else
				msd_radix_sort(svalues, max_width);
			break;

		case BUBBLE:
			if (is_string)
				bubble_sort(svalues);
			else
				bubble_sort(ivalues);
			break;

		case SELECTION:
			if (is_string)
				selection_sort(svalues);
			else
				selection_sort(ivalues);
			break;

		case INSERTION:
			if (is_string)
				insertion_sort(svalues);
			else
				insertion_sort(ivalues);
			break;

		case MERGE_V1:
			if (is_string)
				merge_sort_v1(svalues);
			else
				merge_sort_v1(ivalues);
			break;

		case MERGE_V2:
			if (is_string)
				merge_sort_v2(svalues);
			else
				merge_sort_v2(ivalues);
			break;

		case QSORT_V1:
			if (is_string)
				quick_sort_v1(svalues);
			else
				quick_sort_v1(ivalues);
			break;

		case QSORT_V2:
			if (is_string)
				quick_sort_v2(svalues);
			else
				quick_sort_v2(ivalues);
			break;
			break;

		default:
			break;
	}

	cout << "output: ";
	if (is_string)
		cout << svalues;
	else
		cout << ivalues;
	cout << endl;

	return 0;
}
