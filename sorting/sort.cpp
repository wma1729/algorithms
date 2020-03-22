#include <iostream>
#include <ostream>
#include <sstream>
#include <fstream>
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
 * @return elements is sorted on return.
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
 * @return elements is sorted on return.
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
 * @return elements is sorted on return.
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

#if defined(DEBUG)
	cout << "lo = " << lo << ", hi = " << hi << endl;
#endif // DEBUG

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
	cout << "w = " << w + 1 << " : " << elements << endl;
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
 * @return elements is sorted on return.
 */
void
msd_radix_sort(vector<string> &elements, size_t max_width)
{
	vector<string> aux;     // auxiliary vector of strings
	msd_radix_sort(elements, aux, 0, elements.size(), 0, max_width);
}

static int
usage(const char *progname)
{
	cerr << progname << " -in <file> [-string]" << endl
		<< "    -lsd_radix_v1               Perform LSD radix sort (using queues)." << endl
		<< "    -lsd_radix_v2               Perform LSD radix sort (using key index count)." << endl
		<< "    -msd_radix                  Perform MSD radix sort." << endl;
	return 1;
}

enum sort_algo
{
	NONE,
	LSD_RADIX_V1,
	LSD_RADIX_V2,
	MSD_RADIX
};

int
main(int argc, const char **argv)
{
	sort_algo       algo = NONE;
	string          file;
	string          tmpstr;
	int             tmpval = -1;
	bool            is_string = false;
	vector<int>     ivalues;
	vector<string>  svalues;

	for (int i = 1; i < argc; ++i) {
		if (strcmp(argv[i], "-in") == 0) {
			++i;
			if (argv[i]) {
				file = argv[i];
			} else {
				cerr << "missing argument for " << argv[i] << endl;
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
	if (is_string)
		cout << svalues;
	else
		cout << ivalues;
	cout << endl;

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
