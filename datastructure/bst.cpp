#include <iostream>
#include <queue>

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
			if (root->left == nullptr) {
				bst_node *tmp = root->right;
				delete root;
				root = tmp;
			} else {
				recursive_remove_min(root->left);
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
				if (child->left == nullptr) {
					if (parent) {
						if (d < parent->data)
							parent->left = child->right;
						else
							parent->right = child->right;
					} else {
						root = child->right;
					}
					delete child;
				} else if (child->right == nullptr) {
					if (parent) {
						if (d < parent->data)
							parent->left = child->left;
						else
							parent->right = child->left;
					} else {
						root = child->left;
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
	tree.remove(11, false);
	tree.traverse(bst_tree<int>::traversal_order::breadthfirst);

	cout << "removing 21" << endl;
	tree.remove(21, true);
	tree.traverse(bst_tree<int>::traversal_order::breadthfirst);
	return 0;
}
