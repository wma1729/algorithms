#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

/*
 * Find majority in the sequence.
 */
int
majority(const vector<int> &seq)
{
	int candidate = seq[0]; // possible candidate
	int frequency = 1;      // its frequency

	for (size_t i = 1; i < seq.size(); ++i) {
		if (seq[i] == candidate) {
			frequency++;
		} else {
			frequency--;
			if (frequency == 0) {
				candidate = seq[i];
				frequency = 1;
			}
		}
	}

	size_t multiplicity = 0;

	if (frequency) {
		/*
		 * There is a possibility that we have a majority.
		 * 'frequency' is not the real frequency. A +ve
		 * frequency simply means that there is a candidate
		 * that occurs more times than any other integer.
		 */
		for (size_t i = 0; i < seq.size(); ++i) {
			if (seq[i] == candidate)
				multiplicity++;
		}
	}

	return (multiplicity <= seq.size() / 2) ? -1 : candidate;
}

int
main(int argc, const char **argv)
{
	int i;
	vector<int> seq;

	fstream fin(argv[1], ios_base::in);
	while (fin >> i)
		seq.push_back(i);
	fin.close();

	int m = majority(seq);
	if (m == -1) {
		cout << "no majority" << endl;
	} else {
		cout << "majorty = " << m << endl;
	}

	return 0;
}
