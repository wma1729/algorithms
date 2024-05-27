#include <iostream>
#include <iomanip>
#include <random>

static std::random_device rd;

static size_t
get_random_in_range(size_t start, size_t end)
{
	std::uniform_int_distribution<size_t> dist(start, end);
	return dist(rd);
}

static void
shuffle(int array[], size_t count)
{
	for (size_t i = 0; i < count; i++) {
		size_t r =  get_random_in_range(0, i);
		if (i != r) {
			std::swap(array[i], array[r]);
		}
	}
}

static void
dump_array(int array[], size_t count)
{
	for (size_t i = 0; i < count; ++i)
		std::cout << std::setw(4) << array[i];
	std::cout << std::endl;
}

int
main(int, const char **argv)
{
	size_t n = atoi(argv[1]);
	int *array = new int[n];

	for (size_t i = 0; i < n; ++i)
		array[i] = i;

	dump_array(array, n);

	shuffle(array, n);

	dump_array(array, n);

	return 0;
}
