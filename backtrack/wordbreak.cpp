#include <iostream>
#include <set>
#include <string>

using namespace std;

// all dictionary words
class dictionary
{
private:
	set<string> words;

public:
	dictionary() {}
	~dictionary() {}

	void add(const string &w)
	{
		words.insert(w);
	}

	bool contains(const string &w) const { return (words.find(w) != words.end()); }
};

static int
usage(const char *progname)
{
	cerr << progname << " <string-to-break>" << endl;
	return 1;
}

/*
 * Determines if the given string could be broken into
 * words from a given dictionary.
 *
 * @param to_break input string.
 * @param dict     reference to a dictionary.
 *
 * @return true if the string can be broken or false if the
 * word cannot be broken into words from a dictionary.
 */
bool
word_break(const string &to_break, const dictionary &dict)
{
	if (to_break.empty()) {
		// No more characters left. Return success
		return true;
	}

	for (size_t len = 1; len <= to_break.size(); ++len) {
		// Break the string in two: left (length = len) and right (rest if the string)
		string left = to_break.substr(0, len);
		string right = to_break.substr(len);

		if (dict.contains(left)) {
			// If found left, try to do the same for right.
			if (word_break(right, dict))
				return true;
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
