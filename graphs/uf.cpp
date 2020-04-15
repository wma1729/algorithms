#include <iostream>
#include "uf.h"

using namespace std;

int
main()
{
	union_find<int> uf;
	uf.union_op(1, 2);
	uf.union_op(2, 3);
	uf.union_op(3, 4);
	cout << "connected(2, 4) = " << boolalpha << uf.connected(2, 4) << endl;
	cout << "connected(2, 5) = " << boolalpha << uf.connected(2, 5) << endl;
	uf.dump();
	return 0;
}
