#if !defined(GRAPH_H_)
#define GRAPH_H_

#include <vector>
#include <set>
#include <queue>
#include <istream>
#include <ostream>
#include <sstream>
#include <iostream>
#include <algorithm>

using namespace std;

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
		T vrtx = v;
		cout << vrtx << endl;
	}

	/* Override as needed */
	virtual void post(const vertex<T> &)
	{
		return;
	}

	bool is_visited(const T &v)
	{
		bool b = (visited.end() != visited.find(v));
#if defined(DEBUG)
		cout << "is_visited(" << v << ") = " << boolalpha << b << endl;
#endif
		return b;
	}

	bool is_visited(const vertex<T> &v)
	{
		T vrtx = v;
		return is_visited(vrtx);
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
		T vrtx = v;
		set_visited(vrtx, val);
	}
};

#endif // GRAPH_H_
