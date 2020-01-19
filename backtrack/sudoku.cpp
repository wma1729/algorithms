#include <iostream>
#include <cmath>

using namespace std;

class sudoku
{
private:
	int rows;
	int cols;
	int **board;

	void init()
	{
		for (int r = 0; r < rows; ++r) {
			for (int c = 0; c < cols; ++c) {
				board[r][c] = 0;
			}
		}
	}

	bool is_valid_in_row(int r, int val)
	{
		for (int c = 0; c < cols; ++c) {
			if (board[r][c] == val)
				return false;
		}
		return true;
	}

	bool is_valid_in_col(int c, int val)
	{
		for (int r = 0; r < rows; ++r) {
			if (board[r][c] == val)
				return false;
		}
		return true;
	}

	bool is_valid_in_box(int r, int c, int val)
	{
		int boxsize = static_cast<int>(sqrt(rows));
		int br_start = (r / boxsize) * boxsize;
		int br_end = br_start + boxsize;
		int bc_start = (c / boxsize) * boxsize;
		int bc_end = bc_start + boxsize;

		for (int i = br_start; i < br_end; ++i) {
			for (int j = bc_start; j < bc_end; ++j) {
				if (board[i][j] == val)
					return false;
			}
		}
		return true;
	}

	bool is_valid(int r, int c, int val)
	{
		return
			is_valid_in_row(r, val) &&
			is_valid_in_col(c, val) &&
			is_valid_in_box(r, c, val);
	}

	bool solve(int r, int c)
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
			return solve(r, c + 1);

		// Iterate over all values.
		for (int v = 1; v <= rows; ++v) {
			if (is_valid(r, c, v)) {
				// Place the number.
				board[r][c] = v;

				// Solve for remaining numbers.
				if (solve(r, c + 1))
					return true;

				// Backtrack
				board[r][c] = 0;
			}
		}

		return false;
	}

public:
	sudoku(int n) : rows(n), cols(n)
	{
		board = new int *[rows];
		board[0] = new int[rows * cols];
		for (int i = 1; i < rows; ++i)
			board[i] = board[i - 1] + cols;

		init();
	}

	~sudoku()
	{
		delete [] board[0];
		delete [] board;
	}

	void set(int r, int c, int val)
	{
		if ((r >= 0) && (r < rows) && (c >= 0) && (c < cols)) {
			// valid index
			if ((val > 0) && (val <= rows)) {
				// valid value
				board[r][c] = val;
			}
		}
	}

	void dump()
	{
		for (int r = 0; r < rows; ++r) {
			for (int c = 0; c < cols; ++c) {
				cout << board[r][c] << " ";
			}
			cout << endl;
		}
		cout << endl;
	}

	bool solve()
	{
		return solve(0, 0);
	}
};

int
main(int argc, const char **argv)
{
	sudoku s(9);

	s.set(0, 1, 6);
	s.set(0, 3, 4);
	s.set(0, 5, 7);
	s.set(0, 7, 1);
	s.set(1, 0, 2);
	s.set(1, 1, 4);
	s.set(1, 3, 9);
	s.set(1, 5, 5);
	s.set(1, 6, 6);
	s.set(2, 0, 7);
	s.set(2, 3, 6);
	s.set(2, 4, 8);
	s.set(2, 6, 2);
	s.set(2, 8, 4);
	s.set(3, 1, 3);
	s.set(3, 2, 7);
	s.set(3, 5, 8);
	s.set(3, 8, 2);
	s.set(4, 2, 6);
	s.set(4, 3, 1);
	s.set(4, 4, 3);
	s.set(4, 6, 4);
	s.set(5, 0, 9);
	s.set(5, 1, 1);
	s.set(5, 6, 5);
	s.set(5, 7, 3);
	s.set(6, 7, 9);
	s.set(6, 8, 1);
	s.set(8, 1, 5);
	s.set(8, 4, 4);

	s.dump();

	if (s.solve()) {
		cout << "Found a solution!" << endl;
		return 0;
	} else {
		cout << "No solution found." << endl;
		return 1;
	}
}
