#include <iostream>
#include <string>

using namespace std;

class subset
{
private:
	string str;
	bool   choices[2];

	void dump(const string &current_subset)
	{
		cout << "{" << current_subset << "}" << endl;
	}

public:
	subset(const string &s)
		: str(s)
	{
		choices[0] = false;
		choices[1] = true;
	}

	~subset() {}

	void generate(string &current_subset, size_t idx, size_t max)
	{
		if (idx == max) {
			dump(current_subset);
			return;
		}

		for (size_t i = 0; i < sizeof(choices) / sizeof(bool); ++i) {
			if (choices[i]) current_subset.push_back(str[idx]);
			generate(current_subset, idx + 1, max);
			if (choices[i]) current_subset.pop_back();
		}
	}

	void generate()
	{
		string current_subset;
		generate(current_subset, 0, str.length());
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

	subset the_subset(argv[1]);
	the_subset.generate();

	return 0;
}
