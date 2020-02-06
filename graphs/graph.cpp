#include <iostream>
#include <ostream>
#include <sstream>
#include <fstream>
#include <cstring>
#include <string>
#include <vector>
#include <set>
#include <map>
#include <queue>
#include <stack>
#include <algorithm>

using namespace std;

/*
 * Vertex of a graph.
 */
template<typename T>
struct vertex
{
	T           vrtx;       // the vertex itself
	vector<T>   adjacent;   // adjacent vertices

	explicit vertex(T v) : vrtx(v) {}
	bool operator==(T v) const { return (this->vrtx == v); }
};

/*
 * Graph implementation.
 */
template<typename T>
class graph
{
private:
	bool                directed;       // directed or undirected?
	size_t              count;          // number of vertices
	vector<vertex<T>>   vertices;       // vertices in the graph

	/*
	 * Get a vertex, v.
	 * Throws out_of_range exception if the vertex is not found.
	 */
	vertex<T> & get_vertex(const T &v)
	{
		typename vector<vertex<T>>::iterator it = find(vertices.begin(), vertices.end(), v);
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
	void add_edge(vertex<T> &from, const vertex<T> &to)
	{
		if (from.adjacent.end() == find(from.adjacent.begin(), from.adjacent.end(), to.vrtx))
			from.adjacent.push_back(to.vrtx);
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
				typename vector<T>::const_iterator it;
				for (it = v.adjacent.begin(); it != v.adjacent.end(); ++it) {
					os << from << " " << *it << endl;
					q.push(*it);
				}
			}
		}
	}

public:
	graph(bool dir = true) : directed(dir), count(0) {}

	graph(istream &is)
	{
		int dir;
		int v1, v2;

		is >> dir;
		directed = (dir == 1);

		while (is) {
			is >> v1 >> v2;
			add_edge(v1, v2);
		}
	}

	/*
	 * Add a vertex, v.
	 */
	void add_vertex(const T &v)
	{
		typename vector<vertex<T>>::iterator it = find(vertices.begin(), vertices.end(), v);
		if (it == vertices.end()) {
			vertices.emplace_back(v);
			++count;
		}
	}

	/*
	 * Get a vertex, v.
	 * Throws out_of_range exception if the vertex is not found.
	 */
	const vertex<T> & get_vertex(const T &v) const
	{
		typename vector<vertex<T>>::const_iterator it = find(vertices.begin(), vertices.end(), v);
		if (it == vertices.end()) {
			ostringstream oss;
			oss << "vertex " << v << " not found";
			throw out_of_range(oss.str());
		} else {
			return *it;
		}
	}

	/*
	 * Add an edge (from, to). If the graph is undirected, edge (to, from)
	 * is added as well.
	 */
	void add_edge(const T &from, const T &to)
	{
		add_vertex(from);
		add_vertex(to);

		vertex<T> &v1 = get_vertex(from);
		vertex<T> &v2 = get_vertex(to);

		add_edge(v1, v2);
		if (!directed)
			add_edge(v2, v1);
	}

	bool is_directed() const { return directed; }
	size_t num_vertices() const { return count; }
	const vector<vertex<T>> &get_vertices() const { return vertices; }

	/*
	 * Find the degree of an vertex, v.
	 */
	size_t degree(const T &v) const
	{
		return get_vertex(v).adjacent.size();
	}

	/*
	 * Reverse a graph.
	 */
	graph<T> reverse() const
	{
		graph<T> g(true);

		if (directed) {
			for (auto v : vertices) {
				for (auto adj : v.adjacent) 
					g.add_edge(adj, v.vrtx);
			}
		} else {
			g = *this;
		}

		return g;
	}

	/*
	 * Serialize a graph.
	 * Line 1: 0|1 (undirected or directed)
	 * Line 2: v1 v2 (two vertices of an edge)
	 * Line 3: v3 v4 (two vertices of an edge)
	 * ...
	 */
	void serialize(ostream &os)
	{
		set<T> visited;

		cout << (directed ? "1" : "0") << endl;
		
		typename vector<vertex<T>>::const_iterator it;
		for (it = vertices.begin(); it != vertices.end(); ++it)
			serialize(os, visited, it->vrtx);
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
			if (it->second == current.vrtx) {
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
 * @param [in] parent  the parent vertex of the first vertex; usually -1 for T = int.
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

/*
 * Perform topological sorting.
 *
 * @param [in]  g       the graph.
 * @param [in]  visitor the visitor class.
 * @param [in]  current the current vertex being visited.
 * @param [out] stk     the toplogical sorted vertex on return.
 */
template<typename T>
static void
topological_sort(const graph<T> &g, visitor<T> &visitor, const vertex<T> &current, stack<T> &stk)
{
	if (visitor.is_visited(current))
		return;

	visitor.set_visited(current, true);

	typename vector<T>::const_iterator it;
	for (it = current.adjacent.begin(); it != current.adjacent.end(); ++it)
		topological_sort(g, visitor, g.get_vertex(*it), stk);

	/*
	 * Add the vertex to the stack.
	 */
	stk.push(current.vrtx);
}

/*
 * Perform topological sorting.
 *
 * @param [in]  g      the graph.
 * @param [out] stk    the toplogical sorted vertex on return.
 */
template<typename T>
void
topological_sort(const graph<T> &g, stack<T> &stk)
{
	visitor<T> visitor;

	typename vector<vertex<T>>::const_iterator it;
	for (it = g.get_vertices().begin(); it != g.get_vertices().end(); ++it)
		topological_sort(g, visitor, *it, stk);
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
		cc_map[v.vrtx] = cc_count;
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
	for (it = g.get_vertices().begin(); it != g.get_vertices().end(); ++it) {
		if (!cc.is_visited(*it))
			cc.next_component();
		dfs(g, cc, *it);
	}
}

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
		<< "    [-cc_num]                               Number of connected components." << endl
		<< "    [-cc -v <vertex1, vertex2>]             Is vertex1 and vertex2 connected?" << endl
		<< "    [-scc_num]                              Number of connected components." << endl
		<< "    [-scc -v <vertex1, vertex2>]            Is vertex1 and vertex2 connected?" << endl;
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
	CC_NUM,
	CC,
	SCC_NUM,
	SCC
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
		} else if (strcmp(argv[i], "-cc_num") == 0) {
			op = CC_NUM;
		} else if (strcmp(argv[i], "-cc") == 0) {
			op = CC;
		} else if (strcmp(argv[i], "-scc_num") == 0) {
			op = SCC_NUM;
		} else if (strcmp(argv[i], "-scc") == 0) {
			op = SCC;
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
			g.dump();
			break;

		case REVERSE:
			{
				graph<int> gr = std::move(g.reverse());
				gr.dump();
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
			topological_sort(g, stk);
			while (!stk.empty()) {
				cout << stk.top() << " ";
				stk.pop();
			}
			cout << endl;
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

		default:
			break;
	}

	return 0;
}
