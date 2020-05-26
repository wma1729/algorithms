# Dynamic Programming

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
j:      [1, 8]
Input:  2   4   3   5   1   7   6   9   8
Max:    1   2   2   2   1   2   2   2   2
Index: -1   0   0   0  -1   0   0   0   0
```

Now do the same for the next element, 4.
```
i:      1
j:      [2, 8]
Input:  2   4   3   5   1   7   6   9   8
Max:    1   2   2   3   1   3   3   3   3
Index: -1   0   0   1  -1   1   1   1   1
```

Next do the same for the next element, 3.
```
i:      2
j:      [3, 8]
Input:  2   4   3   5   1   7   6   9   8
Max:    1   2   2   3   1   3   3   3   3
Index: -1   0   0   1  -1   1   1   1   1
```

What? The max values are not incremented. Why? Because they were already more than the max value of 3.
> Max[j] = max(Max[j], Max[i] + 1)

Here is the work-out for the remaining sequence:
```
i:      3
j:      [4, 8]
Input:  2   4   3   5   1   7   6   9   8
Max:    1   2   2   3   1   4   4   4   4
Index: -1   0   0   1  -1   3   3   3   3

i:      4
j:      [5, 8]
Input:  2   4   3   5   1   7   6   9   8
Max:    1   2   2   3   1   4   4   4   4
Index: -1   0   0   1  -1   3   3   3   3

i:      5
j:      [6, 8]
Input:  2   4   3   5   1   7   6   9   8
Max:    1   2   2   3   1   4   4   5   5
Index: -1   0   0   1  -1   3   3   5   5

i:      6
j:      [7, 8]
Input:  2   4   3   5   1   7   6   9   8
Max:    1   2   2   3   1   4   4   5   5
Index: -1   0   0   1  -1   3   3   5   5

i:      7
j:      [8, 8]
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
This algorithm needs little additional space. But the sequence is scanned *(n - 1) + (n - 2) + ... + 2 + 1* times, resulting in complexity of *n<sup>2</sup>*.

### Approach 3 (using Patience sort)
Create piles of numbers in descending order as they are processed. If a higher number is encountered, a new pile is created. Also, for each item, we maintain a predecesor item (which is the top item from the previous pile).

> Input:        2  4  3  5  1  7  6  9  8

Predecessor is set to -1 for all items and there are no piles yet.
```
Input :  2   4   3   5   1   7   6   9   8
pred  : -1  -1  -1  -1  -1  -1  -1  -1  -1
piles :  None
```

Now add one item at a time:
```
item     : 2
piles[0] : (2, pred[2] = -1)

item     : 4
piles[0] : (2, pred[2] = -1) 
piles[1] : (4, Pred[4] = 2, top item from previous pile)

item     : 3
piles[0] : (2, pred[2] : -1)
piles[1] : (4, pred[4] = 2), (3, pred[3] = 2)

item     : 5
piles[0] : (2, pred[2] : -1)
piles[1] : (4, pred[4] = 2), (3, pred[3] = 2)
piles[2] : (5, pred[5] = 3)

item     : 1
piles[0] : (2, pred[2] : -1), (1, pred[1] = -1)
piles[1] : (4, pred[4] = 2), (3, pred[3] = 2)
piles[2] : (5, pred[5] = 3)

item     : 7
piles[0] : (2, pred[2] : -1), (1, pred[1] = -1)
piles[1] : (4, pred[4] = 2), (3, pred[3] = 2)
piles[2] : (5, pred[5] = 3)
piles[3] : (7, pred[7] = 5)

item     : 6
piles[0] : (2, pred[2] : -1), (1, pred[1] = -1)
piles[1] : (4, pred[4] = 2), (3, pred[3] = 2)
piles[2] : (5, pred[5] = 3)
piles[3] : (7, pred[7] = 5), (6, [pred[6] = 3)

item     : 9
piles[0] : (2, pred[2] : -1), (1, pred[1] = -1)
piles[1] : (4, pred[4] = 2), (3, pred[3] = 2)
piles[2] : (5, pred[5] = 3)
piles[3] : (7, pred[7] = 5), (6, pred[6] = 3)
piles[4] : (9, pred[9] = 6)

item     : 8
piles[0] : (2, pred[2] : -1), (1, pred[1] = -1)
piles[1] : (4, pred[4] = 2), (3, pred[3] = 2)
piles[2] : (5, pred[5] = 3)
piles[3] : (7, pred[7] = 5), (6, pred[6] = 3)
piles[4] : (9, pred[9] = 6), (8, pred[8] = 6)
```

There are 5 files and that is the number of items in the longest increasing subsequence. The subsequence itself can be generated by picking the top element from the last pile and following predecessor to fetch all elements in the sequence. Below is an implementation. For convenience, instead of storing the number itself in *piles* and *pred*, the item index in the sequence is stored.
```C++
/*
 * Longest increasing subsequence (using Patience sort)
 */
vector<int>
lis_v3(const vector<int> &seq)
{
	vector<int> lis;

	if (seq.empty())
		return lis;

	/*
	 * Piles of numbers in descending order.
	 * We only keep the index of the number in the piles
	 * and not the number itself.
	 */
	vector<stack<int>> piles;

	/*
	 * Index of predecessor number from the
	 * preceeding pile.
	 */
	vector<int> pred(seq.size(), -1);

	for (size_t i = 0; i < seq.size(); ++i) {
		size_t j;

		for (j = 0; j < piles.size(); ++j) {
			if (seq[piles[j].top()] > seq[i])
				break;
		}

		if (j == piles.size()) {
			// start a new pile
			stack<int> stk;
			stk.push(static_cast<int>(i));
			piles.push_back(stk);
		} else {
			// use an existing pile
			piles[j].push(static_cast<int>(i));
		}

		// set the predecessor index at the time of each insert
		if (j > 0) 
			pred[i] = static_cast<int>(piles[j - 1].top());
	}

	// form the sequence going back from the last -> first pile
	int idx = piles.back().top();
	while (idx != -1) {
		lis.push_back(seq[idx]);
		idx = pred[idx];
	}

	// reverse the sequence
	reverse(lis.begin(), lis.end());

	return lis;
}
```
The complexity of this algorithm is no better than the first approach. But it can be modified to use binary search to find the correct pile for inserting next item. With binary search in place, the complexity is *n * log<sub>2</sub>(n)*.

## 0/1 Knapsack Problem
*Problem:* There are *n* items with different weights, *W = { w<sub>1</sub>, w<sub>2</sub>, ..., w<sub>n</sub> }*, and different values, *V = { v<sub>1</sub>, v<sub>2</sub>, ..., v<sub>n</sub> }*. The problem is to pack the items in a given knapsack of capacity, *C*, in a way that the knapsack has the maximum value.

*Solution:* As can be seen, the solution involves extensive search. We need to try all possible combinations making sure that the total capacity does not exceed the capacity of the knapsack. The combination that gives us the maximum value is our solution.

### Approach 1 (crude backtracking)
Let the first weight and value be 0. This helps in calculations later.
```
int w[] = { 0, 1, 4, 2, 5, 2 };
int v[] = { 0, 5, 5, 3, 2, 3 };
```
We start with the last item and calculate the knapsack value by both excluding and including the item.
```C++
/*
 * The 0/1 knapsack problem.
 *
 * @param n - the number of items.
 * @param C - the capacity of the knapsack.
 * @param w - weights of the items.
 * @param v - values of the items.
 *
 * @return the maximum value of the knapsack.
 */
int
knapsack_v1(int n, int C, int w[], int v[])
{
	int V; // Cumulative value of the knapsack so far.

	if ((n == 0) || (C == 0)) {
		/*
		 * If no more item left OR knapsack has no more capacity,
		 * nothing more to do.
		 */
		V = 0;
	} else if (w[n] > C) {
		/*
		 * If the current item is heavier than what knapsack
		 * can carry, leave this item and try the next one.
		 */
		V = knapsack_v1(n - 1, C, w, v);
	} else {
		/*
		 * If this item can be included, calculate the knapsack
		 * value by excluding this item and by including this
		 * item. Choose the maximum of the two.
		 */
		int v_excl = knapsack_v1(n - 1, C, w, v);
		int v_incl = v[n] + knapsack_v1(n - 1, C - w[n], w, v);
		V = max(v_excl, v_incl);
	}

	return V;
}
```
