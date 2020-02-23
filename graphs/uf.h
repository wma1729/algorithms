#include <memory>
#include <vector>
#include <algorithm>

using namespace std;

/*
 * Union-Find node.
 */
template<typename T>
struct ufnode
{
	T               value;      // value
	size_t          tsize;      // tree size
	struct ufnode   *parent;    // parent node

	ufnode(const T &v) : value(v), tsize(1), parent(nullptr) {}
	bool operator==(const T &v) { return (value == v); }
};

/*
 * Union-Find operations.
 */
template<typename T>
class union_find
{
private:
	// All the nodes.
	vector<unique_ptr<ufnode<T>>> nodes;

	/*
	 * Find the node. If the node does not exist, add it.
	 * @return pointer to the node.
	 */
	ufnode<T> *find_node(const T &n)
	{
		for (size_t i = 0; i < nodes.size(); ++i) {
			if (nodes[i]->value == n) {
				return nodes[i].get();
			}
		}

		ufnode<T> *nptr = new ufnode<T>(n);
		nodes.emplace_back(nptr);
		return nptr;
	}

	/*
	 * Find the parent of the node (the node where
	 * parent is nullptr).
	 * @return the parent node.
	 */
	ufnode<T> *find(ufnode<T> *nptr)
	{
		while (nptr && nptr->parent)
			nptr = nptr->parent;
		return nptr;
	}

public:
	union_find() {}
	~union_find() {}

	/*
	 * Perform union of two elements. After this,
	 * the parent of both node should be the same (or)
	 * connected(n1, n2) should return true.
	 *
	 * @param [in] n1 the first element.
	 * @param [in] n2 the second element.
	 */
	void union_op(const T &n1, const T &n2)
	{
		ufnode<T> *n1ptr = find_op(n1);
		ufnode<T> *n2ptr = find_op(n2);

		if (n1ptr->tsize > n2ptr->tsize) {
			n1ptr->tsize += n2ptr->tsize;
			n2ptr->tsize = 0;
			n2ptr->parent = n1ptr;
		} else {
			n2ptr->tsize += n1ptr->tsize;
			n1ptr->tsize = 0;
			n1ptr->parent = n2ptr;
		}
	}

	/*
	 * Find the parent node for the given element.
	 *
	 * @param [in] n the given element.
	 *
	 * @return the parent node.
	 */
	ufnode<T> *find_op(const T &n)
	{
		ufnode<T> *nptr = find_node(n);
		return find(nptr);
	}

	/*
	 * Are two elements connected (or) a union
	 * operation (directly/indirectly) has already been
	 * performed?
	 */
	bool connected(const T &n1, const T &n2)
	{
		ufnode<T> *n1ptr = find_op(n1);
		ufnode<T> *n2ptr = find_op(n2);
		return (n1ptr == n2ptr);
	}

	/*
	 * Dump the nodes.
	 */
	void dump()
	{
		for (size_t i = 0; i < nodes.size(); ++i) {
			cout << nodes[i]->value << " (" << nodes[i]->tsize << ") ";
			if (nodes[i]->parent)
				cout << " -> " << nodes[i]->parent->value;
			cout << endl;
		}
	}
};
