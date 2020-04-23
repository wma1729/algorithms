#include <iostream>
#include <cstring>

using namespace std;

void
toh(int h, const string &s, const string &i, const string &t)
{
	if (h) {
		toh(h - 1, s, t, i);
		cout << "move disk " << h << " from " << s << " to " << t << endl;
		toh(h - 1, i, s, t);
	}
}

static int
usage(const char *progname)
{
	cerr << progname << " -h <height_of_tower>" << endl;
	return 1;
}

int
main(int argc, const char **argv)
{
	int h = 3;

	for (int i = 1; i < argc; ++i) {
		if (strcmp(argv[i], "-h") == 0) {
			++i;
			if (argv[i]) {
				h = atoi(argv[i]);
				if (h < 1) {
					cerr << "invalid height specified " << argv[i] << endl;
				}
			} else {
				cerr << "missing argument to -n" << endl;
				return usage(argv[0]);
			}
		} else {
			return usage(argv[0]);
		}
	}

	toh(h, "source", "intermediate", "target");

	return 0;
}
