# Graphs

A graph, **G = (V, E)**, is a data structure that consists of **V** vertices and **E** edges.
An edge connects two vertices and is represented by a pair of the two vertices it connects, (*v<sub>1</sub>*, *v<sub>2</sub>*).
A graph could be:
* **directed**, also called **digraph**, with a sense of direction, where an edge is an ordered pair (*v<sub>1</sub>*, *v<sub>2</sub>*),
* **undirected** where an edge is an unordered pair (*v<sub>1</sub>*, *v<sub>2</sub>*) and (*v<sub>2</sub>*, *v<sub>1</sub>*).
A graph could be:
* **unweighted** where there is no cost/length/time is associated with an edge.
* **weighted** where is a a cost/length/time associated with an edge.

![A simple graph](graph.jpeg)

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
Adjancency list is the most common representation used for graph. Here is a possible C++ implementation:
```C++
/*
 * Represents an edge and optional weight.
 */
template<typename T>
class edge
{
private:
	T       _from;   // start/source vertex
	T       _to;     // end/sink vertex
	double  _wt;     // optional weight, 0.0 for unweighted graph.

public:
	explicit edge(const T & v1, const T & v2)
		: _from(v1), _to(v2), _wt(0.0) {}
	explicit edge(const T & v1, const T & v2, double w)
		: _from(v1), _to(v2), _wt(w) {}

	T source() const { return _from; }
	void source(const T & v) { _from = v; }

	T sink() const { return _to; }
	void sink(const T & v) { _to = v; }

	double weight() const { return _wt; }
	void weight(double w) { _wt = w; }
	bool is_weighted() const { return (_wt != 0.0); }

	bool operator==(const T & v) { return (sink() == v); }
	bool operator==(const edge<T> &e)
	{
		return ((source() == e.source()) &&
			(sink() == e.sink()) &&
			(weight() == e.weight()));
	}
};

template<typename T>
ostream &
operator << (ostream &os, const edge<T> &e)
{
	os << "[(" << e.source() << ", " << e.sink() << ")";
	if (e.is_weighted())
		os << ", "  << e.weight();
	os << "]";
	return os;
}

/*
 * Vertex of a graph.
 */
template<typename T>
class vertex
{
private:
	T                   _vrtx;  // the vertex itself
	vector<edge<T> *>   _edges; // edges originating from the vertex

public:
	explicit vertex(const T & v) : _vrtx(v) {}

	// type operator
	operator T() const { return _vrtx; }

	// get adjacent vertices
	vector<T> adjacent() const
	{
		vector<T> v;
		for (auto e : _edges)
			v.push_back(e->sink());
		return v;
	}

	// add edge
	void add_edge(edge<T> *e)
	{
		if (e && (e->source() == _vrtx))
			_edges.push_back(e);
	}

	// get all edges originating from the vertex
	const vector<edge<T> *> & edges() const { return _edges; }
	vector<edge<T> *> & edges() { return _edges; }

	// get degree of the vertex.
	size_t degree() const { return _edges.size(); }

	bool operator==(const T & v) const { return (_vrtx == v); }
};

template<typename T>
ostream &
operator << (ostream &os, const vertex<T> &v)
{
	T vrtx = v;
	os << vrtx << " : ";

	typename vector<edge<T> *>::const_iterator it;
	for (it = v.edges().begin(); it != v.edges().end(); ++it) {
		if (it != v.edges().begin())
			os << ", ";
		const edge<T> *e = *it;
		os << *e;
	}

	return os;
}

/*
 * Graph implementation.
 */
template<typename T>
class graph
{
private:
	bool                _directed;      // directed or undirected?
	size_t              _count;         // number of vertices
	vector<vertex<T>>   _vertices;      // vertices in the graph
	vector<edge<T> *>   _edges;         // all edges in the graph

	/*
	 * Get a vertex, v.
	 * Throws out_of_range exception if the vertex is not found.
	 */
	vertex<T> & get_vertex(const T &v)
	{
		typename vector<vertex<T>>::iterator it = find(_vertices.begin(), _vertices.end(), v);
		if (it == _vertices.end()) {
			ostringstream oss;
			oss << "vertex " << v << " not found";
			throw out_of_range(oss.str());
		} else {
			return *it;
		}
	}

	/*
	 * Add an edge (from, to, optional-weight).
	 */
	void add_edge(vertex<T> &from, const vertex<T> &to, double weight = 0.0)
	{
		// check if the edge already exists
		typename vector<edge<T> *>::iterator it;
		for (it = from.edges().begin(); it != from.edges().end(); ++it) {
			edge<T> *e = *it;
			if (e->sink() == to) {
				// edge already exists! update weight.
				e->weight(weight);
				return;
			}
		}

		edge<T> *e = new edge<T>(from, to, weight);
		_edges.push_back(e);
		from.add_edge(e);
	}

	/*
	 * Serialize the graph using BFS.
	 */
	void serialize(ostream &os, set<T> &visited, const T &from)
	{
		queue<T> q;

		q.push(from);

		while (!q.empty()) {
			T current = q.front();
			q.pop();

			if (visited.end() == visited.find(from)) {
				/*
				 * Not visited yet.
				 * Process and mark as visited.
				 */
				visited.insert(current);

				const vertex<T> &v = get_vertex(current);

				/*
				 * Now add all the adjacent ones to the queue.
				 */
				typename vector<edge<T> *>::const_iterator it;
				for (it = v.edges().begin(); it != v.edges().end(); ++it) {
					const edge<T> *e =  *it;
					os << e->source() << " " << e->sink() << " " << e->weight() << endl;
					q.push(e->sink());
				}
			}
		}
	}

public:
	graph(bool dir = true) : _directed(dir), _count(0) {}

	graph(istream &is)
	{
		int dir;
		T v1, v2;
		double w = 0.0;

		_count = 0;

		is >> dir;
		_directed = (dir == 1);

		while (is) {
			is >> v1 >> v2 >> w;
			add_edge(v1, v2, w);
		}

	}

	~graph()
	{
		typename vector<edge<T> *>::iterator it = _edges.begin();
		while (it != _edges.end()) {
			edge<T> *e = *it;
			delete e;
			++it;
		}
	}

	/*
	 * Add a vertex, v.
	 */
	void add_vertex(const T &v)
	{
		typename vector<vertex<T>>::iterator it = find(_vertices.begin(), _vertices.end(), v);
		if (it == _vertices.end()) {
			_vertices.emplace_back(v);
			++_count;
		}
	}

	/*
	 * Get a vertex, v.
	 * Throws out_of_range exception if the vertex is not found.
	 */
	const vertex<T> & get_vertex(const T &v) const
	{
		typename vector<vertex<T>>::const_iterator it = find(_vertices.begin(), _vertices.end(), v);
		if (it == _vertices.end()) {
			ostringstream oss;
			oss << "vertex " << v << " not found";
			throw out_of_range(oss.str());
		} else {
			return *it;
		}
	}

	/*
	 * Get the first vertex.
	 */
	const vertex<T> & first() const
	{
		return _vertices[0];
	}

	/*
	 * Add an edge (from, to, optional-weight). If the graph is undirected,
	 * edge (to, from, optional-weight) is added as well.
	 */
	void add_edge(const T &from, const T &to, double weight = 0.0)
	{
		add_vertex(from);
		add_vertex(to);

		vertex<T> &v1 = get_vertex(from);
		vertex<T> &v2 = get_vertex(to);

		add_edge(v1, v2, weight);
		if (!_directed)
			add_edge(v2, v1, weight);
	}

	bool directed() const { return _directed; }
	size_t count() const { return _count; }
	const vector<vertex<T>> &vertices() const { return _vertices; }
	const vector<edge<T> *> &edges() const { return _edges; }

	/*
	 * Find the degree of an vertex, v.
	 */
	size_t degree(const T &v) const
	{
		return get_vertex(v).degree();
	}

	/*
	 * Reverse a graph.
	 */
	graph<T> reverse() const
	{
		graph<T> g(true);

		if (_directed) {
			for (auto v : _vertices) {
				typename vector<edge<T> *>::const_iterator it;
				for (it = v.edges().begin(); it != v.edges().end(); ++it) {
					const edge<T> *e = *it;
					g.add_edge(e->sink(), e->source(), e->weight());
				}
			}
		} else {
			g = *this;
		}

		return g;
	}
	/*
	 * Serialize a graph.
	 * Line 1: 0|1 (undirected or directed)
	 * Line 2: v1 v2 w1 (two vertices of an edge and its weight)
	 * Line 3: v3 v4 w2 (two vertices of an edge and its weight)
	 * ...
	 */
	void serialize(ostream &os)
	{
		set<T> visited;

		cout << (_directed ? "1" : "0") << endl;
		
		typename vector<vertex<T>>::const_iterator it;
		for (it = _vertices.begin(); it != _vertices.end(); ++it)
			serialize(os, visited, *it);
	}
};

template<typename T>
ostream &
operator << (ostream &os, const graph<T> &g)
{
	os << (g.directed() ? "Directed" : "Un-directed") << " graph with "
		<< g.count() << " vertices" << endl;

	for (auto v : g.vertices())
		cout << v << endl;

	return os;
}
```

## Degree of a vertex
The number of edges coming out of a vertex is the degree of that edge.

## Graph Traversal
There are two main graph traversal methods based on the order in which the vertices are visited:
* **Depth First Search (DFS)** A very popular approach suitable to recursion. An arbitrary vertex is chosen. The action needed on the vertex is performed and the vertex is marked as *visited*. Then an adjacent, unvisited, vertex is picked up arbitrarily. The action needed is performed and that vertex is marked as *visited* as well. The process continues, as we go deeper into the graph, until all the connected vertices are processed and marked as *visited*. The data structure used to save the *unexplored* vertices is stack and that makes it apt for recursion.
* **Breadth First Search (BFS)** An organized approach were vertices are visited level by level. An arbitrary vertex is chosen. The action needed on the vertex is performed and the vertex is marked as *visited*. Next all adjacent, unvisited, vertices are processed and marked as *visited*. The process continues until all the vertices at all levels are processed and marked as *visited*. The data structure used to save the *unexplored* vertices is queue. BFS gives the **shortest path** between any two vertices of the graph.

We need a data structure to do processing at the vertex and mark the vertex as visited. Here is one possibility:
```C++
/*
 * The default vertex visitor. It provides two functionalities:
 * 1) Perform needed pre/post processing while traversing the graph.
 * 2) Maintains a set of vertices already visited.
 *
 * It is implemented as a C++ class.
 * Default pre processing: prints the vertex.
 * Default post processing: no-op.
 * This can be overridden in the sub-class to do something
 * more meaningful.
 */
template<typename T>
class visitor
{
private:
	set<T> visited;

public:
	visitor() {}
	virtual ~visitor() {}

	/* Override as needed */
	virtual void pre(const vertex<T> &v)
	{
		cout << v.vrtx << endl;
	}

	/* Override as needed */
	virtual void post(const vertex<T> &)
	{
		return;
	}

	bool is_visited(const T &v)
	{
		return (visited.end() != visited.find(v));
	}

	bool is_visited(const vertex<T> &v)
	{
		return is_visited(v.vrtx);
	}

	void set_visited(const T &v, bool val)
	{
		if (val)
			visited.insert(v);
		else
			visited.erase(v);
	}

	void set_visited(const vertex<T> &v, bool val)
	{
		set_visited(v.vrtx, val);
	}
};
```
With the graph and visitor in place, a number of algorithms could be fairly easily developed.
```C++
/*
 * Depth first traversal starting from vertex, from.
 * @param [in]  g       the graph.
 * @param [in]  visitor the visitor class.
 * @param [in]  from    the starting vertex.
 */
template<typename T>
static void
dfs(const graph<T> &g, visitor<T> &visitor, const vertex<T> &from)
{
	if (visitor.is_visited(from)) 
		return;

	// Process and mark as visited
	visitor.pre(from);
	visitor.set_visited(from, true);

	typename vector<T>::const_iterator it;
	for (it = from.adjacent.begin(); it != from.adjacent.end(); ++it)
		dfs(g, visitor, g.get_vertex(*it));

	// Perform post processing
	visitor.post(from);
}

/*
 * Depth first traversal for the whole graph.
 * @param [in]  g       the graph.
 * @param [in]  visitor the visitor class.
 */
template<typename T>
void
dfs(const graph<T> &g, visitor<T> &visitor)
{
	typename vector<vertex<T>>::const_iterator it;
	for (it = g.get_vertices().begin(); it != g.get_vertices().end(); ++it)
		dfs(g, visitor, *it);
}

/*
 * Breadth first traversal starting from vertex, from
 * @param [in]  g       the graph.
 * @param [in]  visitor the visitor class.
 * @param [in]  from    the starting vertex.
 */
template<typename T>
static void
bfs(const graph<T> &g, visitor<T> &visitor, const vertex<T> &from)
{
	queue<T> q;

	// Add the first vertex to the queue and mark as visited.
	q.push(from.vrtx);
	visitor.set_visited(from, true);

	while (!q.empty()) {
		T v = q.front();
		q.pop();

		const vertex<T> &current = g.get_vertex(v);
		visitor.pre(current);

		// Now add all the adjacent ones to the queue.
		typename vector<T>::const_iterator it;
		for (it = current.adjacent.begin(); it != current.adjacent.end(); ++it) {
			if (!visitor.is_visited(*it)) {
				q.push(*it);
				visitor.set_visited(*it, true);
			}
		}
	}
}

/*
 * Breadth first traversal for the whole graph.
 * @param [in]  g       the graph.
 * @param [in]  visitor the visitor class.
 */
template<typename T>
void
bfs(const graph<T> &g, visitor<T> &visitor)
{
	typename vector<vertex<T>>::const_iterator it;
	for (it = g.get_vertices().begin(); it != g.get_vertices().end(); ++it) {
		if (!visitor.is_visited(*it))
			bfs(g, visitor, *it);
	}
}
```
## Traversal Problem 1
*Problem:* Given a graph with vertices v<sub>1</sub>, v<sub>2</sub>, v<sub>3</sub>, ..., v<sub>n - 1</sub>, v<sub>n</sub>, does a path exists between vertex v<sub>source</sub> and v<sub>target</sub>? Or simply is vertex v<sub>target</sub> reachable from v<sub>source</sub>?<br>
*Solution:* Perform DFS traversal starting from vertex v<sub>source</sub>. If we encounter v<sub>target</sub>, then a path exists. If the DFS traversal completes without finding v<sub>target</sub>, then no path exists between them.
```C++
/*
 * Determine if vertex, target, is reachable from vertex, source.
 * We are essentially doing depth first search starting from source
 * until we find target.
 *
 * @param [in] g       the graph.
 * @param [in] visitor the visitor class.
 * @param [in] source  the source vertex.
 * @param [in] target  the target vertex.
 *
 * @return true if a path exists, false otherwise.
 */
template<typename T>
static bool
is_reachable(const graph<T> &g, visitor<T> &visitor, const vertex<T> &source, const T &target)
{
	visitor.set_visited(source, true);

	typename vector<T>::const_iterator it;
	for (it = source.adjacent.begin(); it != source.adjacent.end(); ++it) {
		if (!visitor.is_visited(*it)) {
			if (*it == target)
				return true;
			if (is_reachable(g, visitor, g.get_vertex(*it), target))
				return true;
		}
	}

	return false;
}

/*
 * Determine if vertex, target, is reachable from vertex, source.
 *
 * @param [in] g       the graph.
 * @param [in] source  the source vertex.
 * @param [in] target  the target vertex.
 *
 * @return true if a path exists, false otherwise.
 */
template<typename T>
bool
is_reachable(const graph<T> &g, const T &source, const T &target)
{
	if (source != target) {
		visitor<T> visitor;
		return is_reachable(g, visitor, g.get_vertex(source), target);
	}
	return true;
}
```
## Traversal Problem 2
*Problem:* Given a graph with vertices v<sub>1</sub>, v<sub>2</sub>, v<sub>3</sub>, ..., v<sub>n - 1</sub>, v<sub>n</sub>, find all paths between vertex v<sub>source</sub> and v<sub>target</sub>?<br>
*Solution:* Perform DFS traversal starting from vertex v<sub>source</sub>. Keep recording the paths until we reach the vertex v<sub>target</sub>. We must also mark the vertex as visited as we proceed. This is to avoid cycles, especially for undirected graphs. This is yet another example of **backtracking** problem.
```C++
/*
 * Get all paths between two vertices: source and target
 *
 * @param [in]    g       the graph.
 * @param [in]    visitor the visitor class.
 * @param [in]    source  the source vertex.
 * @param [in]    target  the target vertex.
 * @param [inout] paths   a container vector to hold paths as they are discovered.
 */
template<typename T>
static void
get_paths(const graph<T> &g, visitor<T> &visitor, const vertex<T> &source, const T &target, vector<T> &paths)
{
	/*
	 * Mark the path as visited.
	 * Add the vertex to the set of path list.
	 */
	visitor.set_visited(source, true);
	paths.push_back(source.vrtx);

	typename vector<T>::const_iterator it;
	for (it = source.adjacent.begin(); it != source.adjacent.end(); ++it) {
		if (!visitor.is_visited(*it)) {
			if (*it == target) {
				/*
				 * We have reached the target vertex, print the paths.
				 */
				cout << "path: ";
				for (auto v : paths)
					cout << v << " ";
				cout << target << endl;
			} else {
				get_paths(g, visitor, g.get_vertex(*it), target, paths);
			}
		}
	}

	/*
	 * Backtrack.
	 * Remove the vertex from path list.
	 * Unmark the path as visited.
	 */
	paths.pop_back();
	visitor.set_visited(source, false);
}

/*
 * Get all paths between two vertices: source and target
 *
 * @param [in] g       the graph.
 * @param [in] source  the source vertex.
 * @param [in] target  the target vertex.
 */
template<typename T>
void
get_paths(const graph<T> &g, const T &source, const T &target)
{
	if (source != target) {
		visitor<T> visitor;
		vector<T> paths;
		get_paths(g, visitor, g.get_vertex(source), target, paths);
	} else {
		cout << source << endl;
	}
}
``` 
## Traversal Problem 3
*Problem:* Given a graph with vertices v<sub>1</sub>, v<sub>2</sub>, v<sub>3</sub>, ..., v<sub>n - 1</sub>, v<sub>n</sub>, find if there is a cycle in the graph.<br>
*Solution:* Perform DFS traversal. Along with keeping track of vertices that have already been visited, maintain a hierarchy table. The table has two columns:
* current vertex
* vertex from where the current vertex is visited or simply the parent vertex.

Before inserting the relationship, check if the vertex already exists in the hierarchy table. If it does there is a loop. However there is a special case to be handled here. It is the edge going back to the parent for undirected graphs. We do not want to count (*v<sub>1</sub> -> v<sub>2</sub>*) and (*v<sub>2</sub> -> v<sub>1</sub>*) as a loop. Accounting all these considerations, we have the following algorithm:
```C++
/*
 * Is there a cycle in the graph?
 *
 * @param [in] g             the graph.
 * @param [in] visitor       the visitor class.
 * @param [in] current       the current vertex being visited.
 * @param [in] parent        the parent vertex of the current vertex.
 * @param [inout] hierarchy  maintains parent/child relationship for each vertex.
 *
 * @return true if there is a cycle and false otherwise.
 * If there is a loop, the vertices in the loop are printed as well.
 */
template<typename T>
static bool
is_cyclic(const graph<T> &g, visitor<T> &visitor, const vertex<T> &current, T parent, map<T, T> &hierarchy)
{
	if (hierarchy.end() == hierarchy.find(current.vrtx)) {
		/*
		 * Not in hierarchy at all; add the relationship.
		 */
		hierarchy[current.vrtx] = parent;
	} else {
		/*
		 * Already in hierarchy. Check if it is an edge
		 * going back to the parent (forming a small loop
		 * involving just two nodes).
		 *
		 * Hierarchy table for graph 1 <--> 2
		 * vertex | parent
		 *   1    |  -1
		 *   2    |   1
		 * This function is called with current = 1, parent = 2.
		 */
		typename map<T, T>::const_iterator it = hierarchy.find(parent);
		if (it != hierarchy.end()) {
			if (!g.is_directed() && (it->second == current.vrtx)) {
				/*
				 * It is a back-edge to the immediate parent. Ignore it.
				 */
				return false;
			} else {
				/*
				 * Print the cycle and return true.
				 */
				cout << current.vrtx << " " << parent << " ";
				while (it != hierarchy.end()) {
					cout << it->second << " ";
					it = hierarchy.find(it->second);
					if (it->first == current.vrtx)
						break;
				}
				cout << endl;
				return true;
			}
		}
	}

	if (!visitor.is_visited(current)) {
		/*
		 * Not visited yet; mark as visited.
		 */
		visitor.set_visited(current, true);
	
		typename vector<T>::const_iterator it;
		for (it = current.adjacent.begin(); it != current.adjacent.end(); ++it) {
			if (is_cyclic(g, visitor, g.get_vertex(*it), current.vrtx, hierarchy))
				return true;
		}
	}

	/* Backtrack */
	hierarchy.erase(current.vrtx);
	return false;
}

/*
 * Is there a cycle in the graph?
 *
 * @param [in] g       the graph.
 * @param [in] parent  the sentinel vertex, usually -1 or NULL.
 *
 * @return true if there is a cycle in the graph, false otherwise.
 */
template<typename T>
bool
is_cyclic(const graph<T> &g, T parent)
{
	map<T, T> hierarchy;
	visitor<T> visitor;

	typename vector<vertex<T>>::const_iterator it;
	for (it = g.get_vertices().begin(); it != g.get_vertices().end(); ++it) {
		if (is_cyclic(g, visitor, *it, parent, hierarchy))
			return true;
	}

	return false;
}
```

## Directed Acyclic Graph (DAG)
DAG is a directed graph with no cycles in it.
```C++
/*
 * Is the graph a directed acyclic graph (DAG)?
 *
 * @param [in] g       the graph.
 * @param [in] parent  the parent vertex of the first vertex; usually -1 for T = int.
 *
 * @return true if there graph is a DAG, false otherwise.
 */
template<typename T>
bool
is_dag(const graph<T> &g, T parent)
{
	if (!g.is_directed())
		return false;
	return !is_cyclic(g, parent);
}
```

## Topological Sorting
Suppose we have *n* jobs at hand with the following constraints:
* There are only finite number of jobs (which means there is a sink or, in other words, there is at least one job with no dependent job).
* Only one job can be performed at a time.
* Certain jobs depend on other jobs. They can only be started when the dependent tasks are done.

The algorith to generate a schedule is called **topological sorting**. Some examples of use in real life are:
* Designing the assembly line for a component in a manufacturing division.
* Generating the classroom schedule.

The problem can be formulated as a **DAG** where each job represents a vertex and the job dependencies are represented by edges. Let's take an example. There are 6 courses (*A, B, C, D, E, F*) to be taken to get a certain certification. However for many courses there is one or more dependent courses (the dependent courses must be completed before taking a course).

Course | Dependent Course
-------|-----------------
A      | -
B      | A
C      | B
D      | A, C
E      | C, D
F      | E

This can be represented as the following DAG with A as the source (no dependency) and E as the sink (no outedge).
![Topological Sorting](topological-sorting.jpeg)

Alternative this could also be represented in a straight-line as following:
![Toplogical Sorting Straight Line](topological-sorting-reordered.jpeg)

As can been seen, A has indegree of 0. It is not dependent on any course. A is our **source**. F has outdegree of 0. It is our **sink**. The algorithm for topological sorting involves finding the vertex with indegree of 0 and removing the vertex and the outgoing edges. Put this in a queue. Repeat the process until we are left with no more vertices. Let's see how this applies to our case.
* A has indegree of 0. Remove A. Add A to a queue. Q = A.
* Now B has indegree of 0. Remove B. Add B to the queue. Q = A B.
* C now has indegree of 0. Remove C. Add C to the queue. Q = A B C.
* D now has indegree of 0. Remove D. Add D to the queue. Q = A B C D.
* E now has indegree of 0. Remove E. Add E to the queue. Q = A B C D E.
* We are left with just F which too has indegree of 0. Remove F. Add F to the queue. Q = A B C D E F.

The algorithm involves:
* Calculating the indegree of all the vertices. This can be done with **DFS**.
* Removing vertex with indegree of 0. This also mean updating the indegree of vertices adjacent to the removed vertex.
* Putting the removed vertex in a queue.

There is an alternate approach to get the same result. Perform **DFS**. When you are returning from the last vertex (*sink*), add it to a stack. The stack holds the results of topological sorting in reversed order. When you pop the vertices from the stack, you get the correct order. In short, we do DFS with a specialized visitor.
```C++
/*
 * A visitor subclass to find to do topological sorting.
 */
template<typename T>
class topological_sort : public visitor<T>
{
private:
	stack<T>     _stk;

public:
	topological_sort() {}
	virtual ~topological_sort() {}

	void pre(const vertex<T> &) {}
	void post(const vertex<T> &v)
	{
		T vrtx = v;
		_stk.push(vrtx);
	}

	bool empty() const { return _stk.empty(); }
	const T &top() const { return _stk.top(); } 
	void pop() { _stk.pop(); }
};
```

## Traversal problem 4
*Problem:* Determine if a given graph is a **bipartite**.<br>
*Solution:* A bipartite graph is one whose vertices could be divided in two sets such all the edges connect vertices from one set to vertices in another set.<br>
Edges allowed (connected vertices from different sets):
* (v<sub>m-set1</sub>, v<sub>n-set2</sub>)
* (v<sub>s-set2</sub>, v<sub>t-set1</sub>)

Edges not allowed (connected vertices from the same set):
* (v<sub>M-set1</sub>, v<sub>N-set1</sub>)
* (v<sub>S-set2</sub>, v<sub>T-set2</sub>)

The way to solve this is to perform DFS. Assign altenate colors to vertices are we go down the tree. If we hit a condition where the current vertex being visited has the same color as the adjacent one, the graph is not bipartite.
```C++
enum color_t { red, blue };

/*
 * Is a graph bipartite?
 *
 * @param [in]    g          the graph.
 * @param [in]    visitor    the visitor class.
 * @param [in]    current    the current vertex being visited.
 * @param [in]    curr_color the color of the current vertex.
 * @param [inout] vrtx_col   the map of colored vertex.
 *
 * @return true if the graph is bipartite, false otherwise.
 */
template<typename T>
static bool
is_bipartite(
	const graph<T> &g,
	visitor<T> &visitor,
	const vertex<T> &current,
	color_t curr_color,
	map<T, color_t> &vrtx_col)
{
	visitor.set_visited(current, true);
	// Get the next color
	color_t next_color = (curr_color == red) ? blue : red;

	typename vector<T>::const_iterator it;
	for (it = current.adjacent.begin(); it != current.adjacent.end(); ++it) {
		if (visitor.is_visited(*it)) {
			/*
			 * If the vertex is already visited and is not the expected
			 * color, it is not a bipartite graph.
			 */
			if (vrtx_col[*it] != next_color)
				return false;
		} else {
			/*
			 * Mark the next vertex with next_color i.e.,
			 * if current is red, mark it blue
			 * if current is blue, mark it red
			 */
			vrtx_col[*it] = next_color;
			if (!is_bipartite(g, visitor, g.get_vertex(*it), next_color, vrtx_col))
				return false;
		}
	}

	return true;
}

/*
 * Is a bipartite graph?
 *
 * @param [in]  g      the graph.
 *
 * @return true if the graph is bipartite, false otherwise.
 */
template<typename T>
bool
is_bipartite(const graph<T> &g)
{
	visitor<T> visitor;
	map<T, color_t> vrtx_col;

	typename vector<vertex<T>>::const_iterator it;
	for (it = g.get_vertices().begin(); it != g.get_vertices().end(); ++it) {
		if (!visitor.is_visited(*it)) {
			// Mark the first unvisited vertex as red
			vrtx_col[it->vrtx] = red;
			if (!is_bipartite(g, visitor, *it, red, vrtx_col))
				return false;
		}
	}

	return true;
}
```

## Connected components
### Undirected graph
![Connected components in undirected graph](undirected.jpeg)
The above graph has 3 **connected components (CC)**. Usually a graph is pre-processed to find the connected components. A *component ID* is assigned to each vertex. Thereafter, we can easily answer the following questions:
* How many connected components are in the graph?
* Is vertex, *v* connected to vertex, *w*? We can answer this in constant time by checking if both *v* and *w* have the same *component ID*.

#### How to assign component IDs to the vertices of an undirected graph?
* Initialize component ID to 1.
* Pick an unmarked vertex. Perform DFS marking the vertex as visited and setting the component ID to 1.
* Increment component ID.
* Pick the next unmarked vertex. Perform DFS marking the vertex as visited and setting the component ID to 2.
* Continue until all the vertices are marked.

In the code below, the *visitor* class is overridden so that *post* prepares a vertex -> component ID map.
```C++
/*
 * A visitor subclass to find the connected components in a graph.
 * The first component ID is 1 and is incremented.
 * A map of vertex to component ID is prepared while traversing the graph.
 */
template<typename T>
class connected_components : public visitor<T>
{
private:
	map<T, int>  cc_map;
	int          cc_count;

public:
	connected_components() : visitor<T>(), cc_count(0) {}
	virtual ~connected_components() {}

	int num_of_comp() const { return cc_count; }

	bool connected(const T &v1, const T &v2) const
	{
		typename map<T, int>::const_iterator it1, it2;
		it1 = cc_map.find(v1);
		it2 = cc_map.find(v2);
		return ((it1 != cc_map.end()) && (it2 != cc_map.end()) && (it1->second == it2->second));
	}

	void next_component()
	{
		cc_count++;
	}

	void pre(const vertex<T> &) {}

	void post(const vertex<T> &v)
	{
		cc_map[v.vrtx] = cc_count;
	}
};

/*
 * Traverses the graph using dfs and prepares a connected component map.
 *
 * @param [in]    g       the graph.
 * @param [inout] cc      the visitor for connected components.
 */
template<typename T>
void
find_connected_components(const graph<T> &g, connected_components<T> &cc)
{
	typename vector<vertex<T>>::const_iterator it;
	for (it = g.get_vertices().begin(); it != g.get_vertices().end(); ++it) {
		if (!cc.is_visited(*it))
			cc.next_component();
		dfs(g, cc, *it);
	}
}
```
### Directed graph
![Connected components in directed graph](directed.jpeg)
Two vertices, *v* and *w* are called **strongly connected** if there is a path from *v* to *w* and from *w* back to *v*. This implies a loop and *v* and *w* are two vertices of that loop. A directed graph could be divided in multiple such **strongly connected components (SCC)**. If each strong connected component is considered as a single entity (**metagraph**), we end up with a **DAG**. Like connected components of an undirected graph, the graph is usually preprocessed and the above questions (number of strongly connected components and if two vertices are strongly connected) can be similarly answered.

#### How to assign strong component IDs to the vertices of an directed graph?
* Reverse the graph. The reversed graph has the same strong components.
* Perform **DFS**. When you are returning from the last vertex (*sink*), add it to a stack. The stack holds the results of topological sorting in reversed order. Basically we are doing topological sort.
* Initialize component ID to 1.
* Pop a vertex from the stack. Perform DFS on the original graph and not the reversed one marking the vertex as visited and setting the component ID to 1.
* Increment component ID.
* Pop the next vertex. If unmarked, perform DFS marking the vertex as visited and setting the component ID to 2.
* Continue until all the vertices are popped.
```C++
/*
 * Finds the strongly connected components of a directed graph.
 *
 * @param [in]    g       the graph.
 * @param [inout] scc     the visitor for strongly connected components.
 *
 * The same visitor is used for strongly connected components as connected
 * component. The difference is:
 * - the graph is first reversed.
 * - a topological sort is performed.
 * - the vertices (of the original graph) are visited in the topological sort order.
 */
template<typename T>
void
find_strongly_connected_components(const graph<T> &g, connected_components<T> &scc)
{
	graph<T> gr = std::move(g.reverse());
	stack<T> stk;
	topological_sort(gr, stk);

	while (!stk.empty()) {
		T v = stk.top();
		stk.pop();

		if (!scc.is_visited(v))
			scc.next_component();
		dfs(g, scc, g.get_vertex(v));
	}
}
```

## Eulerian graphs
Google Konigsberh puzzle.<br>
*Problem:* For a given undirected graph G(V, E) with vertices v<sub>1</sub>, v<sub>2</sub>, v<sub>3</sub>, ..., v<sub>n - 1</sub>, v<sub>n</sub>, is it possible to traverse the whole graph and come back to the starting vertex, say v<sub>m</sub>, visiting each egde only once?<br>
*Solution:* Euler concluded that it is possible iff the following conditions are true:<br>
* The graph is a connected graph.
* All the vertices have even degrees i.e., each vertex has even number of adjacent vertices.

## Glossary
* A graph is called **connected** if there is a path from any vertex in the graph to any other vertex.
* A  **circuit** is a path where starting vertex is same as the last vertex.
* A **forest** is a graph that does not contain a cycle.
* A **tree** is a connected forest i.e., path from any vertex to any other vertex and no cycles.

# Weighted graph

A graph where weights (cost/length/time, etc.) are associated with edges.

## Single Source Shortest Path (SSSP)
The problem is to find shortest paths from a given vertex *v* to all other vertices of the graph.

A simple data structure is used to:
- track weight at each vertex (we are tracking cumulative weight).
- track the parent vertex that is used to reach the current vertex. This enables us to generate the whole path.

```C++
/*
 * Manages shortest path details.
 */
template<typename T>
class shortest_path
{
private:
	T sentinel;                     // sentinel to indicate no parent, typically -1 or null.
	vector<edge<T>> paths;          // all paths recorded
	set<T> vertices;                // all known vertices

	const T &find_parent(const T &v, int &w)
	{
		typename vector<edge<T>>::const_iterator it;
		for (it = paths.begin(); it != paths.end(); ++it) {
			if (it->to == v) {
				w = it->weight;	
				return it->from;
			}
		}
		w = 0;
		return sentinel;
	}

	/*
	 * Print path taken from src vertex to dest vertex.
	 */
	void path(const T &src, const T &dest)
	{
		int w = 0;
		T v = dest;

		do {
			const T &to = find_parent(v, w);
			cout << v << " (" << w << "), ";
			w = 0;
			v = to;
		} while (v != src);

		cout << src << " (" << 0 << ")" << endl;
	}

public:
	shortest_path(const T &s) : sentinel(s) {}

	const T &sentinel_value() const { return sentinel; }

	/*
	 * If the vertex has already been visited, return the assigned weight. Else
	 * return INT_MAX (representative of infinity).
	 */
	int weight(const T &v) const
	{
		typename vector<edge<T>>::const_iterator it;
		for (it = paths.begin(); it != paths.end(); ++it) {
			if (it->to == v)
				return it->weight;
		}
		return INT_MAX;
	}

	/*
	 * Assign weight to the vertex. No parent.
	 */
	void add(const T &v, int w)
	{
		add(sentinel, v, w);
	}

	/*
	 * Assign weight to the vertex.
	 */
	void add(const T &p, const T &v, int w)
	{
		if (p != sentinel)
			vertices.insert(p);
		vertices.insert(v);

		typename vector<edge<T>>::iterator it;
		for (it = paths.begin(); it != paths.end(); ++it) {
			if (it->to == v) {
				it->from = p;
				it->weight = w;
				return;
			}
		}

		edge<T> e(p, v, w);
		paths.push_back(e);
	}

	void dump(const T &src)
	{
		for (auto v : vertices) {
			if (v != src)
				path(src, v);
		}
	}
};
```

### Special case (DAG)
- Perform toplogical sort.
- All vertices, other than the starting vertex *v*, are assigned weight of infinity.
- The starting vertex *v* is assigned the weight of 0.
- The other vertices are traversed in the **toplogical sort** order and the cummulative path to each vertex is calculated.
- If the computed path is shorter than what has already been seen, the weight assigned to the vertex is updated.
```C++
/*
 * Perform topological sorting.
 *
 * @param [in]  g       the weighted graph.
 * @param [in]  visitor the visitor class.
 * @param [in]  current the current vertex being visited.
 * @param [in]  stop    the last vertex to add to the stack.
 * @param [out] stk     the toplogical sorted vertex on return.
 */
template<typename T>
static void
topological_sort(const weighted_graph<T> &g, visitor<T> &visitor, const vertex<T> &current, const T &stop, stack<T> &stk)
{
	if (visitor.is_visited(current))
		return;

	visitor.set_visited(current, true);

	typename vector<edge<T> *>::const_iterator it;
	for (it = current.adjacent.begin(); it != current.adjacent.end(); ++it) {
		const edge<T> *e = *it;
		topological_sort(g, visitor, g.get_vertex(e->to), stop, stk);
	}

	/*
	 * Add the vertex to the stack until vertex 'stop' is seen.
	 */
	if (stk.empty() || (stk.top() != stop))
		stk.push(current.vrtx);
}

/*
 * Perform topological sorting.
 *
 * @param [in]  g      the graph.
 * @param [in]  stop   the last vertex to add to the stack.
 * @param [out] stk    the toplogical sorted vertex on return.
 */
template<typename T>
void
topological_sort(const weighted_graph<T> &g, const T &stop, stack<T> &stk)
{
	visitor<T> visitor;

	typename vector<vertex<T>>::const_iterator it;
	for (it = g.get_vertices().begin(); it != g.get_vertices().end(); ++it)
		topological_sort(g, visitor, *it, stop, stk);
}

/*
 * Single Source Shortest Path (SSSP) for DAG.
 *
 * Determine the minimum cost (weight) to traverse all vertices in the graph
 * starting from the given vertex. The cost to traverse the starting
 * vertex is 0. If the starting vertex is not the source of the DAG,
 * some vertices will never be visited.
 *
 * Note: The vertices are visited in topological sort order, starting
 * with the given 'start' vertex.
 *
 * @param [in]  g       the weighted graph.
 * @param [in]  start   the starting vertex.
 * @param [out] sp      the shortest path object with cost table.
 */ 
template<typename T>
void
dag_sssp(const weighted_graph<T> &g, const T &start, shortest_path<T> &sp)
{
	/*
	 * Perform topological sort. Pass 'start' so that no vertex is pushed
	 * on the stack once 'start' is seen.
	 */
	stack<T> stk;
	topological_sort(g, start, stk);

	/* Add the start vertex with weight of 0. */
	sp.add(start, 0);

	while (!stk.empty()) {
		T v = stk.top();
		stk.pop();

		const vertex<T> &current = g.get_vertex(v);

		typename vector<edge<T> *>::const_iterator it;
		for (it = current.adjacent.begin(); it != current.adjacent.end(); ++it) {
			const edge<T> *e = *it;
			/*
			 * Find the new cummulative weight of visiting e->to from e->from.
			 */
			int w = sp.weight(e->from) + e->weight;

			/*
			 * If it is less than what is already in the table, update it.
			 */
			if (w < sp.weight(e->to))
				sp.add(e->from, e->to, w);
		}
	}
}
```

### Generic case (Dijkstra's Algorithm)
When traversing the graph, always pick the next vertex as the one that yields the shortest path. This means all the unvisited adjacent vertices are evaluated first to find the shortest path. Most implementations, however, evaluate the path to all the adjacent vertices and store them in a priority queue. A fetch from the priority queue then gives the vertex with the shortest path.
Similar to the DAG case,
- All vertices, other than the starting vertex *v*, are assigned weight of infinity.
- The starting vertex *v* is assigned the weight of 0.
- The paths to all the unvisited adjacent vertices are evaluated and stored in a priority queue.
- The vertex with the shortest path is evaluated next (as a result of fetching next vertex from the priority queue).
- This continues until all the vertices are visited and the priority queue is empty.
```C++
template<typename T>
struct weight_gt
{
	bool operator()(const edge<T> &v1, const edge<T> &v2)
	{
		return (v1.weight > v2.weight);
	}
};

/*
 * Single Source Shortest Path (SSSP) for generic case.
 *
 * Determine the minimum cost (weight) to traverse all vertices in the graph
 * starting from the given vertex. The cost to traverse the starting
 * vertex is 0.
 *
 * Note: The vertices are visited in priority search order i.e., the next
 * vertex visited is the one with the minimum weight.
 *
 * @param [in]  g       the weighted graph.
 * @param [in]  start   the starting vertex.
 * @param [out] sp      the shortest path object with cost table.
 */ 
template<typename T>
void
sssp(const weighted_graph<T> &g, const T &start, shortest_path<T> &sp)
{
	visitor<T> visitor;

	priority_queue<edge<T>, vector<edge<T>>, weight_gt<T>> pq;

	/* Push the current vertex to the priority queue with weight 0. */
	pq.emplace(sp.sentinel_value(), start, 0);

	/* Add the start vertex with weight of 0. */
	sp.add(start, 0);

	while (!pq.empty()) {
		edge<T> the_edge = pq.top();
		pq.pop();

		visitor.set_visited(the_edge.to, true);

		const vertex<T> &current = g.get_vertex(the_edge.to);

		typename vector<edge<T> *>::const_iterator it;
		for (it = current.adjacent.begin(); it != current.adjacent.end(); ++it) {
			const edge<T> *e = *it;
			if (!visitor.is_visited(e->to)) {
				/*
				 * Find the new cummulative weight of visiting e->to from e->from
				 */
				int w = sp.weight(e->from) + e->weight;

				/*
				 * If it is less than what is already in the table,
				 * - update the shortest path table.
				 * - push the vertex in to the priority queue.
				 */
				if (w < sp.weight(e->to)) {
					sp.add(e->from, e->to, w);
					pq.emplace(e->from, e->to, w);
				}
			}
		}
	}
}
```

## Minimum-cost Spanning Tree (MCST or simply MST)
A **spanning tree** of a given undirected (and connected) graph is a subgraph that is a tree and contains all the vertices of the graph. The **minimum-cost spanning tree** is a spanning tree with minimum cost (or weight) for such a weighted graph.

### Prim's algorithm
An arbitrary starting vertex is chosen. Look at all the edges originating from the starting vertex. Add the edge with the minumum weight to the MST. The edges gives us the next set of vertices to be looked at (provided they are not already visited). As the edges are discovered, new edges are looked at and edges with minumum weight amongst them are added to the MST tree.
```C++
/*
 * Add edges originiating from a given vertex to the priority queue.
 *
 * @param [inout] pq       the priority queue.
 * @param [inout] visitor  the visitor class.
 * @param [in]    origin   the origin vertex.
 */
template<typename T>
void
add_edges(priority_queue<edge<T>, vector<edge<T>>, weight_gt<T>> &pq,
	visitor<T> &visitor,
	const vertex<T> &origin)
{
	/* Mark the vertex as visited. */
	visitor.set_visited(origin, true);

	/*
	 * Add all the edges originating from the origin to the priority queue
	 * (provided the other end-point is not already visited).
	 */
	typename vector<edge<T> *>::const_iterator it;
	for (it = origin.adjacent.begin(); it != origin.adjacent.end(); ++it) {
		const edge<T> *e = *it;
		if (!visitor.is_visited(e->to))
			pq.push(*e);
	}
}

/*
 * Find the minimum-cost spanning tree: Prim
 *
 * @param [in] g the weighted undirected graph.
 *
 * @return edges that constitute the minimum-cost spanning tree.
 */
template<typename T>
vector<edge<T>>
mst_prim(const weighted_graph<T> &g)
{
	size_t vc = g.num_vertices();   // number of vertices in the graph
	size_t ec = vc - 1;             // number of edges in the MST
	visitor<T> visitor;
	vector<edge<T>> mst_edges;
	priority_queue<edge<T>, vector<edge<T>>, weight_gt<T>> pq;

	/*
	 * Pick an arbitrary vertex (the first in our case).
	 */
	add_edges(pq, visitor, g.first());

	/*
	 * While the priority queue is not empty
	 * and not all the edges are discovered.
	 */
	while (!pq.empty() && (mst_edges.size() < ec)) {
		const edge<T> the_edge = pq.top();
		pq.pop();

		// If both the end-point of the edge are visited, continue.
		if (visitor.is_visited(the_edge.to))
			continue;

		// Add the edge to the MST
		mst_edges.push_back(the_edge);

		add_edges(pq, visitor, g.get_vertex(the_edge.to));
	}

	return mst_edges;
}
```

### Kruskal's algorithm
All the edges of the graphs are sorted in ascending order of weight. The edges with lowest weights are chosen and added to the MST. If adding an edge results in a loop, that edge is skipped. Union-Find is an easy way to determine if adding an edge will lead to a loop.
```C++
template<typename T>
bool weight_lt(const edge<T> *v1, const edge<T> *v2)
{
	return (v1->weight < v2->weight);
}

/*
 * Find the minimum-cost spanning tree: Kruskal
 *
 * @param [in] g the weighted undirected graph.
 *
 * @return edges that constitute the minimum-cost spanning tree.
 */
template<typename T>
vector<edge<T>>
mst_kruskal(const weighted_graph<T> &g)
{
	vector<edge<T>> mst_edges;

	/*
	 * Get all the edges of the graph.
	 */
	vector<edge<T> *> edges = g.get_edges();

	/*
	 * Sort the edges based on the weight.
	 */
	sort(edges.begin(), edges.end(), weight_lt<T>);

	union_find<T> uf;

	typename vector<edge<T> *>::const_iterator it;
	for (it = edges.begin(); it != edges.end(); ++it) {
		const edge<T> *e = *it;

		/*
		 * Add the edges to the MST as long as
		 * no loops are formed. The union-find is
		 * used to determine if a loop is formed
		 * by adding an edge.
		 */
		if (!uf.connected(e->from, e->to)) {
			uf.union_op(e->from, e->to);
			mst_edges.push_back(*e);
		}
	}

	return mst_edges;
}
```
