# Backtracking

Some problems involve iterating over the entire search space. Backtracking is a technique used to find a solution efficiently for such combinatorial problems. The efficiency comes from not visiting the same state more than once. The best way to understand **backtracking** is through examples.

## N-queen problem
The queen in a chess-board has three possible moves:
* horizontal
* vertical
* diagnal

*Problem:* Put N queens in a N * N chessboard such that they are not a threat to each other.<br>
*Solution:* Let the chess be represented by a 2-dimensional boolean array, board. We will scan the board row-by-row placing a queen in a row/column such that is safe there. If we find no such position, we will backtrack. In the following code snippet: *valid_position(i, j)* determines if the i<sup>th</sup> row and j<sup>th></sup> column is a safe place for the queen.
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

				solve(i + 1, all);
				if (sol && !all)
					return;

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
