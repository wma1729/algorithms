#include <iostream>
#include <vector>

using namespace std;

template<typename T>
class heap
{
private:
	vector<T> data;

	int left_child(int i) const { return 2 * i + 1; }
	int right_child(int i) const { return 2 * i + 2; }
	int parent(int i) const { return (i - 1) / 2; }

	void swim(size_t i)
	{
		while (i > 0) {
			size_t p = parent(i);
			if (data[i] < data[p])
				swap(data[i], data[p]);
			i = p;
		}
	}

	void sink(size_t i)
	{
		size_t c, l, r;

		while (i < data.size()) {
			l = left_child(i);
			r = right_child(i);

			if (l >= data.size())
				c = r;
			else if (r >= data.size())
				c = l;
			else if (data[l] < data[r])
				c = l;
			else
				c = r;

			if ((c < data.size()) && (data[c] < data[i])) {
				swap(data[c], data[i]);
				i = c;
			} else {
				break;
			}
		}
	}

public:
	heap() {}
	~heap() {}

	void put(const T &val)
	{
		data.push_back(val);
		swim(data.size() - 1);
	}

	T get()
	{
		T top = data[0];
		data[0] = data.back();
		data.pop_back();
		sink(0);
		return top;
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
	heap<int> h;

	h.put(15);
	h.put(27);
	h.put(19);
	h.put(33);
	h.put(47);
	h.put(23);

	h.dump();

	while (!h.empty())
		cout << h.get() << endl;
	
	return 0;
}
