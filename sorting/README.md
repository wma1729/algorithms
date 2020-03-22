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

Input | w = 3 | w = 2 | w = 1
------|-------|-------|------
ram   | mad   | mad   | all
mad   | she   | ram   | bar
she   | the   | car   | car
him   | all   | bar   | cat
the   | ram   | cat   | him
cat   | him   | she   | mad
car   | car   | the   | ram
bar   | bar   | him   | she
sky   | cat   | sky   | sky
all   | sky   | all   | the

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
 * LSD radix sort. The sort uses multiple queues:
 * 1) main queue.
 * 2) one for every possible character [0-255].
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
	array<queue<string>, N> queues;

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
	 * based on the character at index i - 1.
	 */
	for (size_t i = max_width; i > 0; --i) {
		/*
		 * Read the strings from the main queue and
		 * push the strings in the correct queue.
		 */
		while (!mainq.empty()) {
			string s = mainq.front();
			mainq.pop();
			queues[char_at(s, i - 1)].push(s);
		}

		/*
		 * Copy the strings back from auxiliary queues
		 * to the main queue.
		 */
		for (size_t j = 0; j < N; ++j) {
			while (!queues[j].empty()) {
				mainq.push(queues[j].front());
				queues[j].pop();
			}
		}

#if defined(DEBUG)
		cout << "i = " << i << " : " << mainq << endl;
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
	constexpr int N = 257;  // A trick
	vector<string> aux;     // auxiliary vector of strings
	array<int, N> count;    // count of valid characters: one for each character.

	/*
	 * Repeat max_width times from least significant digit
	 * (LSD) to most significant digit (MSD) sorting strings
	 * based on the character at index i - 1.
	 */
	for (size_t i = max_width; i > 0; --i) {
		// character index we are dealing with.
		int idx = i - 1;

		// copy all elements to the auxiliary vector.
		for (auto e : elements)
			aux.push_back(e);

		// initialize all the counters to 0.
		for (size_t c = 0; c < N; ++c)
			count[c] = 0;

		/*
		 * Set the count of each character. There is a trick here.
		 * Instead of setting the count[char_at(a, idx)], it sets
		 * count[char_at(a, idx) + 1].
		 */
		for (auto a : aux)
			count[char_at(a, idx) + 1]++;

		// set the cumulative count.
		for (size_t c = 0; c < N - 1; ++c)
			count[c + 1] += count[c];

		/*
		 * Now count[char_at(a, idx)] represents the index where the
		 * string should be copied in the main vector. count[char_at(a, idx)]
		 * is incremented to represent the next index in case the character
		 * is seen again.
		 */
		for (auto a : aux)
			elements[count[char_at(a, idx)]++] = a;

		aux.clear();

#if defined(DEBUG)
		cout << "i = " << i << " : " << elements << endl;
#endif // DEBUG
	}

	return;
}
```

## MSD Radix Sort (Radix Exchange Sort)

