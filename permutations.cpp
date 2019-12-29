#include <iostream>

using namespace std;

class permutations
{
private:
	string  str;

	void dump(const string &s)
	{
		cout << s << endl;
	}

	void generate(string &perm, const string &remaining)
	{
		if (remaining.empty()) {
			dump(perm);
			return;
		}

		for (size_t i = 0; i < remaining.length(); ++i) {
			perm.push_back(remaining[i]);
			generate(perm, remaining.substr(0, i) + remaining.substr(i + 1));
			perm.pop_back();
		}

		return;
	}

public:
	permutations(const string &s)
		: str(s)
	{
	}

	~permutations() {}

	void generate()
	{
		string p;
		generate(p, str);
	}
};

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

	permutations perm(argv[1]);
	perm.generate();

	return 0;
}
