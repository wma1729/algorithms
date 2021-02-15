#include <iostream>
#include <string>

using namespace std;

inline char n2c(const std::string str)
{
	int n = stoi(str);
	return 'a' + (n - 1);
}

void decode(const string &s, int idx, string &tmp)
{
	if (idx >= s.size()) {
		cout << tmp << endl;
	} else {
		tmp.push_back(n2c(s.substr(idx, 1)));
		decode(s, idx + 1, tmp);
		tmp.pop_back();

		if (idx < s.size() - 1) {
			tmp.push_back(n2c(s.substr(idx, 2)));
			decode(s, idx + 2, tmp);
			tmp.pop_back();
		}
	}
}

void decode(const string &s)
{
	string tmp;
	decode(s, 0, tmp);
}

int
main()
{
	string s { "112" };
	decode(s);
	return 0;
}
