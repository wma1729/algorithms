# Graphs

A graph, **G = (V, E)**, is a data structure that consists of **V** vertices and **E** edges.
An edge connects two vertices and is represented by a pair of the two vertices it connects, (*v1*, *v2*).
A graph could be:
* **directed**, with a sense of direction, where an edge is an ordered pair (*v1*, *v2*),
* **undirected** where an edge is an unordered pair (*v1*, *v2*) and (*v2*, *v1*).

![A simple graph](graph.jpg)

## Graph representation
* Adjacency matrix *undirected*
```
    1  2  3  4  5  6  7  8  9 10 11
1   0  1  0  1  0  0  0  0  0  0  0
2   1  0  1  0  0  0  0  0  0  0  0  
3   0  1  0  0  0  0  0  0  0  0  0
4   1  0  0  0  1  0  1  0  0  0  0
5   0  0  0  1  0  1  1  0  0  0  0
6   0  0  0  0  1  0  0  1  0  0  0
7   0  0  0  1  1  0  0  1  0  0  0
8   0  0  0  0  0  1  1  0  0  0  0
9   0  0  0  0  0  0  0  0  0  1  1
10  0  0  0  0  0  0  0  0  1  0  0
11  0  0  0  0  0  0  0  0  1  0  0
```
* Adjacency list *undirected*
```
1  -> 2, 4
2  -> 1, 3
3  -> 2
4  -> 1, 5, 7
5  -> 4, 6, 7
6  -> 5, 8
7  -> 4, 5, 8 
8  -> 6, 7
9  -> 10, 11
10 -> 9
11 -> 9
```
Adjancency list is the most common representation used for graph. Here is an example:
```C++
/*
 * Graph implementation.
 */
template<typename T>
class graph
{
private:
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
	 * Get a vertex, v. The const version.
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

public:
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
	 * Add an edge, (from, to). If the graph is undirected, edge (to, from)
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
```

## Degree of a vertex
The number of edges coming out of a vertex is the degree of that edge.
```C++
	// Part of graph class

	/*
	 * Find the degree of an vertex, v.
	 */
	size_t degree(T v) const
	{
		return get_vertex(v).adjacent.size();
	}
```

## Graph Traversal
There are two main graph traversal methods based on the order in which the vertices are visited:
* **Depth First Search (DFS)** A very popular approach suitable to recursion. An arbitrary vertex is chosen. The action needed on the vertex is performed and the vertex is marked as *visited*. Then an adjacent, unvisited, vertex is picked up arbitrarily. The action needed is performed and that vertex is marked as *visited* as well. The process continues, as we go deeper into the graph, until all the connected vertices are processed and marked as *visited*. The data structure used to save the *unexplored* vertices is stack and that makes it apt for recursion.
* **Breadth First Search (BFS)** An organized approach were vertices are visited level by level. An arbitrary vertex is chosen. The action needed on the vertex is performed and the vertex is marked as *visited*. Next all adjacent, unvisited, vertices are processed and marked as *visited*. The process continues until all the vertices at all levels are processed and marked as *visited*. The data structure used to save the *unexplored* vertices is queue. BFS gives the **shortest path** between any two vertices of the graph.
```C++
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

	// Part of graph class

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

	enum class traversal_order
	{
		depth_first,
		breadth_first
	};

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
```
