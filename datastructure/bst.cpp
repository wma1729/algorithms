#include <iostream>
#include <sstream>
#include <queue>
#include <vector>

using namespace std;

template<typename T>
class bst_tree
{
private:
	struct bst_node
	{
		T		data;
		bst_node	*left;
		bst_node	*right;
		bst_node(T d) : data(d), left(nullptr), right(nullptr) {}
	};

	bst_node *the_root;

	void traverse_preorder(bst_node *root)
	{
		if (root) {
			cout << root->data << endl;
			traverse_preorder(root->left);
			traverse_preorder(root->right);
		}
	}

	void traverse_inorder(bst_node *root)
	{
		if (root) {
			traverse_inorder(root->left);
			cout << root->data << endl;
			traverse_inorder(root->right);
		}
	}

	void traverse_postorder(bst_node *root)
	{
		if (root) {
			traverse_postorder(root->left);
			traverse_postorder(root->right);
			cout << root->data << endl;
		}
	}

	void traverse_breadthfirst(bst_node *root)
	{
		queue<bst_node *> q;

		if (root)
			q.push(root);

		while (!q.empty()) {
			bst_node *next = q.front();
			q.pop();

			cout << next->data << endl;

			if (next->left)
				q.push(next->left);
			if (next->right)
				q.push(next->right);
		}
	}

	void add(bst_node *& root, T d)
	{
		if (root == nullptr) {
			root = new bst_node(d);
		} else {
			bst_node *parent = nullptr;
			bst_node *child = root;

			while (child) {
				if (d < child->data) {
					parent = child;
					child = child->left;
				} else if (d > child->data) {
					parent = child;
					child = child->right;
				} else {
					// already there
					return;
				}
			}

			if (d < parent->data)
				parent->left = new bst_node(d);
			else
				parent->right = new bst_node(d);
		}
	}

	void recursive_add(bst_node *& root, T d)
	{
		if (root == nullptr) {
			root = new bst_node(d);
		} else {
			if (d < root->data) {
				recursive_add(root->left, d);
			} else if (d > root->data) {
				recursive_add(root->right, d);
			}
		}
	}

	bst_node *min(bst_node *root)
	{
		while (root && root->left)
			root = root->left;
		return root;
	}

	bst_node *recursive_min(bst_node *root)
	{
		while (root && root->left)
			return recursive_min(root->left);
		return root;
	}

	void remove_min(bst_node *& root)
	{
		bst_node *parent = nullptr;
		bst_node *child = root;

		while (child) {
			if (child->left) {
				parent = child;
				child = child->left;
			} else {
				if (parent)
					parent->left = child->right;
				else
					root = child->right;
				delete child;
				break;
			}
		}
	}

	void recursive_remove_min(bst_node *& root)
	{
		if (root) {
			if (root->left) {
				recursive_remove_min(root->left);
			} else {
				bst_node *tmp = root->right;
				delete root;
				root = tmp;
			}
		}
	}

	void remove(bst_node *& root, T d)
	{
		bst_node *parent = nullptr;
		bst_node *child = root;

		while (child) {
			if (d < child->data) {
				parent = child;
				child = child->left;
			} else if (d > child->data) {
				parent = child;
				child = child->right;
			} else {
				if (!child->left || !child->right) {
					bst_node *new_child = nullptr;

					if (child->left == nullptr)
						new_child = child->right;
					else
						new_child = child->left;

					if (!parent) {
						root = new_child;
					} else {
						if (child == parent->left)
							parent->left = new_child;
						else
							parent->right = new_child;
					}

					delete child;
				} else {
					bst_node *tmp = min(child->right);
					child->data = tmp->data;
					remove_min(child->right);
				}
				break;
			}
		}
	}

	void recursive_remove(bst_node *& root, T d)
	{
		if (root) {
			if (d < root->data) {
				recursive_remove(root->left, d);
			} else if (d > root->data) {
				recursive_remove(root->right, d);
			} else {
				bst_node *tmp = nullptr;

				if (root->left == nullptr) {
					tmp = root->right;
					delete root;
					root = tmp;
				} else if (root->right == nullptr) {
					tmp = root->left;
					delete root;
					root = tmp;
				} else {
					tmp = recursive_min(root->right);
					root->data = tmp->data;
					recursive_remove_min(root->right);
				}
			}
		}
	}

	void topview_left(bst_node *root)
	{
		if (root) {
			topview_left(root->left);
			cout << root->data << endl;
		}
	}

	void topview_right(bst_node *root)
	{
		if (root) {
			cout << root->data << endl;
			topview_right(root->right);
		}
	}

	void split(const string &data, vector<bst_node *> &nodes)
	{
		stringstream ss{data};
		while (ss.good()) {
			string s;
			getline(ss, s, ',');
			if (!s.empty()) {
				if (s == "*")
					nodes.push_back(nullptr);
				else
					nodes.push_back(new bst_node(stoi(s)));
			}
		}
	}

public:
	enum class traversal_order
	{
		preorder,
		inorder,
		postorder,
		breadthfirst
	};

	bst_tree() : the_root(nullptr) {}

	~bst_tree()
	{
		if (the_root) {
			if (the_root->left) delete the_root->left;
			if (the_root->right) delete the_root->right;
			delete the_root;
			the_root = nullptr;
		}
	}

	void add(T d, bool recursive = false)
	{
		if (recursive)
			recursive_add(the_root, d);
		else
			add(the_root, d);
	}

	void remove(T d, bool recursive = false)
	{
		if (recursive)
			recursive_remove(the_root, d);
		else
			remove(the_root, d);
	}

	void traverse(traversal_order order)
	{
		switch (order) {
			case traversal_order::preorder:
				traverse_preorder(the_root);
				break;

			case traversal_order::inorder:
				traverse_inorder(the_root);
				break;

			case traversal_order::postorder:
				traverse_postorder(the_root);
				break;

			case traversal_order::breadthfirst:
				traverse_breadthfirst(the_root);
				break;
		}
	}
	
	void topview()
	{
		topview_left(the_root->left);
		cout << the_root->data << endl;
		topview_right(the_root->right);
	}

	string serialize_bfs()
	{
		ostringstream oss;
		queue<bst_node *> q;
		q.push(the_root);

		while (!q.empty()) {
			bst_node *n = q.front();
			q.pop();

			if (n == nullptr) {
				oss << "*,";
			} else {
				oss << n->data << ","; 
				q.push(n->left);
				q.push(n->right);
			}
		}

		return oss.str();
	}

	void deserialize_bfs(const string &data)
	{
		vector<bst_node *> nodes;
		queue<bst_node *> q;
		split(data, nodes);

		if (nodes.empty() || (nodes[0] == nullptr)) {
			the_root = nullptr;
			return;
		}

		the_root = nodes[0];
		q.push(the_root);

		int i = 1;
		while (i < nodes.size()) {
			bst_node *parent = q.front();
			q.pop();

			if (nodes[i])
				q.push(parent->left = nodes[i]);
			i++;

			if ((i < nodes.size()) && nodes[i])
				q.push(parent->right = nodes[i]);
			i++;
		}
	}

	string serialize_preorder(bst_node *root)
	{
		ostringstream oss;

		if (root == nullptr) {
			oss << "*,";
		} else {
			oss << root->data << ',';
			oss << serialize_preorder(root->left);
			oss << serialize_preorder(root->right);
		}

		return oss.str();
	}

	string serialize_preorder()
	{
		return serialize_preorder(the_root);
	}

	// 8,6,5,*,*,7,*,*,10,9,*,*,11,*,*,
	bst_node *deserialize_preorder(const vector<bst_node *> &nodes, size_t &i)
	{
		bst_node *root = nodes[i++];
		if (root) {
			root->left = deserialize_preorder(nodes, i);
			root->right = deserialize_preorder(nodes, i);
		}
		return root;
	}

	void deserialize_preorder(const string &data)
	{
		vector<bst_node *> nodes;
		queue<bst_node *> q;
		split(data, nodes);

		if (nodes.empty() || (nodes[0] == nullptr)) {
			the_root = nullptr;
			return;
		}

		size_t i = 0;
		the_root = deserialize_preorder(nodes, i);
	}
};

int
main()
{
	bst_tree<int> tree;

	tree.add(11, true);
	tree.add(9, false);
	tree.add(13, true);
	tree.add(21, false);
	tree.add(15, false);
	tree.add(10, true);

	cout << "pre-order traversal" << endl;
	tree.traverse(bst_tree<int>::traversal_order::preorder);

	cout << "in-order traversal" << endl;
	tree.traverse(bst_tree<int>::traversal_order::inorder);

	cout << "post-order traversal" << endl;
	tree.traverse(bst_tree<int>::traversal_order::postorder);

	cout << "breadth-first traversal" << endl;
	tree.traverse(bst_tree<int>::traversal_order::breadthfirst);

	cout << "removing 11" << endl;
	tree.remove(11, true);
	tree.traverse(bst_tree<int>::traversal_order::breadthfirst);

	cout << "removing 21" << endl;
	tree.remove(21, true);
	tree.traverse(bst_tree<int>::traversal_order::breadthfirst);

	cout << "removing 15" << endl;
	tree.remove(15, true);
	tree.traverse(bst_tree<int>::traversal_order::breadthfirst);

	cout << "removing 10" << endl;
	tree.remove(10, true);
	tree.traverse(bst_tree<int>::traversal_order::breadthfirst);

	cout << "removing 9" << endl;
	tree.remove(9, true);
	tree.traverse(bst_tree<int>::traversal_order::breadthfirst);

	cout << "removing 13" << endl;
	tree.remove(13, true);
	tree.traverse(bst_tree<int>::traversal_order::breadthfirst);

	tree.add(8);
	tree.add(6);
	tree.add(5);
	tree.add(7);
	tree.add(10);
	tree.add(9);
	tree.add(11);
	tree.traverse(bst_tree<int>::traversal_order::breadthfirst);
	cout << "top view" << endl;
	tree.topview();

	cout << "breadth-first traversal before serialization" << endl;
	tree.traverse(bst_tree<int>::traversal_order::breadthfirst);

	string s = tree.serialize_bfs();

	bst_tree<int> tree2;
	tree2.deserialize_bfs(s);

	cout << "breadth-first traversal after de-serialization" << endl;
	tree2.traverse(bst_tree<int>::traversal_order::breadthfirst);

	s = tree.serialize_preorder();
	cout << s << endl;

	bst_tree<int> tree3;
	tree3.deserialize_preorder(s);

	cout << "breadth-first traversal after de-serialization using preorder" << endl;
	tree3.traverse(bst_tree<int>::traversal_order::breadthfirst);

	return 0;
}
