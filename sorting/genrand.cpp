#include <iostream>
#include <random>

/*
 * Usage: genrand <count>
 */
int
main(int, const char **argv)
{
	int n = atoi(argv[1]);
	std::random_device rd;
	std::uniform_int_distribution<int> dist(0, n);

	for (int i = 0; i < n; ++i)
		std::cout << dist(rd) << std::endl;

	return 0;
}
