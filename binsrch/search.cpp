#include <iostream>
#include <vector>

using namespace std;

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

template<typename T>
int
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

template<typename T>
int
binary_search_v2(const vector<T> &seq, const T &key)
{
	if (seq.empty())
		return -1;

	return binary_search_v2(seq, 0, seq.size(), key);
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

	return 0;
}
