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
 * Find the longest palindrome substring in a given sequence.
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
			int l = j - i + 1;
			if (l > length) {
				if (is_palindrome(s, i, j)) {
					length = l;
					start = i;
				}
			}
		}
	}

	p.assign(s, start, length);
	return length;
}

/*
 * Find the longest palindrome substring in a given sequence
 * using dynamic programming.
 *
 * @param [in]  s - the input string.
 * @param [out] p - the longest palindrome.
 *
 * @return the length of the longest palindrome.
 */
int
longest_palindrome_v2(const std::string &s, std::string &p)
{
	int start = 0;
	int length = 1;
	int slen = static_cast<int>(s.size());
	int table[slen][slen];

	/*
	 * Handle palindrome of length 1.
	 * Simply set all diagonal element from
	 * top-left to bottom-right to true and
	 * every else to false.
	 */
	for (int i = 0; i < slen; ++i)
		for (int j = 0; j < slen; ++j)
			table[i][j] = (i == j) ? true : false;

	/*
	 * Handle palindrome of length 2.
	 * This could be merged with the generic length
	 * handling but this is more convenient.
	 */
	for (int i = 0; i < slen - 1; ++i)
		if (s[i] == s[i + 1]) {
			table[i][i + 1] = true;
			length = 2;
			start = i;
		}

	/*
	 * Handle palindrome of length 2 <= l < slen.
	 */
	for (int l = 3; l < slen; ++l) {
		for (int i = 0; i < slen - l + 1; ++i) {
			int j = i + l - 1;

			/*
			 * i is the staring index.
			 * j is the ending index.
			 * l == j - i + 1
			 * if s[i] == s[j] && * table[i + 1][j - 1] is a palindrome
			 * then
			 *     table[i][j] is a palindrome as well.
			 */

			if ((s[i] == s[j]) && table[i + 1][j - 1]) {
				table[i][j] = true;
				if (l > length) {
					length = l;
					start = i;
				}
			}
		}
	}

#if defined(DEBUG)
	cout << endl;
	cout << "    ";
	for (int i = 0; i < slen; ++i)
		cout << setw(4) << s[i];
	cout << endl;

	for (int i = 0; i < slen; ++i) {
		cout << setw(4) << s[i];
		for (int j = 0; j < slen; ++j) {
			cout << setw(4) << table[i][j] ? 1 : 0;
		}
		cout << endl;
	}
#endif

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
		<< longest_palindrome_v2("ab", p) << ", p = "
		<< p << endl;
	p.clear();

	cout << "longest palindrome (" << "xyabcbazq" << "): len = "
		<< longest_palindrome_v2("xyabcbazq", p) << ", p = "
		<< p << endl;
	p.clear();

	cout << "longest palindrome (" << "rabadxf" << "): len = "
		<< longest_palindrome_v2("rabadxf", p) << ", p = "
		<< p << endl;
	p.clear();
	return 0;
}
