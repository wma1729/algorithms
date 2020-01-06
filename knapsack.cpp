#include <iostream>
#include <array>

using namespace std;

class knapsack
{
private:
	int capacity; // weight that the knapsack can carry

	int solve(int n, int c, int w[], int v[])
	{
		int maxvalue;

		if ((n == 0) || (c == 0)) {
			maxvalue = 0;
		} else if (w[n] > c) {
			maxvalue = solve(n - 1, c, w, v);
		} else {
			maxvalue = max(solve(n - 1, c, w, v), v[n] + solve(n - 1, c - w[n], w, v));
		}

		return maxvalue;
	}

public:
	knapsack(int c) : capacity(c) {}
	~knapsack() {}

	int solve(int nitems, int weight[], int value[])
	{
		return solve(nitems, capacity, weight, value);
	}
};

int
main(int argc, const char **argv)
{
	int weight[] = { 0, 1, 2, 4, 2, 5 };
	int value[]  = { 0, 5, 3, 5, 3, 2 };

	knapsack ks(10);
	int result = ks.solve(5, weight, value);
	cout << "result = " << result << endl;
	return 0;
}
