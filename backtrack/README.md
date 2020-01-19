# Backtracking

Some problems involve iterating over the entire search space. Backtracking is a technique used to find a solution efficiently for such combinatorial problems. The efficiency comes from not visiting the same state more than once. The best way to understand **backtracking** is through examples.

## N-queen problem
The queen in a chess-board has three possible moves:
* horizontal
* vertical
* diagnal

*Problem:* Put N queens in a N * N chessboard such that they are not a threat to each other.<br>
*Solution:* Let the chess be represented by a 2-dimensional boolean array, *board*. We will scan the board row-by-row placing a queen in a row/column such that is safe there. If we find no such position, we will backtrack. In the following code snippet: *valid_position(i, j)* determines if the i<sup>th</sup> row and j<sup>th></sup> column is a safe place for the queen.
```C++
/*
 * Solves the n-queen problem.
 */
void solve(int i, bool all)
{
	if (i == num) {
		// All the rows are tried. We have a possible solution.
		sol++;

		// Dump the solution.
		dump();
	} else {
		for (int j = 0; j < num; ++j) {
			if (valid_position(i, j)) {
				// Found the correct column in the ith row.
				board[i][j] = true;

				// Go solve for the next row
				solve(i + 1, all);
				if (sol && !all)
					return;

				// Backtrack
				board[i][j] = false;
			} else {
				// Try the next column. board[i][j] is not safe.
			}
		}
	}
}

/*
 * Solve n-queen problem. num has the number of rows/columns.
 * @param all find all solutions if true else return as soon as the first
 *            solution is found.
 * @retturn the number of solutions.
 */
int solve(bool all)
{
	solve(0, all);

	cout << num << "-queen problem has " << sol << " solutions" << endl;

	return sol;
}
```
Now for 4-queen problem, we have two solutions:
```
# nqueen -n 4 -a
- Q - - 
- - - Q 
Q - - - 
- - Q - 

- - Q - 
Q - - - 
- - - Q 
- Q - - 

4-queen problem has 2 solutions.
```

## Sudoku
Sudoku is a N * N board with N rows and N columns that needs to be filled with numbers from [1 - N]. Some numbers are pre-filled. The remaining numbers are to be deduced. The placement of a number must meet the following constraints:
* The number is unique in the row.
* The number is unique in the column.
* The number is unique in the box (a sub board √N * √N).

This is quite similar to the N-queen problem. Instead of the placement of queue, we are talking about placement of numbers.

*Problem:* Solve N * N sudoku board.<br>
*Solution:* Let the sudoku board be represented by a 2-dimensional integer array, board. We will scan the board row-by-row placing a queen in a row/column such that is safe there. If we find no such position, we will backtrack. In the following code snippet: *is_valid(r, c, val)* determines if it is okay to put val in the r<sup>th</sup> row and r<sup>th></sup> column. The empty numbers are set to 0 to indicate that we need to fill them.
```C++
/*
 * Solve the sudoku problem.
 *
 * @param board the sudoku board.
 * @param r     the current row we are solving.
 * @param r     total number of rows (equal to n).
 * @param r     the current column we are solving.
 * @param r     total number of columns (equal to n).
 *
 * @return true if we found a solutions, false otherwise.
 */
bool solve(int **board, int r, int rows, int c, int cols)
{
	if (c == cols) {
		// Done with column; move on to the next row.
		c = 0;
		r++;

		if (r == rows) {
			// We are done with all rows; Print the solution.
			dump();
			return true;
		}
	}

	// If the number is pre-filled, move on to the next column.
	if (board[r][c] != 0) 
		return solve(board, r, rows, c + 1, cols);

	// Iterate over all values.
	for (int v = 1; v <= rows; ++v) {
		if (is_valid(r, c, v)) {
			// Place the number.
			board[r][c] = v;

			// Solve for remaining numbers.
			if (solve(board, r, rows, c + 1, cols))
				return true;

			// Backtrack
			board[r][c] = 0;
		}
	}

	return false;
}

/*
 * Solve the sudoku problem.
 *
 * @param board the sudoku board.
 * @param n     size of the sudoku board.
 *
 * @return true if we found a solutions, false otherwise.
 */
bool solve(int **board, int n)
{
	return solve(board, 0, n, 0, n);
}
```

Here is a solution for a given sudoku. The program first display the problem (number 0 indicates empty slots) and then the solution.
```
# sudoku
0 6 0 4 0 7 0 1 0
2 4 0 9 0 5 6 0 0
7 0 0 6 8 0 2 0 4
0 3 7 0 0 8 0 0 2
0 0 6 1 3 0 4 0 0
9 1 0 0 0 0 5 3 0
0 0 0 0 0 0 0 9 1
0 0 0 0 0 0 0 0 0
0 5 0 0 4 0 0 0 0

8 6 5 4 2 7 9 1 3
2 4 3 9 1 5 6 8 7
7 9 1 6 8 3 2 5 4
4 3 7 5 9 8 1 6 2
5 8 6 1 3 2 4 7 9
9 1 2 7 6 4 5 3 8
3 7 4 2 5 6 8 9 1
6 2 9 8 7 1 3 4 5
1 5 8 3 4 9 7 2 6

Found a solution!
```

## Permutations of a string
*Problem:* Find all possible permutations of a given string.<br>
*Solution:* The solution involves trying all possible solution. Let's assume the given string is *abc*.<br>
* Take *a* and permutate *bc*. Do the same for the remaining string *bc*.
* Take *b* and permutate *ac*. Do the same for the remaining string *ac*.
* Take *c* and permutate *ab*. Do the same for the remaining string *ab*.

And this gives us the following simple algorithm.
```C++
/*
 * Find all permutations of the given string.
 *
 * @param remaining remaining string excluding characters that are arranged.
 * @param perm      characters already permutated.
 *
 * Invariant: length(remaining) + length(perm) = length(original string).
 */
void permutate(const string &remaining, string &perm)
{
	if (remaining.empty()) {
		// No character left in the string. Print this permutation.
		cout << perm << endl;
		return;
	}

	for (size_t i = 0; i < remaining.length(); ++i) {
		// Consume the first character.
		perm.push_back(remaining[i]);

		// Permutate the remaining string.
		permutate(remaining.substr(0, i) + remaining.substr(i + 1), perm);

		// Backtrack.
		perm.pop_back();
	}

	return;
}

/*
* Find all permutations of the given string.
*
* @param str input string.
*/
void permutate(const string &str)
{
	string p;
	permutate(str, p);
}

```
*abcd* has the following permutations possible:
```
# permutations abcd
abcd
abdc
acbd
acdb
adbc
adcb
bacd
badc
bcad
bcda
bdac
bdca
cabd
cadb
cbad
cbda
cdab
cdba
dabc
dacb
dbac
dbca
dcab
dcba
```

## Subset of a string
*Problem:* Find all subsets of a given string.<br>
*Solution:* For each charater, we have two choices: leave it or take it in the subset. We try both possibilities for each character in the given string. First create a 2-element boolean array and initialize the elements to false (leave it) and true (take it). The array is then iterated over for each element of the given string giving us the following algorithm:
```C++
bool choices[2];
choices[0] = false; // leave it
choices[1] = true;  // take it

/*
 * Generate subsets of the given string.
 *
 * @param current_subset current subset so far.
 * @param str            input string.
 * @param idx            current string index.
 * @param max            input string length.
 */
void generate(string &current_subset, const string &str, size_t idx, size_t len)
{
	if (idx == len) {
		// No character left in the string. Print this subset.
		cout << "{" << current_subset << "}" << endl;
		return;
	}

	for (size_t i = 0; i < sizeof(choices) / sizeof(bool); ++i) {
		// Either leave or take the ith character.
		if (choices[i]) current_subset.push_back(str[idx]);

		// Generate subset for the remaining string.
		generate(current_subset, str, idx + 1, len);

		// Backtrack
		if (choices[i]) current_subset.pop_back();
	}
}

/*
 * Generate subsets of the given string.
 */
void generate(const string &str)
{
	string current_subset;
	generate(current_subset, str, 0, str.length());
}
```
*abc* has the following subsets:
```
# subset abc
{}
{c}
{b}
{bc}
{a}
{ac}
{ab}
{abc}
```

# Word break
*Problem:* We are given a dictionary. The dictionary provides a method *contains()* which tells us if the dictionary contains a given word. Find if a given string can be broken down into valid words from the dictionary. Let's assume that the dictionary contains: *cats, sand, and, dog, cat*. If the input string is *catsandog*, it cannot be broken. However, if the input string is *catsanddog*, it can be.<br>
*Solution:* Again we will try the brute force. First try to find words of length 1, then length 2, and so on upto words of length n where n is the length of the given string. Every time we find a word, we do the same for the rest of the string. Using this approach gives us the following algorithm:
```C++
/*
 * Determines if the given string could be broken into
 * words from a given dictionary.
 *
 * @param to_break input string.
 * @param dict     reference to a dictionary.
 *
 * @return true if the string can be broken or false if the
 * word cannot be broken into words from a dictionary.
 */
bool
word_break(const string &to_break, const dictionary &dict)
{
	if (to_break.empty()) {
		// No more characters left. Return success
		return true;
	}

	for (size_t len = 1; len <= to_break.size(); ++len) {
		// Break the string in two: left (length = len) and right (rest if the string)
		string left = to_break.substr(0, len);
		string right = to_break.substr(len);

		if (dict.contains(left)) {
			// If found left, try to do the same for right.
			if (word_break(right, dict))
				return true;
		}
	}

	return false;
}
```
