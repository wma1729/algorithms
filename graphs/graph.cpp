#include <iostream>
#include <ostream>
#include <sstream>
#include <vector>
#include <set>
#include <queue>
#include <algorithm>

using namespace std;

/*
 * Sample visitor function type.
 */
template<typename T>
using visitor_t = void(*)(T);

/*
 * Sample visitor function definition.
 * The visitor simply prints the vertex.
 */
template<typename T>
void visitor(T v)
{
	cout << v << endl;
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

	/*
	 * Determine if a path exists between two vertices.
	 * We are essentially doing depth first search starting from source
	 * until we find target.
	 *
	 * @param visited maintains a set of vertices that are already visited.
	 * @param source  source vertex.
	 * @param target  target vertex.
	 * @return true if a path exists, false otherwise.
	 */
	bool path_exists(set<T> &visited, const vertex &source, T target)
	{
		if (visited.end() != visited.find(source.vrtx))
			return false;

		/*
		 * Not visited yet. Mark as visited.
		 */
		visited.insert(source.vrtx);

		/*
		 * Short-cut: See if we have reached the target vertex.
		 * This check could well be in the loop below as well.
		 * See the commented code in the loop below.
		 */
		typename vector<T>::const_iterator it = find(source.adjacent.begin(), source.adjacent.end(), target);
		if (it != source.adjacent.end())
			return true;

		/*
		 * Dive deeper looking for target.
		 */
		for (it = source.adjacent.begin(); it != source.adjacent.end(); ++it) {
			/*
			 * if (it->vrtx == target)
			 *     return true;
			 */
			if (path_exists(visited, get_vertex(*it), target))
				return true;
		}

		return false;
	}

	/*
	 * Get all paths between two vertices.
	 * @param paths  maintains paths already visited.
	 * @param visited maintains a set of vertices that are already visited.
	 * @param source source vertex.
	 * @param target target vertex.
	 */
	void get_paths(vector<T> &paths, set<T> &visited, const vertex &from, T to)
	{
		/*
		 * Mark the path as visited.
		 * Add the vertex to the set of path list.
		 */
		visited.insert(from.vrtx);
		paths.push_back(from.vrtx);

		typename vector<T>::const_iterator it = find(from.adjacent.begin(), from.adjacent.end(), to);
		if (it != from.adjacent.end()) {
			/*
			 * We have reached the target vertex, print the paths.
			 */
			cout << "path: ";
			for (auto v : paths)
				cout << v << " ";
			cout << to << endl;
		} else {
			/*
			 * Dive deeper.
			 */
			for (it = from.adjacent.begin(); it != from.adjacent.end(); ++it) {
				if (visited.end() == visited.find(*it)) {
					get_paths(paths, visited, get_vertex(*it), to);
				}
			}
		}

		/*
		 * Remove the vertex from path list.
		 * Unmark the path as visited.
		 */
		paths.pop_back();
		visited.erase(from.vrtx);
	}

	/*
	 * Depth first traversal starting from vertex *from*.
	 * @param visited maintains a set of vertices that are already visited.
	 * @param visitor performs processing at the each vertex.
	 * @param from    starting vertex.
	 */
	void depth_first_traversal(set<T> &visited, visitor_t<T> visitor, const vertex &from)
	{
		if (visited.end() == visited.find(from.vrtx)) {
			/*
			 * Not visited yet.
			 * Process and mark as visited.
			 */
			(*visitor)(from.vrtx);
			visited.insert(from.vrtx);
		} else {
			return;
		}

		/*
		 * Dive deeper.
		 */
		typename vector<T>::const_iterator it;
		for (it = from.adjacent.begin(); it != from.adjacent.end(); ++it)
			depth_first_traversal(visited, visitor, get_vertex(*it));
	}

	/*
	 * Depth first traversal for the whole graph.
	 * @param visited maintains a set of vertices that are already visited.
	 * @param visitor performs processing at the each vertex.
	 */
	void depth_first_traversal(set<T> &visited, visitor_t<T> visitor)
	{
		/*
		 * Do DFS for each vertex.
		 */
		typename vector<vertex>::const_iterator it;
		for (it = vertices.begin(); it != vertices.end(); ++it)
			depth_first_traversal(visited, visitor, *it);
	}

	/*
	 * Breadth first traversal starting from vertex *from*.
	 * @param visited maintains a set of vertices that are already visited.
	 * @param visitor performs processing at the each vertex.
	 * @param from    starting vertex.
	 */
	void breadth_first_traversal(set<T> &visited, visitor_t<T> visitor, T from)
	{
		queue<T> q;

		q.push(from);

		while (!q.empty()) {
			from = q.front();
			q.pop();

			if (visited.end() == visited.find(from)) {
				/*
				 * Not visited yet.
				 * Process and mark as visited.
				 */
				(*visitor)(from);
				visited.insert(from);

				vertex &v = get_vertex(from);

				/*
				 * Now add all the adjacent ones to the queue.
				 */
				typename vector<T>::const_iterator it;
				for (it = v.adjacent.begin(); it != v.adjacent.end(); ++it)
					q.push(*it);
			}
		}
	}

	/*
	 * Breadth first traversal for the whole graph.
	 * @param visited maintains a set of vertices that are already visited.
	 * @param visitor performs processing at the each vertex.
	 */
	void breadth_first_traversal(set<T> &visited, visitor_t<T> visitor)
	{
		/*
		 * Do BFS for each vertex.
		 */
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

	/*
	 * Determine if a path exists between two vertices.
	 * @param source source vertex.
	 * @param target target vertex.
	 * @return true if a path exists, false otherwise.
	 */
	bool path_exists(T source, T target)
	{
		set<T> visited;
		return path_exists(visited, get_vertex(source), target);
	}

	/*
	 * Get all paths between two vertices.
	 * @param source source vertex.
	 * @param target target vertex.
	 */
	void get_paths(T source, T target)
	{
		vector<T> paths;
		set<T> visited;
		get_paths(paths, visited, get_vertex(source), target);
	}

	/*
	 * Traverse the graph based on the traveral order requested
	 * and applying visitor at each vertex.
	 */
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
	graph<int> g(false);

	g.add_edge(1, 2);
	g.add_edge(1, 4);
	g.add_edge(2, 3);
	g.add_edge(4, 5);
	g.add_edge(4, 7);
	g.add_edge(5, 6);
	g.add_edge(5, 7);
	g.add_edge(6, 8);
	g.add_edge(7, 8);
	g.add_edge(9, 10);
	g.add_edge(9, 11);

	g.dump();

	cout << "degree of " << 4 << " is " << g.degree(4) << endl;

	cout << "depth-first traversal" << endl;
	g.traverse(graph<int>::traversal_order::depth_first, visitor);

	cout << "breadth-first traversal" << endl;
	g.traverse(graph<int>::traversal_order::breadth_first, visitor);

	if (g.path_exists(1, 8))
		cout << "path from 1 to 8 exists" << endl;
	else
		cout << "no path from 1 to 8" << endl;

	if (g.path_exists(6, 9))
		cout << "path from 6 to 9 exists" << endl;
	else
		cout << "no path from 6 to 9" << endl;

	cout << "all paths from 4 to 8" << endl;
	g.get_paths(4, 8);

	return 0;
}
