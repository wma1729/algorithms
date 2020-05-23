# Dynamic Programming

## Longest Increasing Sequence
*Problem:* Given a sequences of *n* integers, *S = { i<sub>1</sub>, i<sub>2</sub>, i<sub>3</sub>, ..., i<sub>n - 1</sub>, i<sub>n</sub> }*, an increasing sequence, *LIS = { i<sub>m1</sub>, i<sub>m2</sub>, ..., i<sub>mk</sub> }*, is such that<br>
> *i<sub>mj</sub> < i<sub>m(j + 1)</sub>* for all *1 <= j < k*

There might me many such sequences. The longest increasing sequence in an increasing sequence of maximum length.

*Solution:* There are various ways to solve this problem.

### Approach 1 (maintain best increasing sequences seen so far)
In this approach, one item is processed at a time. A list of increasing sequences is maintained as we proceed with the algorithm. The sequences are arranged in the best pick order for the next item. The sort order is:
- the longest sequence, say of size *s*, followed by sequence(s) of size, *s - 1*, and so on.
- if there are two sequences of the same size, the sequence with the smaller last item comes first.
```C++
/*
 * Comparator to sort LISes in the best
 * pick order for appending the next item.
 */
class best_lis_cmp
{
public:
        /*
         * The longest sequence should be first.
         * If two or more sequences have the same size,
         * the sequence with the smaller last item wins.
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
To add an item, *i<sub>k</sub>*, we find an increasing sequence, where the last element is less than *i<sub>k</sub>*. Whenever we extend an increasing sequence, we retain the original sequence as well as it might be a better candidate for a future item.
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
                        // save a copy of existing sequence
                        newseq = seq;

                        // add item to this sequence
                        seq.push_back(item);
                        inserted = true;
                        break;
                }
        }

        if (!inserted)
                newseq.push_back(item);

        if (!newseq.empty())
                sequences.push_back(newseq);

        // sort the sequence for the next item
        sort(sequences.begin(), sequences.end(), best_lis_cmp());
}
```
Now that we know how to add an item to the best increasing sequence, the algorithm is really straight-forward:
```C++
/*
 * Longest increasing sequence.
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
Input             | Increasing Sequences
------------------|---------------------
2 4 3 5 1 7 6 9 8 | 2, 3, 5, 6, 8<br>2, 3, 5, 6, 9<br>2, 3, 5, 6<br>2, 3, 5, 7<br>2, 3, 5<br>2, 3<br>2, 4<br>1<br>2<br>
11 14 13 7 8 15   | 7, 8, 15<br>7, 8<br>11, 13<br>11, 14<br>7<br>11<br>
