#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

/*
 * Defines a person and a sequence of people known.
 */
struct person
{
	string          name;   // your name
	vector<string>  known;  // people you know

	/*
	 * Name and comma-separated people known.
	 * If the people known is 'nil', then the person
	 * knows no one and is a candidate to be celebrity.
	 */
	explicit person(const string &n, const string &k)
		: name(n)
	{
		stringstream ss(k);

		while (ss.good()) {
			string val;
			getline(ss, val, ',');
			if (!val.empty() && (val != "nil"))
				known.push_back(val);
		}
	}

	/* Does this person knows 'someone'? */
	bool knows(const string &someone) const
	{
		for (auto s : known) {
			if (s == someone)
				return true;
		}
		return false;
	}

	void dump()
	{
		cout << name << " knows ";
		if (known.empty()) {
			cout << "no one";
		} else {
			for (size_t i = 0; i < known.size(); ++i) {
				if (i != 0)
					cout << ", ";
				cout << known[i];
			}
		}
		cout << endl;
	}
};

/*
 * Find celebrity.
 */
string
find_celebrity(const vector<person> &people)
{
	// Let the first person be the celebrity.
	size_t celeb = 0;

	/*
	 * If he knows the next person, then he is not
	 * a celebrity. Assign the next person as the
	 * celebrity.
	 */
	for (size_t i = 1; i < people.size(); ++i) {
		if (people[celeb].knows(people[i].name))
			celeb = i;
	}

	/*
	 * If there is a celebrity, we will know it by now.
	 * Handle the case when there is no celebrity.
	 */

	for (size_t i = 0; i < people.size(); ++i) {
		if (i != celeb) {
			/*
			 * If people[i] does not know the celebrity (OR)
			 * if the celebrity knows the people[i], then
			 * people[celeb] is not a celebrity.
			 */
			if (!people[i].knows(people[celeb].name) ||
				people[celeb].knows(people[i].name)) {
				return string();
			}
		}
	}

	return people[celeb].name;
}

int
main(int argc, const char **argv)
{
	string name, known;
	vector<person> people;

	fstream fin(argv[1], ios_base::in);
	while (fin >> name >> known)
		people.emplace_back(name, known);
	fin.close();

	for (auto p : people)
		p.dump();

	cout << "Celbrity: " << find_celebrity(people) << endl;

	return 0;
}
