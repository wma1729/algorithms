# Sorting

## Stable Sort
A **stable** sort preserves the original ordering of identical items.

```
Input     : [(a, 10), (b, 7), (c, 8), (d, 7), (e, 6), (f, 10), (g, 3)]
Operation : Sort by the second numberic field.
Output    : [(g, 3), (e, 6), (b, 7), (d, 7), (c, 8), (a, 10), (f, 10)]
```

The stablity property of the sorting algorith guarantees that the relative ordering of [(b, 7), (d, 7)] and [(a, 10), (f, 10)] is preserved. Note (b, 7) appears before (d, 7) both in the input and the output. Similarly (a, 10) appears before (f, 10) in both the input and the output.

## Radix Sort

Input: Sequence of *n* items, each item with *w* digits. Each digit is in the range *0 - m*.

Radix sort is often used when the keys to sort are small strings. Let's see a sample input set:

```
Input: ram, mad, she, him, the, cat, car, bar, sky, all
n - 10, there are 10 items to sort.
w - 3, each item has 3 digits.
m - 255, each digit can have values in the range 0 - 255.
```

## LSD Radix Sort (Straight Radix Sort)
The digits of items are scanned from the least significant digit (LSD) to the most significant digit (MSD).
```
input : ram, mad, she, him, the, cat, car, bar, sky, all
w = 3 : mad, she, the, all, ram, him, car, bar, cat, sky
w = 2 : mad, ram, car, bar, cat, she, the, him, sky, all
w = 1 : all, bar, car, cat, him, mad, ram, she, sky, the
output: all, bar, car, cat, him, mad, ram, she, sky, the
```

This is a stable sort and complexity is *nw*.

This works great for strings of equal length. What to do for strings of variable lengths? We can assume that the all strings are as long as the longest string in the input and return 0 for the overflowing w.

```C++
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
```

### Implementation using queues.
```C++
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
```

### Implementation using key index counting sort.
LSD radix sort using queues is simple to understand and easy to implement. However, it needs a lot of additional space. Can we do better? In the following counting sort approach, we use an integer array of size (256 + 1). As before, we scan from LSD to MSD. Instead of adding strings to the queue, we increment the corresponding array element count.
```
input    : ram, mad, she, him, the, cat, car, bar, sky, all
i        :  0 a b c d e f g h i j k l m n o p q r s t u v w x y z {
count[i] :  0 0 0 0 0 1 2 0 0 0 0 0 0 1 2 0 0 0 0 2 0 1 0 0 0 0 1 0
```
Please note that instead of incremented *count[N]*, *count[N + 1]* is incremented. There is a trick involved here. Next, we generate a cumulative count.
```
input    : ram, mad, she, him, the, cat, car, bar, sky, all
i        :  0 a b c d e f g h i j k l m n o p q r s t u v w x y z {
count[i] :  0 0 0 0 0 1 3 3 3 3 3 3 3 4 6 6 6 6 6 8 8 9 9 9 9 91010
```
Now *count[N]* gives us the index where the item should go. And we do need increment *count[N]* after copying the item. Here is a complete implementation:
```C++
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
```

## MSD Radix Sort (Radix Exchange Sort)
The digits of items are scanned from the least significant digit (LSD) to the most significant digit (MSD).
```
input : ram, mad, she, him, the, cat, car, bar, sky, all
w = 1 : all, bar, cat, car, him, mad, ram, she, sky, the
w = 2 : all, bar, cat, car, him, mad, ram, she, sky, the
w = 3 : all, bar, car, cat, him, mad, ram, she, sky, the
output: all, bar, car, cat, him, mad, ram, she, sky, the
```
The algorithm implementation is recursive and messy. The complexity is between *n* and *nw* or *n(1 + w) / 2*.
```C++
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
	// cout << "lo = " << lo << ", hi = " << hi << endl;
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
```
