#include <iostream>
#include <cstring>
#include <cstdlib>

using namespace std;

/*
 * N-Queen Problem.
 */
class nqueen
{
private:
	int	num;            // number of rows/columns
	int     sol;            // number of solutions
	bool    **board;        // the board

	void init()
	{
		for (int i = 0; i < num; ++i) {
			for (int j = 0; j < num; ++j) {
				board[i][j] = false;
			}
		}
	}

	bool valid_horizontal(int i, int j)
	{
		for (int jj = j - 1; jj >= 0; --jj) {
			if (board[i][jj])
				return false;
		}
		return true;
	}

	bool valid_vertical(int i, int j)
	{
		for (int ii = i - 1; ii >= 0; --ii) {
			if (board[ii][j])
				return false;
		}
		return true;
	}

	bool valid_left_diagnal(int i, int j)
	{
		for (int ii = i - 1, jj = j - 1; (ii >= 0) && (jj >= 0); --ii, --jj) {
			if (board[ii][jj])
				return false;
		}
		return true;
	}

	bool valid_right_diagnal(int i, int j)
	{
		for (int ii = i - 1, jj = j + 1; (ii >= 0) && (jj < num); --ii, ++jj) {
			if (board[ii][jj]) {
				return false;
			}
		}
		return true;
	}

	bool valid_position(int i, int j)
	{
		if (valid_horizontal(i, j) && valid_vertical(i, j) &&
			valid_left_diagnal(i, j) && valid_right_diagnal(i, j)) {
			return true;
		}
		return false;
	}

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

public:
	nqueen(int n)
		: num(n)
		, sol(0)
	{
		board = new bool *[num];
		board[0] = new bool[num * num];
		for (int i = 1; i < num; ++i)
			board[i] = board[i - 1] + num;

		init();
	}

	~nqueen()
	{
		delete board[0];
		delete board;
	}

	void dump()
	{
		for (int i = 0; i < num; ++i) {
			for (int j = 0; j < num; ++j) {
				cout << (board[i][j] ? 'Q' : '-') << " ";
			}
			cout << endl;
		}
		cout << endl;
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

		cout << num << "-queen problem has " << sol << " solutions." << endl;

		return sol;
	}
};

static int
usage(const char *progname)
{
	cerr << progname << " [-n <num_of_rows_cols>] [-a]" << endl;
	return 1;
}

int
main(int argc, const char **argv)
{
	int n = 4;
	bool all = false;

	for (int i = 1; i < argc; ++i) {
		if (strcmp(argv[i], "-n") == 0) {
			++i;
			if (argv[i]) {
				n = atoi(argv[i]);
				if (n < 4) {
					cerr << "invalid row/col specified " << argv[i] << endl;
				}
			} else {
				cerr << "missing argument to -n" << endl;
				return usage(argv[0]);
			}
		} else if (strcmp(argv[i], "-a") == 0) {
			all = true;
		} else {
			return usage(argv[0]);
		}
	}

	nqueen problem(n);

	if (!problem.solve(all)) {
		return 1;
	}

	return 0;
}
