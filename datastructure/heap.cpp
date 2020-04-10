#include <iostream>
#include <vector>

using namespace std;

/*
 * A basic minimum heap implementation.
 */
template<typename T>
class heap
{
private:
	vector<T> data;

public:
	static int left_child(int i) { return 2 * i + 1; }
	static int right_child(int i) { return 2 * i + 2; }
	static int parent(int i) { return (i - 1) / 2; }

	/*
	 * Swim (percolate) up. The element at index i is
	 * moved up to its correct location in the heap.
	 */
	static void swim(vector<T> &elements, size_t i)
	{
		while (i > 0) {
			size_t p = parent(i);
			if (elements[i] < elements[p])
				swap(elements[i], elements[p]);
			i = p;
		}
	}

	/*
	 * Sink (percolate) down. The element at index i is
	 * moved down to its correct lacation in the heap.
	 */
	static void sink(vector<T> &elements, size_t i)
	{
		size_t c, l, r;

		while (i < elements.size()) {
			l = left_child(i);
			r = right_child(i);

			if (l >= elements.size())
				c = r;
			else if (r >= elements.size())
				c = l;
			else if (elements[l] < elements[r])
				c = l;
			else
				c = r;

			if ((c < elements.size()) && (elements[c] < elements[i])) {
				swap(elements[c], elements[i]);
				i = c;
			} else {
				break;
			}
		}
	}

	heap() {}
	~heap() {}

	/*
	 * Put an element to the heap.
	 */
	void put(const T &val)
	{
		data.push_back(val);
		swim(data, data.size() - 1);
	}

	/*
	 * Get an element from the the heap.
	 * The element from the top of the heap is returned.
	 */
	T get()
	{
		T top = data[0];
		data[0] = data.back();
		data.pop_back();
		sink(data, 0);
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

/*
 * Turn a random vector into a minimum heap.
 */
template<typename T>
void
make_heap(vector<T> &elements)
{
	int n = static_cast<int>(elements.size());
	for (int i = n / 2 - 1; i >= 0; --i)
		heap<T>::sink(elements, i);
}

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

	cout << "make heap" << endl;
	vector<int> v({ 0, 9, 2, 4, 7, 6, 3, 5, 1, 8 });
	make_heap(v);
	for (auto i : v)
		cout << i << endl;
	
	return 0;
}
