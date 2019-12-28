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

	void solve(int i, bool all)
	{
		if (i == num) {
			sol++;
			dump();
		} else {
			for (int j = 0; j < num; ++j) {
				if (valid_position(i, j)) {
					board[i][j] = true;

					solve(i + 1, all);
					if (sol && !all)
						return;

					board[i][j] = false;
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
		for (int i = 0; i < num; ++i)
			board[i] = new bool[num];
		init();
	}

	~nqueen()
	{
		for (int i = 0; i < num; ++i)
			delete board[i]; 
		delete board;
	}

	void dump()
	{
		for (int i = 0; i < num; ++i) {
			for (int j = 0; j < num; ++j) {
				cout << board[i][j] ? 1 : 0;
			}
			cout << endl;
		}
		cout << endl;
	}

	int solve(bool all)
	{
		solve(0, all);

		cout << num << "-queen problem has " << sol << " solutions" << endl;

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
