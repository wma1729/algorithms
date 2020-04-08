#include <iostream>
#include <vector>

using namespace std;

template<typename T>
class pq
{
private:
	vector<T> data;

	int left_child(int i) const { return 2 * i + 1; }
	int right_child(int i) const { return 2 * i + 2; }
	int parent(int i) const { return (i - 1) / 2; }

public:
	pq() {}
	~pq() {}

	void put(const T &val)
	{
		data.push_back(val);
		size_t i = data.size() - 1;

		while (i != 0) {
			size_t p = parent(i);
			if (data[i] < data[p])
				swap(data[i], data[p]);
			i = p;
		}
	}

	T get()
	{
		T val = data[0];
		data[0] = data.back();
		data.pop_back();

		size_t c, c1, c2;
		size_t i = 0;

		while (i < data.size()) {
			c1 = left_child(i);
			c2 = right_child(i);

			if (c1 >= data.size())
				c = c2;
			else if (c2 >= data.size())
				c = c1;
			else if (data[c1] < data[c2])
				c = c1;
			else
				c = c2;

			if ((c < data.size()) && (data[c] < data[i])) {
				swap(data[c], data[i]);
				i = c;
			} else {
				break;
			}
		}

		return val;
	}

	bool empty() const
	{
		return data.empty();
	}

	void dump() const
	{
		for (size_t i = 0; i < data.size(); ++i) {
			if (i != 0)
				cout << ", ";
			cout << data[i];
		}
		cout << endl;
	}
};

int
main()
{
	pq<int> q;

	q.put(5);
	q.put(4);
	q.put(2);
	q.put(6);
	q.put(1);
	q.put(3);
	q.put(0);

	q.dump();

	while (!q.empty())
		cout << q.get() << endl;
	
	return 0;
}
