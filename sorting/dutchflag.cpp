#include <array>
#include <iostream>

using namespace std;

void
sort_dutch_flag(array<int, 9> &flag)
{
	size_t i = 0;
	size_t p1 = 0;
	size_t p2 = flag.size() - 1;

	while (i <= p2) {
		switch (flag[i]) {
			case 0:
				swap(flag[i], flag[p1]);
				i++;
				p1++;
				break;

			case 1:
				i++;
				break;

			case 2:
			default:
				swap(flag[i], flag[p2]);
				p2--;
				break;
		}
	}
}

int
main()
{
	array<int, 9> flag { 2, 1, 0, 1, 0, 2, 2, 1, 0 };

	sort_dutch_flag(flag);

	for (size_t i = 0; i < flag.size(); ++i) {
		if (i != 0)
			cout << ", ";
		cout << flag[i];
	}
	cout << endl;

	return 0;
}
