#include <iostream>
#include <set>
#include <string>

using namespace std;

class dictionary
{
private:
	set<string> words;

public:
	dictionary() {}
	~dictionary() {}

	void add(const string &w) { words.insert(w); }
	bool contains(const string &w) const { return (words.find(w) != words.end()); }
};

static int
usage(const char *progname)
{
	cerr << progname << " <string-to-break>" << endl;
	return 1;
}

bool
word_break(const string &to_break, const dictionary &dict)
{
	if (to_break.empty()) {
		return true;
	}

	for (size_t len = 1; len <= to_break.size(); ++len) {
		string word = to_break.substr(0, len);
		if (dict.contains(word)) {
			if (word_break(to_break.substr(len), dict)) {
				return true;
			}
		}
	}

	return false;
}

int
main(int argc, const char **argv)
{
	if (argc != 2)
		return usage(argv[0]);

	dictionary dict;

	dict.add("cats");
	dict.add("sand");
	dict.add("and");
	dict.add("dog");
	dict.add("cat");

	if (word_break(argv[1], dict))
		cout << "yes" << endl;
	else
		cout << "no" << endl;

	return 0;
}
