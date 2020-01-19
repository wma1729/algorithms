#include <iostream>
#include <iomanip>

using namespace std;

class knapsack
{
private:
	int **memo;
	int **dp;

	int **create_array(int rows, int columns)
	{
		int r = rows + 1;
		int c = columns + 1;

		int **arr = new int *[r];
		arr[0] = new int[r * c];
		for (size_t i = 1; i < r; ++i)
			arr[i] = arr[i - 1] + c;

		for (size_t i = 0; i < r; ++i) {
			for (size_t j = 0; j < c; ++j) {
				arr[i][j] = -1;
			}
		}

		return arr;
	}

	void delete_array(int **arr)
	{
		delete [] arr[0];
		delete [] arr;
	}

	void display_array(int **arr, int nitems, int capacity, int weight[], int value[])
	{
		cout << "        ";
		for (size_t c = 0; c <= capacity; ++c)
			cout << setw(4) << c << " ";
		cout << endl;

		for (size_t r = 0; r <= nitems; ++r) {
			cout << r << "[" << weight[r] << "," << value[r] << "]  ";
			for (size_t c = 0; c <= capacity; ++c)
				cout << setw(4) << arr[r][c] << " ";
			cout << endl;
		}
	}

	int solve_v2_memo(int nitems, int capacity, int weight[], int value[])
	{
		int v;

		if (memo[nitems][capacity] != -1)
			return memo[nitems][capacity];

		if ((nitems == 0) || (capacity == 0)) {
			// If no more item left OR knapsack has no more capacity.
			v = 0;
		} else if (weight[nitems] > capacity) {
			// If the current item is heavier than what knapsack can carry.
			v = solve_v2_memo(nitems - 1, capacity, weight, value);
		} else {
			// Use maximum of value excluding this item and including this item
			v = max(
				solve_v2_memo(nitems - 1, capacity, weight, value),
				value[nitems] + solve_v2_memo(nitems - 1, capacity - weight[nitems], weight, value)
				);
		}

		return (memo[nitems][capacity] = v);
	}

public:
	knapsack() : memo(nullptr), dp(nullptr) {}
	~knapsack() {}

	int solve_v1(int nitems, int capacity, int weight[], int value[])
	{
		int v;

		if ((nitems == 0) || (capacity == 0)) {
			// If no more item left OR knapsack has no more capacity.
			v = 0;
		} else if (weight[nitems] > capacity) {
			// If the current item is heavier than what knapsack can carry.
			v = solve_v1(nitems - 1, capacity, weight, value);
		} else {
			// Use maximum of value excluding this item and including this item
			v = max(
				solve_v1(nitems - 1, capacity, weight, value),
				value[nitems] + solve_v1(nitems - 1, capacity - weight[nitems], weight, value)
				);
		}

		return v;
	}

	int solve_v2(int nitems, int capacity, int weight[], int value[])
	{
		memo = create_array(nitems, capacity);
		int maxvalue = solve_v2_memo(nitems, capacity, weight, value);
		display_array(memo, nitems, capacity, weight, value);
		delete_array(memo);
		return maxvalue;
	}

	int solve_v3(int nitems, int capacity, int weight[], int value[])
	{
		dp = create_array(nitems, capacity);

		for (int n = 0; n <= nitems; ++n) {
			for (int c = 0; c <= capacity; ++c) {
				if ((n == 0) || (c == 0)) {
					// If no more item left OR knapsack has no more capacity.
					dp[n][c] = 0;
				} else if (weight[n] > c) {
					// If the current item is heavier than what knapsack can carry.
					dp[n][c] = dp[n - 1][c];
				} else {
					// Use maximum of value excluding this item and including this item
					dp[n][c] = max(
							dp[n - 1][c],
							value[n] + dp[n - 1][c - weight[n]]
							);
				}
			}
		}

		int maxvalue =  dp[nitems][capacity];

		display_array(dp, nitems, capacity, weight, value);
		delete_array(dp);

		return maxvalue;
	}
};

int
main(int argc, const char **argv)
{
	int weight[] = { 0, 1, 2, 4, 2, 5 };
	int value[]  = { 0, 5, 3, 5, 3, 2 };

	knapsack ks;
	int result = ks.solve_v3(5, 10, weight, value);
	cout << "result = " << result << endl;
	return 0;
}
