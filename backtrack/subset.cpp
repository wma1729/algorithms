#include <iostream>
#include <string>

using namespace std;

class subset
{
private:
	string str;
	bool   choices[2];

	/*
	 * Generate subsets of the given string.
	 *
	 * @param current_subset current subset so far.
	 * @param str            input string.
	 * @param idx            current string index.
	 * @param max            input string length.
	 */
	void generate(string &current_subset, const string &str, size_t idx, size_t len)
	{
		if (idx == len) {
			cout << "{" << current_subset << "}" << endl;
			return;
		}

		for (size_t i = 0; i < sizeof(choices) / sizeof(bool); ++i) {
			if (choices[i]) current_subset.push_back(str[idx]);
			generate(current_subset, str, idx + 1, len);
			if (choices[i]) current_subset.pop_back();
		}
	}

public:
	subset()
	{
		choices[0] = false;
		choices[1] = true;
	}

	~subset() {}

	/*
	 * Generate subsets of the given string.
	 */
	void generate(const string &str)
	{
		string current_subset;
		generate(current_subset, str, 0, str.length());
	}
};

static int
usage(const char *progname)
{
	cerr << progname << " <original-set>" << endl;
	return 1;
}

int
main(int argc, const char **argv)
{
	if (argc != 2)
		return usage(argv[0]);

	subset the_subset;
	the_subset.generate(argv[1]);

	return 0;
}
