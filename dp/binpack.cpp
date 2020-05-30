#include <iostream>
#include <vector>

using namespace std;

constexpr int N = 5;
constexpr int C = 10;

static int w[] = { 0, 2, 4, 5, 3, 1 };
static bool memo[N + 1][C + 1];

bool
binpack_v1(int n, int c)
{
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

	return memo[n][c];
}

int
main()
{
	if (binpack_v2(N, C)) {
		cout << "fits" << endl;
	} else {
		cout << "does not fit" << endl;
	}
}
