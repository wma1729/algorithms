#include <fstream>
#include <cstring>
#include <string>
#include <map>
#include <stack>
#include <climits>
#include "graph.h"
#include "uf.h"

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

	vector<T> adjacent = std::move(from.adjacent());

	typename vector<T>::const_iterator it;
	for (it = adjacent.begin(); it != adjacent.end(); ++it)
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
	for (it = g.vertices().begin(); it != g.vertices().end(); ++it)
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
	q.push(from);
	visitor.set_visited(from, true);

	while (!q.empty()) {
		T v = q.front();
		q.pop();

		const vertex<T> &current = g.get_vertex(v);
		visitor.pre(current);

		// Now add all the adjacent ones to the queue.
		vector<T> adjacent = std::move(current.adjacent());

		typename vector<T>::const_iterator it;
		for (it = adjacent.begin(); it != adjacent.end(); ++it) {
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
	for (it = g.vertices().begin(); it != g.vertices().end(); ++it) {
		if (!visitor.is_visited(*it))
			bfs(g, visitor, *it);
	}
}

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

	vector<T> adjacent = std::move(source.adjacent());

	typename vector<T>::const_iterator it;
	for (it = adjacent.begin(); it != adjacent.end(); ++it) {
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
	paths.push_back(source);

	vector<T> adjacent = std::move(source.adjacent());

	typename vector<T>::const_iterator it;
	for (it = adjacent.begin(); it != adjacent.end(); ++it) {
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
	if (hierarchy.end() == hierarchy.find(current)) {
		/*
		 * Not in hierarchy at all; add the relationship.
		 */
		hierarchy[current] = parent;
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
			if (!g.directed() && (it->second == current)) {
				/*
				 * It is a back-edge to the immediate parent. Ignore it.
				 */
				return false;
			} else {
				/*
				 * Print the cycle and return true.
				 */
				cout << current << " " << parent << " ";
				while (it != hierarchy.end()) {
					cout << it->second << " ";
					it = hierarchy.find(it->second);
					if (it->first == current)
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

		vector<T> adjacent = std::move(current.adjacent());
	
		typename vector<T>::const_iterator it;
		for (it = adjacent.begin(); it != adjacent.end(); ++it) {
			T parent = current;
			if (is_cyclic(g, visitor, g.get_vertex(*it), parent, hierarchy))
				return true;
		}
	}

	/* Backtrack */
	hierarchy.erase(current);
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
	for (it = g.vertices().begin(); it != g.vertices().end(); ++it) {
		if (is_cyclic(g, visitor, *it, parent, hierarchy))
			return true;
	}

	return false;
}

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
	if (!g.directed())
		return false;
	return !is_cyclic(g, parent);
}

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

	vector<T> adjacent = std::move(current.adjacent());

	typename vector<T>::const_iterator it;
	for (it = adjacent.begin(); it != adjacent.end(); ++it) {
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
	for (it = g.vertices().begin(); it != g.vertices().end(); ++it) {
		if (!visitor.is_visited(*it)) {
			// Mark the first unvisited vertex as red
			vrtx_col[*it] = red;
			if (!is_bipartite(g, visitor, *it, red, vrtx_col))
				return false;
		}
	}

	return true;
}

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
		cc_map[v] = cc_count;
	}

	void dump()
	{
		for (auto ent : cc_map)
			cout << ent.first << " " << ent.second << endl;
	}
};

/*
 * Traverses the graph using dfs and prepares a connected component map for
 * undirected graph.
 *
 * @param [in]    g       the graph.
 * @param [inout] cc      the visitor for connected components.
 */
template<typename T>
void
find_connected_components(const graph<T> &g, connected_components<T> &cc)
{
	typename vector<vertex<T>>::const_iterator it;
	for (it = g.vertices().begin(); it != g.vertices().end(); ++it) {
		if (!cc.is_visited(*it))
			cc.next_component();
		dfs(g, cc, *it);
	}
}

/*
 * Finds the strongly connected components of a directed graph.
 *
 * @param [in]    g           the graph.
 * @param [inout] scc         the visitor for strongly connected components.
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
	topological_sort<T> sorter;
	dfs(gr, sorter);

	while (!sorter.empty()) {
		T v = sorter.top();
		sorter.pop();

		if (!scc.is_visited(v))
			scc.next_component();
		dfs(g, scc, g.get_vertex(v));
	}
}

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

	const T find_parent(const T &v, double &w)
	{
		typename vector<edge<T>>::const_iterator it;
		for (it = paths.begin(); it != paths.end(); ++it) {
			if (it->sink() == v) {
				w = it->weight();	
				return it->source();
			}
		}
		w = 0.0;
		return sentinel;
	}

	/*
	 * Print path taken from src vertex to dest vertex.
	 */
	void path(const T &src, const T &dest)
	{
		double w = 0.0;
		T v = dest;

		do {
			const T &to = find_parent(v, w);
			cout << v << " (" << w << "), ";
			w = 0;
			v = to;
		} while (v != src);

		cout << src << " (" << 0.0 << ")" << endl;
	}

public:
	shortest_path(const T &s) : sentinel(s) {}

	const T &sentinel_value() const { return sentinel; }

	/*
	 * If the vertex has already been visited, return the assigned weight. Else
	 * return INT_MAX (representative of infinity).
	 */
	double weight(const T &v) const
	{
		typename vector<edge<T>>::const_iterator it;
		for (it = paths.begin(); it != paths.end(); ++it) {
			if (it->sink() == v)
				return it->weight();
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
			if (it->sink() == v) {
				it->source(p);
				it->weight(w);
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
dag_sssp(const graph<T> &g, const T &start, shortest_path<T> &sp)
{
	topological_sort<T> sorter;
	dfs(g, sorter);

	/* Add the start vertex with weight of 0. */
	sp.add(start, 0);

	while (!sorter.empty()) {
		T v = sorter.top();
		sorter.pop();

		const vertex<T> &current = g.get_vertex(v);

		typename vector<edge<T> *>::const_iterator it;
		for (it = current.edges().begin(); it != current.edges().end(); ++it) {
			const edge<T> *e = *it;
			/*
			 * Find the new cummulative weight of visiting e->sink() from e->source().
			 */
			double w = sp.weight(e->source()) + e->weight();

			/*
			 * If it is less than what is already in the table, update it.
			 */
			if (w < sp.weight(e->sink()))
				sp.add(e->source(), e->sink(), w);
		}
	}
}

template<typename T>
struct weight_gt
{
	bool operator()(const edge<T> &e1, const edge<T> &e2)
	{
		return (e1.weight() > e2.weight());
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
sssp(const graph<T> &g, const T &start, shortest_path<T> &sp)
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

		visitor.set_visited(the_edge.sink(), true);

		const vertex<T> &current = g.get_vertex(the_edge.sink());

		typename vector<edge<T> *>::const_iterator it;
		for (it = current.edges().begin(); it != current.edges().end(); ++it) {
			const edge<T> *e = *it;
			if (!visitor.is_visited(e->sink())) {
				/*
				 * Find the new cummulative weight of visiting e->sink() from e->source()
				 */
				double w = sp.weight(e->source()) + e->weight();

				/*
				 * If it is less than what is already in the table,
				 * - update the shortest path table.
				 * - push the vertex in to the priority queue.
				 */
				if (w < sp.weight(e->sink())) {
					sp.add(e->source(), e->sink(), w);
					pq.emplace(e->source(), e->sink(), w);
				}
			}
		}
	}
}

/*
 * Add edges originiating from a given vertex to the priority queue.
 *
 * @param [inout] pq       the priority queue.
 * @param [inout] visitor  the visitor class.
 * @param [in]    origin   the origin vertex.
 */
template<typename T>
void
add_edges(
	priority_queue<edge<T>, vector<edge<T>>, weight_gt<T>> &pq,
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
	for (it = origin.edges().begin(); it != origin.edges().end(); ++it) {
		const edge<T> *e = *it;
		if (!visitor.is_visited(e->sink()))
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
mst_prim(const graph<T> &g)
{
	size_t vc = g.count();   // number of vertices in the graph
	size_t ec = vc - 1;      // number of edges in the MST
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
		if (visitor.is_visited(the_edge.sink()))
			continue;

		// Add the edge to the MST
		mst_edges.push_back(the_edge);

		add_edges(pq, visitor, g.get_vertex(the_edge.sink()));
	}

	return mst_edges;
}

template<typename T>
bool weight_lt(const edge<T> *v1, const edge<T> *v2)
{
	return (v1->weight() < v2->weight());
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
mst_kruskal(const graph<T> &g)
{
	vector<edge<T>> mst_edges;

	/*
	 * Get all the edges of the graph.
	 */
	vector<edge<T> *> edges = g.edges();

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
		if (!uf.connected(e->source(), e->sink())) {
			uf.union_op(e->source(), e->sink());
			mst_edges.push_back(*e);
		}
	}

	return mst_edges;
}
static int
usage(const char *progname)
{
	cerr << progname << " -in <file> " << endl
		<< "    [-dump]                                 Dump the graph read from file." << endl
		<< "    [-reverse]                              Reverse a directed graph." << endl
		<< "    [-degree -v <vertex>]                   Degree of the vertex." << endl
		<< "    [-reachable -v <vertex1, vertex2>]      Is vertex2 reachable from vertex1?" << endl
		<< "    [-paths -v <vertex1, vertex2>]          All paths between vertex1 and vertex2." << endl
		<< "    [-serialize]                            Serialize the graph." << endl
		<< "    [-dfs]                                  Depth first search traversal." << endl
		<< "    [-bfs]                                  Breadth first search traversal." << endl
		<< "    [-is_cyclic]                            Is there a cycle in the graph?" << endl
		<< "    [-is_dag]                               Is the graph directed acyclic graph?" << endl
		<< "    [-sort]                                 Topological sorting." << endl
		<< "    [-bipartite]                            Is the graph bipartite?" << endl
		<< "    [-cc_num]                               Number of connected components." << endl
		<< "    [-cc -v <vertex1, vertex2>]             Is vertex1 and vertex2 connected?" << endl
		<< "    [-scc_num]                              Number of connected components." << endl
		<< "    [-scc -v <vertex1, vertex2>]            Is vertex1 and vertex2 connected?" << endl
		<< "    [-dag_sssp -v <vertex>]                 DAG single source shortest path." << endl
		<< "    [-sssp -v <vertex>]                     Single source shortest path." << endl
		<< "    [-mst_prim]                             Minumum spanning tree (Prim's algorithm)." << endl
		<< "    [-mst_kruskal]                          Minumum spanning tree (Kruskal's algorithm)." << endl;
	return 1;
}

enum operation
{
	NONE,
	DUMP,
	REVERSE,
	DEGREE,
	REACHABLE,
	PATHS,
	SERIALIZE,
	DFS,
	BFS,
	IS_CYCLIC,
	IS_DAG,
	TOPOLOGICAL_SORT,
	IS_BIPARTITE,
	CC_NUM,
	CC,
	SCC_NUM,
	SCC,
	DAG_SSSP,
	SSSP,
	MST_PRIM,
	MST_KRUSKAL
};

// Driver code
int
main(int argc, const char **argv)
{
	string file;
	operation op = NONE;
	int v1 = -1, v2 = -1;

	for (int i = 1; i < argc; ++i) {
		if (strcmp(argv[i], "-in") == 0) {
			++i;
			if (argv[i]) {
				file = argv[i];
			} else {
				cerr << "missing argument for " << argv[i] << endl;
				return 1;
			}
		} else if (strcmp(argv[i], "-v") == 0) {
			++i;
			if (argv[i]) {
				stringstream ss(argv[i]);
				while (ss.good()) {
					string strvrtx;
					getline(ss, strvrtx, ',');
					if (v1 == -1)
						v1 = atoi(strvrtx.c_str());
					else
						v2 = atoi(strvrtx.c_str());	
				}
			} else {
				cerr << "missing argument for " << argv[i] << endl;
				return 1;
			}
		} else if (strcmp(argv[i], "-dump") == 0) {
			op = DUMP;
		} else if (strcmp(argv[i], "-reverse") == 0) {
			op = REVERSE;
		} else if (strcmp(argv[i], "-degree") == 0) {
			op = DEGREE;
		} else if (strcmp(argv[i], "-reachable") == 0) {
			op = REACHABLE;
		} else if (strcmp(argv[i], "-paths") == 0) {
			op = PATHS;
		} else if (strcmp(argv[i], "-serialize") == 0) {
			op = SERIALIZE;
		} else if (strcmp(argv[i], "-dfs") == 0) {
			op = DFS;
		} else if (strcmp(argv[i], "-bfs") == 0) {
			op = BFS;
		} else if (strcmp(argv[i], "-is_cyclic") == 0) {
			op = IS_CYCLIC;
		} else if (strcmp(argv[i], "-is_dag") == 0) {
			op = IS_DAG;
		} else if (strcmp(argv[i], "-sort") == 0) {
			op = TOPOLOGICAL_SORT;
		} else if (strcmp(argv[i], "-bipartite") == 0) {
			op = IS_BIPARTITE;
		} else if (strcmp(argv[i], "-cc_num") == 0) {
			op = CC_NUM;
		} else if (strcmp(argv[i], "-cc") == 0) {
			op = CC;
		} else if (strcmp(argv[i], "-scc_num") == 0) {
			op = SCC_NUM;
		} else if (strcmp(argv[i], "-scc") == 0) {
			op = SCC;
		} else if (strcmp(argv[i], "-dag_sssp") == 0) {
			op = DAG_SSSP;
		} else if (strcmp(argv[i], "-sssp") == 0) {
			op = SSSP;
		} else if (strcmp(argv[i], "-mst_prim") == 0) {
			op = MST_PRIM;
		} else if (strcmp(argv[i], "-mst_kruskal") == 0) {
			op = MST_KRUSKAL;
		} else {
			return usage(argv[0]);
		}
	}

	if (file.empty()) {
		cerr << "graph file is not specified using -in." << endl;
		return usage(argv[0]);
	}

	fstream fin(file, ios_base::in);
	graph<int> g(fin);
	stack<int> stk;

	switch (op) {
		case DUMP:
			cout << g;
			break;

		case REVERSE:
			{
				graph<int> gr = std::move(g.reverse());
				cout << gr;
			}
			break;

		case DEGREE:
			cout << g.degree(v1) << endl;
			break;

		case REACHABLE:
			cout << boolalpha << is_reachable(g, v1, v2) << endl;
			break;

		case PATHS:
			get_paths(g, v1, v2);
			break;

		case SERIALIZE:
			g.serialize(cout);
			break;

		case DFS:
			{
				visitor<int> visitor;
				dfs(g, visitor);
			}
			break;

		case BFS:
			{
				visitor<int> visitor;
				bfs(g, visitor);
			}
			break;

		case IS_CYCLIC:
			cout << boolalpha << is_cyclic(g, -1) << endl;
			break;

		case IS_DAG:
			cout << boolalpha << is_dag(g, -1) << endl;
			break;

		case TOPOLOGICAL_SORT:
			{
				topological_sort<int> sorter;
				dfs(g, sorter);
				while (!sorter.empty()) {
					cout << sorter.top() << " ";
					sorter.pop();
				}
				cout << endl;
			}
			break;

		case IS_BIPARTITE:
			cout << boolalpha << is_bipartite(g) << endl;
			break;

		case CC_NUM:
			{
				connected_components<int> cc;
				find_connected_components(g, cc);
				cout << cc.num_of_comp() << endl;
				cc.dump();
			}
			break;

		case CC:
			{
				connected_components<int> cc;
				find_connected_components(g, cc);
				cout << boolalpha << cc.connected(v1, v2) << endl;
			}
			break;

		case SCC_NUM:
			{
				connected_components<int> scc;
				find_strongly_connected_components(g, scc);
				cout << scc.num_of_comp() << endl;
				scc.dump();
			}
			break;

		case SCC:
			{
				connected_components<int> scc;
				find_strongly_connected_components(g, scc);
				cout << boolalpha << scc.connected(v1, v2) << endl;
			}
			break;

		case DAG_SSSP:
			{
				shortest_path<int> sp(-1);
				dag_sssp(g, v1, sp);
				sp.dump(v1);
			}
			break;

		case SSSP:
			{
				shortest_path<int> sp(-1);
				sssp(g, v1, sp);
				sp.dump(v1);
			}
			break;

		case MST_PRIM:
			{
				vector<edge<int>> mst_edges = std::move(mst_prim(g));
				int sum = 0;
				for (auto edge : mst_edges) {
					sum += edge.weight();
					cout << edge;
				}
				cout << "Minimum-cost = " << sum << endl;
			}
			break;

		case MST_KRUSKAL:
			{
				vector<edge<int>> mst_edges = std::move(mst_kruskal(g));
				int sum = 0;
				for (auto edge : mst_edges) {
					sum += edge.weight();
					cout << edge;
				}
				cout << "Minimum-cost = " << sum << endl;
			}
			break;

		default:
			break;
	}

	return 0;
}
