#include <iostream>
#include <iomanip>
#include <cstring>
#include "maths.h"

static int
usage(const char *progname)
{
	std::cerr << progname << std::endl
		<< "    [-sqrt <number>]             Finds floor square root of a number." << std::endl
		<< "    [-prime <number>]            Is the number prime?" << std::endl
		<< "    [-prime_ge <number>]         Prime number greater than/equal to a number." << std::endl
		<< "    [-factors <number>]          Factors of a number." << std::endl
		<< "    [-prime_factors <number>]    Prime factors of a number." << std::endl
		<< "    [-gcd <n1> <n2>]             GCD of two numbers." << std::endl
		<< "    [-lcm <n1> <n2>]             LCM of two numbers." << std::endl
		<< "    [-power <n1> <n2>]           n1 raised to the power of n2." << std::endl;
	return 1;
}

static void
print(const std::vector<long> &numbers)
{
	std::vector<long>::const_iterator it;
	for (it = numbers.begin(); it != numbers.end(); ++it) {
		if (it != numbers.begin())
			std::cout << ", ";
		std::cout << *it;
	}
	std::cout << std::endl;
}

int
main(int argc, const char **argv)
{
	long n1 = -1;
	long n2 = -1;

	if (argc < 2) {
		return usage(argv[0]);
	}

	for (int i = 1; i < argc; ++i) {
		if (strcmp(argv[i], "-sqrt") == 0) {
			++i;
			if (argv[i]) {
				n1 = atol(argv[i]);
				if (n1 < 0) {
					std::cerr << "invalid number " << argv[i] << " specified" << std::endl;
					return 1;
				} else {
					std::cout << floor_sqrt(n1) << std::endl;
				}
			} else {
				std::cerr << "missing argument for " << argv[i] << std::endl;
				return 1;
			}
		} else if (strcmp(argv[i], "-prime") == 0) {
			++i;
			if (argv[i]) {
				n1 = atol(argv[i]);
				if (n1 < 0) {
					std::cerr << "invalid number " << argv[i] << " specified" << std::endl;
					return 1;
				} else {
					std::cout << std::boolalpha << is_prime(n1) << std::endl;
				}
			} else {
				std::cerr << "missing argument for " << argv[i] << std::endl;
				return 1;
			}
		} else if (strcmp(argv[i], "-prime_ge") == 0) {
			++i;
			if (argv[i]) {
				n1 = atol(argv[i]);
				if (n1 < 0) {
					std::cerr << "invalid number " << argv[i] << " specified" << std::endl;
					return 1;
				} else {
					std::cout << prime_ge(n1) << std::endl;
				}
			} else {
				std::cerr << "missing argument for " << argv[i] << std::endl;
				return 1;
			}
		} else if (strcmp(argv[i], "-factors") == 0) {
			++i;
			if (argv[i]) {
				n1 = atol(argv[i]);
				if (n1 < 0) {
					std::cerr << "invalid number " << argv[i] << " specified" << std::endl;
					return 1;
				} else {
					std::vector<long> f = std::move(factors(n1));
					print(f);
				}
			} else {
				std::cerr << "missing argument for " << argv[i] << std::endl;
				return 1;
			}
		} else if (strcmp(argv[i], "-prime_factors") == 0) {
			++i;
			if (argv[i]) {
				n1 = atol(argv[i]);
				if (n1 < 0) {
					std::cerr << "invalid number " << argv[i] << " specified" << std::endl;
					return 1;
				} else {
					std::vector<long> f = std::move(prime_factors(n1));
					print(f);
				}
			} else {
				std::cerr << "missing argument for " << argv[i] << std::endl;
				return 1;
			}
		} else if (strcmp(argv[i], "-gcd") == 0) {
			++i;
			if (argv[i]) {
				n1 = atol(argv[i]);
				if (n1 < 0) {
					std::cerr << "invalid number " << argv[i] << " specified" << std::endl;
					return 1;
				}
			} else {
				std::cerr << "missing argument for " << argv[i] << std::endl;
				return 1;
			}
			++i;
			if (argv[i]) {
				n2 = atol(argv[i]);
				if (n2 < 0) {
					std::cerr << "invalid number " << argv[i] << " specified" << std::endl;
					return 1;
				}
			} else {
				std::cerr << "missing argument for " << argv[i] << std::endl;
				return 1;
			}

			std::cout << gcd(n1, n2) << std::endl;
		} else if (strcmp(argv[i], "-lcm") == 0) {
			++i;
			if (argv[i]) {
				n1 = atol(argv[i]);
				if (n1 < 0) {
					std::cerr << "invalid number " << argv[i] << " specified" << std::endl;
					return 1;
				}
			} else {
				std::cerr << "missing argument for " << argv[i] << std::endl;
				return 1;
			}
			++i;
			if (argv[i]) {
				n2 = atol(argv[i]);
				if (n2 < 0) {
					std::cerr << "invalid number " << argv[i] << " specified" << std::endl;
					return 1;
				}
			} else {
				std::cerr << "missing argument for " << argv[i] << std::endl;
				return 1;
			}

			std::cout << lcm(n1, n2) << std::endl;
		} else if (strcmp(argv[i], "-power") == 0) {
			++i;
			if (argv[i]) {
				n1 = atol(argv[i]);
				if (n1 < 0) {
					std::cerr << "invalid number " << argv[i] << " specified" << std::endl;
					return 1;
				}
			} else {
				std::cerr << "missing argument for " << argv[i] << std::endl;
				return 1;
			}
			++i;
			if (argv[i]) {
				n2 = atol(argv[i]);
				if (n2 < 0) {
					std::cerr << "invalid number " << argv[i] << " specified" << std::endl;
					return 1;
				}
			} else {
				std::cerr << "missing argument for " << argv[i] << std::endl;
				return 1;
			}

			std::cout << power(static_cast<int>(n1), static_cast<int>(n2)) << std::endl;
		} else {
			return usage(argv[0]);
		}
	}

	return 0;
}
