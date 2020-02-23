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
#include "uf.h"

using namespace std;

/*
 * Represents an edge along with its weight.
 */
template<typename T>
struct edge
{
	T   from;    // one end of the edge
	T   to;      // the other end of the edge
	int weight;  // weight of the edge

	explicit edge(const T &v1, const T &v2, int w)
		: from(v1), to(v2), weight(w) {}
	bool operator==(const T &v) const { return (this->to == v); }
};

/*
 * Vertex of a weighted graph.
 */
template<typename T>
struct vertex
{
	T                 vrtx;       // the vertex itself
	vector<edge<T> *> adjacent;   // adjacent vertices

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
	vector<edge<T> *>   edges;

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
		typename vector<edge<T> *>::iterator it;
		for (it = from.adjacent.begin(); it != from.adjacent.end(); ++it) {
			edge<T> *e = *it;
			if (e->to == to.vrtx)
				return;
		}

		edge<T> *e = new edge<T>(from.vrtx, to.vrtx, weight);
		edges.push_back(e);
		from.adjacent.push_back(e);
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
				for (it = v.adjacent.begin(); it != v.adjacent.end(); ++it) {
					const edge<T> *e =  *it;
					os << e->from << " " << e->to << " " << e->weight << endl;
					q.push(e->to);
				}
			}
		}
	}

public:
	weighted_graph(bool dir = true) : directed(dir), count(0) {}

	weighted_graph(istream &is)
	{
		int dir;
		T v1, v2;
		int w;

		is >> dir;
		directed = (dir == 1);

		while (is) {
			is >> v1 >> v2 >> w;
			add_edge(v1, v2, w);
		}
	}

	~weighted_graph()
	{
		typename vector<edge<T> *>::iterator it = edges.begin();
		while (it != edges.end()) {
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
	 * Get the first vertex.
	 */
	const vertex<T> & first() const
	{
		return vertices[0];
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
	const vector<edge<T> *> &get_edges() const { return edges; }

	/*
	 * Find the degree of an vertex, v.
	 */
	size_t degree(const T &v) const
	{
		return get_vertex(v).adjacent.size();
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
		for (auto e : edges)
			cout << e->from << " -> " << e->to << "(" << e->weight << ")" << endl;
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
#if defined(DEBUG)
		cout << "set_visited(" << v << ") = " << boolalpha << val << endl;
#endif
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
 * Path information.
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

	const T &sentinel_value() const { return sentinel; }

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
				sp.add(e->to, e->from, w);
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
					sp.add(e->to, e->from, w);
					pq.emplace(e->from, e->to, w);
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

static int
usage(const char *progname)
{
	cerr << progname << " -in <file> " << endl
		<< "    [-dump]                                 Dump the graph read from file." << endl
		<< "    [-serialize]                            Serialize the graph." << endl
		<< "    [-dag_sssp -v <vertex>]                 DAG single source shortest path." << endl
		<< "    [-sssp -v <vertex>]                     Single source shortest path." << endl
		<< "    [-mst_prim]                             Minumum spanning tree." << endl;
	return 1;
}

enum operation
{
	NONE,
	DUMP,
	SERIALIZE,
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
		} else if (strcmp(argv[i], "-serialize") == 0) {
			op = SERIALIZE;
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

		case MST_PRIM:
			{
				vector<edge<int>> mst_edges = std::move(mst_prim(g));
				int sum = 0;
				for (auto edge : mst_edges) {
					sum += edge.weight;
					cout << edge.from << " " << edge.to << " (" << edge.weight << ")" << endl;
				}
				cout << "Minimum-cost = " << sum << endl;
			}
			break;

		case MST_KRUSKAL:
			{
				vector<edge<int>> mst_edges = std::move(mst_kruskal(g));
				int sum = 0;
				for (auto edge : mst_edges) {
					sum += edge.weight;
					cout << edge.from << " " << edge.to << " (" << edge.weight << ")" << endl;
				}
				cout << "Minimum-cost = " << sum << endl;
			}
			break;

		default:
			break;
	}

	return 0;
}
