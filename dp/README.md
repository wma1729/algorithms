#--- Dynamic Programming

## Longest Increasing Subsequence
*Problem:* Given a sequence of *n* integers, *S = { i<sub>1</sub>, i<sub>2</sub>, i<sub>3</sub>, ..., i<sub>n - 1</sub>, i<sub>n</sub> }*, an increasing subsequence, *LIS = { i<sub>m1</sub>, i<sub>m2</sub>, ..., i<sub>mk</sub> }*, is such that<br>
> *i<sub>mj</sub> < i<sub>m(j + 1)</sub>* for all *1 <= j < k*

There might me many such subsequences. The longest increasing subsequence in an increasing subsequence of maximum length.

*Solution:* There are various ways to solve this problem.

### Approach 1 (maintain best increasing subsequences seen so far)
In this approach, one item is processed at a time. A list of increasing subsequences is maintained as we proceed with the algorithm. The subsequences are arranged in the best pick order for the next item. The sort order is:
- the longest subsequence, say of size *s*, followed by subsequence(s) of size, *s - 1*, and so on.
- if there are two subsequences of the same size, the subsequence with the smaller last item comes first.
```C++
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
```
To add an item, *i<sub>k</sub>*, we find an increasing subsequence, where the last element is less than *i<sub>k</sub>*. Whenever we extend an increasing subsequence, we retain the original subsequence as well as it might be a better candidate for a future item.
```C++
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
```
Input             | Increasing Subsequences
------------------|------------------------
2 4 3 5 1 7 6 9 8 | 2, 3, 5, 6, 8<br>2, 3, 5, 6, 9<br>2, 3, 5, 6<br>2, 3, 5, 7<br>2, 3, 5<br>2, 3<br>2, 4<br>1<br>2<br>
11 14 13 7 8 15   | 7, 8, 15<br>7, 8<br>11, 13<br>11, 14<br>7<br>11<br>

Now that we know how to add an item to the best increasing subsequence, the algorithm is really straight-forward:
```C++
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

	return sequences[0];
}
```
This approach is really simple to understand though not the best. Sorting after processing each item is a killer here, though using a stable sort helps.

### Approach 2 (Dynamic Programming)
For each item in the sequence, a corresponding LIS info is maintained. The LIS info contains:
- the maximum length of the subsequence seen so far.
- the index of the previous item.

> Input: 2 4 3 5 1 7 6 9 8

Max is set to 1 and previous index is set to -1 for all items at the start.
```
Input:   2   4   3   5   1   7   6   9   8
Max:     1   1   1   1   1   1   1   1   1
Index:  -1  -1  -1  -1  -1  -1  -1  -1  -1
```

Take the first element, 2, and find all the elements that are greater than it.
```
i:      0
Input:  2   4   3   5   1   7   6   9   8
Max:    1   2   2   2   1   2   2   2   2
Index: -1   0   0   0  -1   0   0   0   0
```

Now do the same for the next element, 4.
```
i:      1
Input:  2   4   3   5   1   7   6   9   8
Max:    1   2   2   3   1   3   3   3   3
Index: -1   0   0   1  -1   1   1   1   1
```

Next do the same for the next element, 3.
```
i:      2
Input:  2   4   3   5   1   7   6   9   8
Max:    1   2   2   3   1   3   3   3   3
Index: -1   0   0   1  -1   1   1   1   1
```

What? The max values are not incremented. Why? Because they were already more than the max values of 3.
> Max[j] = max(Max[j], Max[i] + 1)

Here is the work-out for the remaining sequence:
```
i:      3
Input:  2   4   3   5   1   7   6   9   8
Max:    1   2   2   3   1   4   4   4   4
Index: -1   0   0   1  -1   3   3   3   3

i:      4
Input:  2   4   3   5   1   7   6   9   8
Max:    1   2   2   3   1   4   4   4   4
Index: -1   0   0   1  -1   3   3   3   3

i:      5
Input:  2   4   3   5   1   7   6   9   8
Max:    1   2   2   3   1   4   4   5   5
Index: -1   0   0   1  -1   3   3   5   5

i:      6
Input:  2   4   3   5   1   7   6   9   8
Max:    1   2   2   3   1   4   4   5   5
Index: -1   0   0   1  -1   3   3   5   5

i:      7
Input:  2   4   3   5   1   7   6   9   8
Max:    1   2   2   3   1   4   4   5   5
Index: -1   0   0   1  -1   3   3   5   5
```

Knowing the max value and previous indices, the longest increasing subsequence could be easily constructed.
```C++
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

	while (IDX >= 0) {
		lis.push_back(seq[IDX]);
		IDX = lisinfo[IDX].idx;
	}

	reverse(lis.begin(), lis.end());

	return lis;
}
```
