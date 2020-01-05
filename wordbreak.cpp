#include <iostream>
#include <set>
#include <string>

using namespace std;

// all dictionary words
class dictionary
{
private:
	set<string> words;
	size_t      maxwordsize;

public:
	dictionary() : maxwordsize(0) {}
	~dictionary() {}

	void add(const string &w)
	{
		if (w.length() > maxwordsize)
			maxwordsize = w.length();
		words.insert(w);
	}

	bool contains(const string &w) const { return (words.find(w) != words.end()); }

	size_t max_word_size() const { return maxwordsize; }
};

// words seen so far
class memo
{
private:
	set<string> words;

public:
	memo() {}
	~memo() {}

	void add(const string &w)
	{
#if DEBUG
		cout << '"' << w << "\" added to memo" << endl;
#endif
		words.insert(w);
	}

	bool contains(const string &w) const
	{
		if (words.find(w) != words.end()) {
#if DEBUG
			cout << '"' << w << "\" found in memo" << endl;
#endif
			return true;
		}
		return false;
	}
};

static int
usage(const char *progname)
{
	cerr << progname << " <string-to-break>" << endl;
	return 1;
}

bool
word_break(const string &to_break, const dictionary &dict, memo &m)
{
#if DEBUG
	cout << "to_break = " << to_break << endl;
#endif

	if (to_break.empty())
		return true;

	if (m.contains(to_break))
		return true;

	for (size_t len = 1; len <= dict.max_word_size(); ++len) {
		string left = to_break.substr(0, len);
		string right = to_break.substr(len);

		if (!m.contains(left)) {
			if (dict.contains(left))
				m.add(left);
			else
				continue;
		}

		if (word_break(right, dict, m))
			return true;
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

	memo m;
	if (word_break(argv[1], dict, m))
		cout << "yes" << endl;
	else
		cout << "no" << endl;

	return 0;
}
