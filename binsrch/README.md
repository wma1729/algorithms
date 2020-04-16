# Binary Search

## Iterative binary search
A very simple search operation performed on a sorted sequence, a perfect example of *divide-and-conquer* family of algorithms.

*Problem:* Given a sorted input sequence of *n* elements, *S = { i<sub>0</sub>, i<sub>1</sub>, ..., i<sub>n - 1</sub> }*, find *key* in it.<br>
*Solution:* *key* is compared with the *middle* element. There possible outcomes:
* *key == S[middle]*
  * *key* is found at index *middle*.
* *key < S[middle]*
  * Repeat the process in *S[0, middle - 1]*.
* *key > S[middle]*
  * Repeat the process in *S[middle + 1, n - 1]*.

In each step, we are dividing the search space in half. So the complexity is *log<sub>2</sub>(n)*.
```C++
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
```

## Recursive binary search
There is rarely a need to do a recursive version of this but it can be simply done as following:
```C++
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

	return binary_search_v2(seq, 0, seq.size(), key);
}
```

## Sequence rotation
*Problem:* Given a sequence of *n* elements, rotate the sequence by *p* elements.<br>
*Solution:* This problem has an interesting answer. The sequence is assumed to be composed of two sub-sequences: *S[0, p - 1], S[p, n - 1]*. The solution consists of three steps:
* Reverse the sequence *S[0, p - 1]*.
* Reverse the sequence *S[p, n - 1]*.
* Reverse the sequence *S[0, n - 1]*.
```C++
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
 * @param [in]    p   - how much to rotate? 1 <= n < seq.size()
 *
 * @return the sequence is rotated n position.
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
```
