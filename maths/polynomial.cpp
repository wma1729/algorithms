#include <iostream>
#include <vector>

using namespace std;

static int nmultiplications = 0;
static int nadditions = 0;

static inline void
incr_multiply_cnt()
{
#if defined(DEBUG)
	nmultiplications++;
#endif
}

static inline void
incr_add_cnt()
{
#if defined(DEBUG)
	nadditions++;
#endif
}

static inline void
print_stats()
{
#if defined(DEBUG)
	cout << nmultiplications << " multiplications and " << nadditions << " additions done" << endl;
	nmultiplications = 0;
	nadditions = 0;
#endif
}

long
polynomial_v1(int x, const vector<int> &a)
{
	if (a.empty())
		return 0;

	if (a.size() == 1)
		return a[0];

	long result = a[0];

	for (size_t i = 1; i < a.size(); ++i) {
		long var = x;
		for (size_t j = i; j > 1; --j) {
			var *= x;
			incr_multiply_cnt();
		}

		result += (a[i] * var);
		incr_multiply_cnt();
		incr_add_cnt();
	}

	return result;
}

long
polynomial_v2(int x, const vector<int> &a)
{
	if (a.empty())
		return 0;

	if (a.size() == 1)
		return a[0];

	long result = a[0];
	long var = x;

	for (size_t i = 1; i < a.size(); ++i) {
		result += (a[i] * var);
		incr_multiply_cnt();
		incr_add_cnt();
		var = var * x;
		incr_multiply_cnt();
	}

	return result;
}

long
polynomial_v3(int x, const vector<int> &a)
{
	if (a.empty())
		return 0;

	if (a.size() == 1)
		return a[0];

	int i = static_cast<int>(a.size() - 1);

	long result = a[i] * x + a[i - 1];
	i--;

	incr_multiply_cnt();
	incr_add_cnt();

	for (; i > 0; --i) {
		result = (result * x) + a[i - 1];
		incr_multiply_cnt();
		incr_add_cnt();
	}

	return result;
}

int
main()
{
	vector<int> a;
	a.push_back(1);
	a.push_back(2);
	a.push_back(3);
	a.push_back(4);
	a.push_back(5);
	a.push_back(6);
	a.push_back(7);
	a.push_back(8);
	a.push_back(9);

	cout << polynomial_v1(3, a) << endl;
	print_stats();

	cout << polynomial_v2(3, a) << endl;
	print_stats();

	cout << polynomial_v3(3, a) << endl;
	print_stats();

	return 0;
}
