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
#include <climits>

using namespace std;

/*
 * Represents an end of the edge along with edge's weight.
 */
template<typename T>
struct edge
{
	T   vrtx;    // end of the edge
	int weight;  // weight of the edge

	explicit edge(T v, int w) : vrtx(v), weight(w) {}
	bool operator==(T v) const { return (this->vrtx == v); }
};

/*
 * Vertex of a weighted graph.
 */
template<typename T>
struct vertex
{
	T                 vrtx;       // the vertex itself
	vector<edge<T>>   adjacent;   // adjacent vertices

	explicit vertex(T v) : vrtx(v) {}
	bool operator==(T v) const { return (this->vrtx == v); }
};

/*
 * Weighted Graph implementation.
 */
template<typename T>
class weighted_graph
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
	 * Add an edge (from, to, weight).
	 */
	void add_edge(vertex<T> &from, const vertex<T> &to, int weight)
	{
		if (from.adjacent.end() == find(from.adjacent.begin(), from.adjacent.end(), to.vrtx))
			from.adjacent.emplace_back(to.vrtx, weight);
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
				typename vector<edge<T>>::const_iterator it;
				for (it = v.adjacent.begin(); it != v.adjacent.end(); ++it) {
					os << from << " " << it->vrtx << " " << it->weight << endl;
					q.push(it->vrtx);
				}
			}
		}
	}

public:
	weighted_graph(bool dir = true) : directed(dir), count(0) {}

	weighted_graph(istream &is)
	{
		int dir;
		int v1, v2, w;

		is >> dir;
		directed = (dir == 1);

		while (is) {
			is >> v1 >> v2 >> w;
			add_edge(v1, v2, w);
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
	 * Add an edge (from, to, weight). If the graph is undirected, edge (to, from)
	 * is added as well.
	 */
	void add_edge(const T &from, const T &to, int weight)
	{
		add_vertex(from);
		add_vertex(to);

		vertex<T> &v1 = get_vertex(from);
		vertex<T> &v2 = get_vertex(to);

		add_edge(v1, v2, weight);
		if (!directed)
			add_edge(v2, v1, weight);
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
	weighted_graph<T> reverse() const
	{
		weighted_graph<T> g(true);

		if (directed) {
			for (auto v : vertices) {
				for (auto adj : v.adjacent) 
					g.add_edge(adj.vrtx, v.vrtx, adj.weight);
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
				cout << adj.vrtx << "(" << adj.weight << ") ";

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

	typename vector<edge<T>>::const_iterator it;
	for (it = current.adjacent.begin(); it != current.adjacent.end(); ++it)
		topological_sort(g, visitor, g.get_vertex(it->vrtx), stop, stk);

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
 * Path information. Structurally similar to edge<T>.
 */
template<typename T>
struct path_info
{
	T   parent;     // parent vertex
	int weight;     // cummulative weight so far

	explicit path_info(const T &p, int w) : parent(p), weight(w) {}
};

/*
 * Manages shortest path details.
 */
template<typename T>
class shortest_path
{
private:
	T sentinel;                     // sentinel to indicate no parent, typically -1 or null.
	map<T, path_info<T>> sp;	// vertex -> (parent-vertex + cummulative-weight)

public:
	shortest_path(const T &s) : sentinel(s) {}

	/*
	 * If the vertex has already been visited, return the assigned weight. Else
	 * return INT_MAX (representative of infinity).
	 */
	int weight(const T &v) const
	{
		typename map<T, path_info<T>>::const_iterator it;
		if ((it = sp.find(v)) != sp.end())
			return it->second.weight;
		return INT_MAX;
	}

	/*
	 * Assign weight to the vertex. No parent.
	 */
	void add(const T &v, int w)
	{
		add(v, sentinel, w);
	}

	/*
	 * Assign weight to the vertex.
	 */
	void add(const T &v, const T &p, int w)
	{
		typename map<T, path_info<T>>::iterator it;
		it = sp.find(v);
		if (it == sp.end()) {
			path_info<T> pi(p, w);
			sp.insert(make_pair(v, pi));
		} else {
			it->second.parent = p;
			it->second.weight = w;
		}
	}

	/*
	 * Return path taken from src vertex to dest vertex.
	 */
	vector<T> path(const T &src, const T &dest)
	{
		vector<T> p;
		const T &v = dest;

		while (sp[v].parent != sentinel) {
			p.push_back(v);
			v = sp[v].parent;
		}

		return p;
	}

	void dump()
	{
		for (auto elem : sp)
			cout << elem.first << " -> "
				<< elem.second.weight << " ("
				<< elem.second.parent << ")"
				<< endl;
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

		typename vector<edge<T>>::const_iterator it;
		for (it = current.adjacent.begin(); it != current.adjacent.end(); ++it) {
			/*
			 * Find the new cummulative weight of visiting it->vrtx from v.
			 */
			int w = sp.weight(v) + it->weight;

			/*
			 * If it is less than what is already in the table, update it.
			 */
			if (w < sp.weight(it->vrtx))
				sp.add(it->vrtx, v, w);
		}
	}
}

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
	pq.emplace(start, 0);

	/* Add the start vertex with weight of 0. */
	sp.add(start, 0);

	while (!pq.empty()) {
		edge<T> wv = pq.top();
		pq.pop();

		visitor.set_visited(wv.vrtx, true);

		const vertex<T> &current = g.get_vertex(wv.vrtx);

		typename vector<edge<T>>::const_iterator it;
		for (it = current.adjacent.begin(); it != current.adjacent.end(); ++it) {
			if (!visitor.is_visited(it->vrtx)) {
				/*
				 * Find the new cummulative weight of visiting it->vrtx from v.
				 */
				int w = sp.weight(wv.vrtx) + it->weight;

				/*
				 * If it is less than what is already in the table,
				 * - update the shortest path table.
				 * - push the vertex in to the priority queue.
				 */
				if (w < sp.weight(it->vrtx)) {
					sp.add(it->vrtx, wv.vrtx, w);
					pq.emplace(it->vrtx, w);
				}
			}
		}
	}
}

static int
usage(const char *progname)
{
	cerr << progname << " -in <file> " << endl
		<< "    [-dump]                                 Dump the graph read from file." << endl
		<< "    [-serialize]                            Serialize the graph." << endl
		<< "    [-dag_sssp -v <vertex>]                 DAG single source shortest path." << endl
		<< "    [-sssp -v <vertex>]                     Single source shortest path." << endl;
	return 1;
}

enum operation
{
	NONE,
	DUMP,
	SERIALIZE,
	DAG_SSSP,
	SSSP
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
		} else if (strcmp(argv[i], "-serialize") == 0) {
			op = SERIALIZE;
		} else if (strcmp(argv[i], "-dag_sssp") == 0) {
			op = DAG_SSSP;
		} else if (strcmp(argv[i], "-sssp") == 0) {
			op = SSSP;
		} else {
			return usage(argv[0]);
		}
	}

	if (file.empty()) {
		cerr << "graph file is not specified using -in." << endl;
		return usage(argv[0]);
	}

	fstream fin(file, ios_base::in);
	weighted_graph<int> g(fin);

	switch (op) {
		case DUMP:
			g.dump();
			break;

		case SERIALIZE:
			g.serialize(cout);
			break;

		case DAG_SSSP:
			{
				shortest_path<int> sp(-1);
				dag_sssp(g, v1, sp);
				sp.dump();
			}
			break;

		case SSSP:
			{
				shortest_path<int> sp(-1);
				sssp(g, v1, sp);
				sp.dump();
			}
			break;

		default:
			break;
	}

	return 0;
}
