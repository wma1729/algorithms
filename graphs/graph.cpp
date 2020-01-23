#include <iostream>
#include <ostream>
#include <sstream>
#include <vector>
#include <set>
#include <queue>
#include <algorithm>

using namespace std;

// Sample visitor function type & definition
template<typename T>
using visitor_t = void(*)(T);

template<typename T>
void visitor(T v)
{
	cout << "visited " << v << endl;
}

/*
 * Graph implementation.
 */
template<typename T>
class graph
{
	struct vertex
	{
		T           vrtx;       // vertex
		vector<T>   adjacent;   // adjacent vertices

		explicit vertex(T v) : vrtx(v) {}
		bool operator==(T v) const { return (this->vrtx == v); }
	};

	bool            directed;       // directed or undirected?
	size_t          count;          // number of vertices
	vector<vertex>  vertices;       // vertices in the graph

	/*
	 * Get a vertex, v.
	 * Throws out_of_range exception if the vertex is not found.
	 */
	const vertex & get_vertex(T v) const
	{
		typename vector<vertex>::const_iterator it = find(vertices.begin(), vertices.end(), v);
		if (it == vertices.end()) {
			ostringstream oss;
			oss << "vertex " << v << " not found";
			throw out_of_range(oss.str());
		} else {
			return *it;
		}
	}
	/*
	 * Get a vertex, v.
	 * Throws out_of_range exception if the vertex is not found.
	 */
	vertex & get_vertex(T v)
	{
		typename vector<vertex>::iterator it = find(vertices.begin(), vertices.end(), v);
		if (it == vertices.end()) {
			ostringstream oss;
			oss << "vertex " << v << " not found";
			throw out_of_range(oss.str());
		} else {
			return *it;
		}
	}

	/*
	 * Add an edge (from, to).
	 */
	void add_edge(vertex &from, const vertex &to)
	{
		if (from.adjacent.end() == find(from.adjacent.begin(), from.adjacent.end(), to.vrtx))
			from.adjacent.push_back(to.vrtx);
	}

	bool path_exists(set<T> &visited, const vertex &from, T to)
	{
		if (visited.end() == visited.find(from.vrtx)) {
			// not visited yet; mark as visited
			visited.insert(from.vrtx);
		} else {
			return false;
		}

		typename vector<T>::const_iterator it = find(from.adjacent.begin(), from.adjacent.end(), to);
		if (it != from.adjacent.end())
			return true;

		for (it = from.adjacent.begin(); it != from.adjacent.end(); ++it) {
			if (path_exists(visited, get_vertex(*it), to))
				return true;
		}

		return false;
	}

	void get_paths(vector<T> paths, const vertex &from, T to)
	{
		typename vector<T>::const_iterator it = find(from.adjacent.begin(), from.adjacent.end(), to);
		if (it != from.adjacent.end()) {
			cout << "path: ";
			for (auto v : paths)
				cout << v << " ";
			cout << to << endl;
			return;
		}

		for (it = from.adjacent.begin(); it != from.adjacent.end(); ++it) {
			paths.push_back(*it);
			get_paths(paths, get_vertex(*it), to);
			paths.pop_back();
		}
	}

	void depth_first_traversal(set<T> &visited, visitor_t<T> visitor, const vertex &from)
	{
		if (visited.end() == visited.find(from.vrtx)) {
			// not visited yet; mark as visited
			visited.insert(from.vrtx);
			(*visitor)(from.vrtx);
		} else {
			return;
		}

		typename vector<T>::const_iterator it;
		for (it = from.adjacent.begin(); it != from.adjacent.end(); ++it)
			depth_first_traversal(visited, visitor, get_vertex(*it));
	}

	void depth_first_traversal(set<T> &visited, visitor_t<T> visitor)
	{
		typename vector<vertex>::const_iterator it;
		for (it = vertices.begin(); it != vertices.end(); ++it)
			depth_first_traversal(visited, visitor, *it);
	}

	void breadth_first_traversal(set<T> &visited, visitor_t<T> visitor, T from)
	{
		queue<T> q;

		q.push(from);

		while (!q.empty()) {
			from = q.front();
			q.pop();

			if (visited.end() == visited.find(from)) {
				// not visited yet; mark as visited
				visited.insert(from);
				(*visitor)(from);

				vertex &v = get_vertex(from);

				typename vector<T>::const_iterator it;
				for (it = v.adjacent.begin(); it != v.adjacent.end(); ++it)
					q.push(*it);
			}
		}
	}

	void breadth_first_traversal(set<T> &visited, visitor_t<T> visitor)
	{
		typename vector<vertex>::const_iterator it;
		for (it = vertices.begin(); it != vertices.end(); ++it)
			breadth_first_traversal(visited, visitor, it->vrtx);
	}

public:
	enum class traversal_order
	{
		depth_first,
		breadth_first
	};

	graph(bool dir = true) : directed(dir), count(0) {}

	/*
	 * Add a vertex, v.
	 */
	void add_vertex(T v)
	{
		typename vector<vertex>::iterator it = find(vertices.begin(), vertices.end(), v);
		if (it == vertices.end()) {
			vertices.emplace_back(v);
			++count;
		}
	}

	/*
	 * Add an edge (from, to). If the graph is undirected, edge (to, from)
	 * is added as well.
	 */
	void add_edge(T from, T to)
	{
		add_vertex(from);
		add_vertex(to);

		vertex &v1 = get_vertex(from);
		vertex &v2 = get_vertex(to);

		add_edge(v1, v2);
		if (!directed)
			add_edge(v2, v1);
	}

	size_t num_vertices() const { return count; }

	/*
	 * Find the degree of an vertex, v.
	 */
	size_t degree(T v) const
	{
		return get_vertex(v).adjacent.size();
	}

	bool path_exists(T from, T to)
	{
		set<T> visited;
		return path_exists(visited, get_vertex(from), to);
	}

	void get_paths(T from, T to)
	{
		vector<T> paths;
		paths.push_back(from);
		get_paths(paths, get_vertex(from), to);
	}

	void traverse(traversal_order order, visitor_t<T> visitor)
	{
		set<T> visited;
		if (order == traversal_order::depth_first)
			depth_first_traversal(visited, visitor);
		else if (order == traversal_order::breadth_first)
			breadth_first_traversal(visited, visitor);
	}

	/*
	 * Prints the graph on the screen.
	 */
	void dump()
	{
		cout << "Number of vertices: " << count << endl;

		for (auto v : vertices) {
			cout << v.vrtx;

			if (!v.adjacent.empty())
				cout << " -> ";

			for (auto adj : v.adjacent) 
				cout << adj << " ";

			cout << endl;
		}
	}
};

// Driver code
int
main()
{
	graph<int> g(true);

	g.add_edge(1, 2);
	g.add_edge(1, 4);
	g.add_edge(2, 3);
	g.add_edge(4, 5);
	g.add_edge(4, 6);
	g.add_edge(5, 6);
	g.add_edge(5, 7);
	g.add_edge(6, 8);
	g.add_edge(7, 8);
	g.add_edge(9, 10);
	g.add_edge(9, 11);

	g.dump();

	cout << "degree of " << 4 << " is " << g.degree(4) << endl;

	if (g.path_exists(1, 8))
		cout << "path from 1 to 8 exists" << endl;
	else
		cout << "no path from 1 to 8" << endl;

	cout << "depth-first traversal" << endl;
	g.traverse(graph<int>::traversal_order::depth_first, visitor);

	cout << "breadth-first traversal" << endl;
	g.traverse(graph<int>::traversal_order::breadth_first, visitor);

	cout << "all paths from 4 to 8" << endl;
	g.get_paths(4, 8);

	return 0;
}
