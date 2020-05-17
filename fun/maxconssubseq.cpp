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

double
max_cons_subseq(const vector<double> &seq, size_t &start, size_t &end)
{
	double v = 0.0; // +ve value of a subsequence
	size_t s, e;    // start and end of a subsequence
	double V = 0.0; // maximum value of subsequence seen so far
	size_t S, E;    // start and end of a maximum subsequence seen so far

	// Initialize start and end of the current and maximal subsequences.
	s = S = e = E = seq.size();

	for (size_t i = 0; i < seq.size(); ++i) {
		v += seq[i];
		if (v > 0.0) {
			if (s == seq.size())
				s = e = i;
			else 
				e = i;
		} else {
			s = e = seq.size();
		}

		v = max(v, 0.0);
		if (v > V) {
			S = s;
			E = e;
			V = v;
		}
	}

	start = S;
	end = E;
	return V;
}

int
main(int argc, const char **argv)
{
	vector<double> seq;

	for (int i = 1; i < argc; ++i)
		seq.push_back(atof(argv[i]));

	size_t begin, end;
	begin = end = seq.size();

	cout << max_cons_subseq(seq, begin, end) << endl;
	while ((begin != seq.size()) && (begin <= end))
		cout << seq[begin++] << " ";
	cout << endl;

	return 0;
}
