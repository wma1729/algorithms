#include <iostream>
#include <iomanip>

using namespace std;

/*
 * Determine if the string is a palindrome.
 * End case:
 * An empty string, begin < end, is considered palindrome.
 * A string of size 1, begin == end, is considered palindrome.
 */
bool
is_palindrome(const std::string &s, int begin, int end)
{
	while (begin <= end) {
		if (s[begin] != s[end])
			return false;

		++begin;
		--end;
	}

	return true;
}

/*
 * Determine if the string is a palindrome.
 */
bool
is_palindrome(const std::string &s)
{
	return is_palindrome(s, 0, static_cast<int>(s.size() - 1));
}

/*
 * Find the longest palindrome in a given sequence.
 *
 * @param [in]  s - the input string.
 * @param [out] p - the longest palindrome.
 *
 * @return the length of the longest palindrome.
 */
int
longest_palindrome_v1(const std::string &s, std::string &p)
{
	int start = 0;
	int length = 0;
	int slen = static_cast<int>(s.size());

	for (int i = 0; i < slen; ++i) {
		for (int j = slen - 1; j >= 0; --j) {
			if (i <= j) {
				if (is_palindrome(s, i, j)) {
					int l = j - i + 1;
					if (l > length) {
						length = j - i + 1;
						start = i;
					}
				}
			}
		}
	}

	p.assign(s, start, length);
	return length;
}

int
main()
{
	cout << "string: \"\" -> " << boolalpha << is_palindrome("") << endl;
	cout << "string: \"a\" -> " << boolalpha << is_palindrome("a") << endl;
	cout << "string: \"aba\" -> " << boolalpha << is_palindrome("aba") << endl;
	cout << "string: \"ababa\" ->" << boolalpha << is_palindrome("ababa") << endl;
	cout << "string: \"abada\" ->" << boolalpha << is_palindrome("abada") << endl;

	string p;

	cout << "longest palindrome (" << "ab" << "): len = "
		<< longest_palindrome_v1("ab", p) << ", p = "
		<< p << endl;
	p.clear();

	cout << "longest palindrome (" << "xyabcbazq" << "): len = "
		<< longest_palindrome_v1("xyabcbazq", p) << ", p = "
		<< p << endl;
	p.clear();

	return 0;
}
