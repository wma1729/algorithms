#include <iostream>
#include <vector>
#include <cstdlib>

using namespace std;

double
max_cons_subseq(const vector<double> &seq)
{
	double v = 0.0; // +ve value of a subsequence
	double V = 0.0; // maximum value of subsequence seen so far

	for (size_t i = 0; i < seq.size(); ++i) {
		v += seq[i];
		v = max(v, 0.0);
		V = max(v, V);
	}

	return V;
}

int
main(int argc, const char **argv)
{
	vector<double> seq;

	for (int i = 1; i < argc; ++i)
		seq.push_back(atof(argv[i]));

	cout << max_cons_subseq(seq) << endl;

	return 0;
}
