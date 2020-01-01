#include <iostream>

using namespace std;

template<typename T>
class linked_list
{
private:
	struct node
	{
		T	data;
		node	*next;
		node(T d) : data(d), next(nullptr) {}
	};

	node	*the_root;

	void rr(node *root)
	{
		if (root && root->next) {
			rr(root->next);
			root->next->next = root;
			root->next = nullptr;
		} else {
			the_root = root;
		}
	}

public:
	linked_list() : the_root(nullptr) {}

	~linked_list()
	{
		while (the_root) {
			node *tmp = the_root;
			the_root = the_root->next;
			delete tmp;
		}
	}

	void dump()
	{
		for (node *tmp = the_root; tmp != nullptr; tmp = tmp->next) {
			if (tmp != the_root)
				cout << ", ";
			cout << tmp->data;
		}
		cout << endl;
	}

	void add(T d)
	{
		node *tmp = the_root;
		while (tmp && tmp->next)
			tmp = tmp->next;

		if (tmp)
			tmp->next = new node(d);
		else
			the_root = new node(d);
	}

	void reverse()
	{
		node *prev = nullptr;
		node *curr = the_root;

		while (curr) {
			node *next = curr->next;
			curr->next = prev;
			prev = curr;
			curr = next;
		}

		the_root = prev;
	}

	void recursive_reverse()
	{
		rr(the_root);
	}

};

int
main()
{
	linked_list<int> ll;
	ll.add(1);
	ll.add(2);
	ll.add(3);
	ll.add(4);
	ll.dump();
	ll.reverse();
	ll.dump();
	ll.recursive_reverse();
	ll.dump();
	return 0;
}
