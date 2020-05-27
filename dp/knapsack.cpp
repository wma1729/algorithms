#include <iostream>
#include <iomanip>

using namespace std;

constexpr int N = 5;    // Number of Items
constexpr int C = 10;   // Knapsack capacity

/*
 * Ideally w, v are passed to the function as parameters.
 * It is not done intentionally to make the algorithm
 * easier to read and understand.
 */

int w[] = { 0, 1, 4, 2, 5, 2 };
int v[] = { 0, 5, 5, 3, 2, 3 };

/*
 * The 0/1 knapsack problem.
 *
 * @param n - the number of items.
 * @param C - the capacity of the knapsack.
 *
 * Note: the 1st argument, n, has n possibilities: [1, n]
 *       The 2nd argument, C, has C possibilities: [1, C]
 *
 * @return the maximum value of the knapsack.
 */
int
knapsack_v1(int n, int C)
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
		V = knapsack_v1(n - 1, C);
	} else {
		/*
		 * If this item can be included, calculate the knapsack
		 * value by excluding this item and by including this
		 * item. Choose the maximum of the two.
		 */
		int v_excl = knapsack_v1(n - 1, C);
		int v_incl = v[n] + knapsack_v1(n - 1, C - w[n]);
		V = max(v_excl, v_incl);
	}

	return V;
}

/* Memoization table */
int memo[N + 1][C + 1];

/*
 * Initialize memoization table.
 */
static void
init_memo()
{
	for (int i = 0; i <= N; ++i) {
		for (int j = 0; j <= C; ++j) {
			memo[i][j] = -1;
		}
	}
}

/*
 * The 0/1 knapsack problem (using memoization)
 *
 * @param n - the number of items.
 * @param C - the capacity of the knapsack.
 *
 * Note: the 1st argument, n, has n possibilities: [1, n]
 *       The 2nd argument, C, has C possibilities: [1, C]
 * There are nC subproblems. The results of the subproblems
 * are stored in memo table and are reused as needed.
 *
 * @return the maximum value of the knapsack.
 */
int
knapsack_v2(int n, int C)
{
	int V; // Cumulative value of the knapsack so far.

	if (memo[n][C] == -1) {
		/*
		 * This problem has not been solved earlier.
		 */

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
			V = knapsack_v2(n - 1, C);
		} else {
			/*
			 * If this item can be included, calculate the knapsack
			 * value by excluding this item and by including this
			 * item. Choose the maximum of the two.
			 */
			int v_excl = knapsack_v2(n - 1, C);
			int v_incl = v[n] + knapsack_v2(n - 1, C - w[n]);
			V = max(v_excl, v_incl);
		}

		// Save the problem's result in the memo table
		memo[n][C] = V;
	}

	return memo[n][C];
}

/*
 * The 0/1 knapsack problem (using dynamic programming)
 * Essentially the same as knapsack_v2(). However, the
 * memoization table is built iteratively.
 *
 * @param n - the number of items.
 * @param C - the capacity of the knapsack.
 *
 * Note: the 1st argument, n, has n possibilities: [1, n]
 *       The 2nd argument, C, has C possibilities: [1, C]
 * There are nC subproblems. The results of the subproblems
 * are stored in memo table and are reused as needed.
 *
 * @return the maximum value of the knapsack.
 */
int
knapsack_v3(int n, int C)
{
	for (int i = 0; i <= n; ++i) {
		for (int j = 0; j <= C; ++j) {
			if ((i == 0) || (j == 0)) {
				/*
				 * Instead of the recursion termination condition
				 * in v1 and v2, this should be seen as memo table
				 * initialization.
				 */
				memo[i][j] = 0;
			} else if (w[i] > C) {
				/*
				 * If the current item is heavier than what knapsack
				 * can carry, leave this item and try the next one.
				 */
				memo[i][j] = memo[i - 1][j];
			} else {
				/*
				 * If this item can be included, calculate the knapsack
				 * value by excluding this item and by including this
				 * item. Choose the maximum of the two.
				 */
				int v_excl = memo[i - 1][j];
				int v_incl = v[i] + memo[i - 1][j - w[i]];
				memo[i][j] = max(v_excl, v_incl);
			}
		}
	}

	return memo[n][C];
}

int
main(int argc, const char **argv)
{
	init_memo();
	int result = knapsack_v3(5, 10);
	cout << "result = " << result << endl;
	return 0;
}
