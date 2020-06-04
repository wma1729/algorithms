#include <iostream>
#include <iomanip>
#include <vector>

using namespace std;

constexpr int N = 7;
constexpr int C = 25;

static int w[] = { 0, 2, 3, 5, 6, 7, 9, 11 };
static bool memo[N + 1][C + 1];

bool
binpack_v1(int n, int c)
{
	cout << "n = " << n << ", c = " << c << endl;

	if (c == 0)
		return true;

	if (n == 0)
		return false;

	if (w[n] > c) {
		return binpack_v1(n - 1, c);
	} else {
		return binpack_v1(n - 1, c - w[n]);
	}
}

/*
 * Initialize memoization table.
 */
static void
init_memo()
{
	for (int i = 0; i <= N; ++i)
		for (int j = 0; j <= C; ++j)
			memo[i][j] = false;
}

char
table_entry(int i, int j)
{
	return (memo[i][j]) ? '1' : '0';
}

/*
 * Dump existization table.
 */
static void
dump_memo()
{
	cout << setw(4) << ' ';
	for (int j = 0; j <= C; ++j)
		cout << setw(4) << j;
	cout << endl;

	for (int i = 0; i <= N; ++i) {
		cout << setw(4) << w[i];
		for (int j = 0; j <= C; ++j)
			cout << setw(4) << table_entry(i, j);
		cout << endl;
	}
}

bool
binpack_v2(int n, int c)
{
	init_memo();

	for (int i = 0; i <= n; ++i) {
		for (int j = 0; j <= c; ++j) {
			if ((i == 0) && (j == 0))
				memo[i][j] = true;
			else if (w[i] > j)
				memo[i][j] = memo[i - 1][j];
			else
				memo[i][j] = memo[i - 1][j - w[i]];
		}
	}

	dump_memo();

	return memo[n][c];
}

int
main()
{
	if (binpack_v1(N, C)) {
		cout << "fits" << endl;
	} else {
		cout << "does not fit" << endl;
	}
}
