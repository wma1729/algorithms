#include <iostream>
#include <string>

using namespace std;

/*
 * Find all permutations of the given string.
 *
 * @param remaining remaining string excluding characters that are re-arranged.
 * @param perm      permutated characters.
 *
 * Invariant: length(remaining) + length(perm) = length(original string).
 */
void permutate(const string &remaining, string &perm)
{
	if (remaining.empty()) {
		// No character left in the string. Print this permutation.
		cout << perm << endl;
		return;
	}

	for (size_t i = 0; i < remaining.length(); ++i) {
		// Consume the first character from remaining and permutate the rest.
		perm.push_back(remaining[i]);
		permutate(remaining.substr(0, i) + remaining.substr(i + 1), perm);
		perm.pop_back();
	}

	return;
}

/*
 * Find all permutations of the given string.
 *
 * @param str input string.
 */
void permutate(const string &str)
{
	string p;
	permutate(str, p);
}

static int
usage(const char *progname)
{
	cerr << progname << " <string-to-permutate>" << endl;
	return 1;
}

int
main(int argc, const char **argv)
{
	if (argc != 2)
		return usage(argv[0]);

	permutate(argv[1]);

	return 0;
}
