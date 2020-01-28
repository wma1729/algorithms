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
		 * Backtrack.
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
		if (visited.end() != visited.find(from.vrtx))
			return;

		/*
		 * Not visited yet.
		 * Process and mark as visited.
		 */
		(*visitor)(from.vrtx);
		visited.insert(from.vrtx);

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

	/*
	 * Serialize the graph using BFS.
	 */
	void serialize(ostream &os, set<T> &visited, T from)
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
				visited.insert(from);

				vertex &v = get_vertex(from);

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

	/*
	 * Is there a cycle in the graph?
	 *
	 * @param hierarchy maintains a (vertex -> parent vertex) map.
	 * @param visited   maintains a set of vertices that are already visited.
	 * @param v         vertex being visited.
	 * @param parent    parent vertex. -1 if there is no parent.
	 *
	 * @return true if there is a cycle and false otherwise.
	 * If there is a loop, the vertices in the loop are printed as well.
	 */
	bool is_cyclic(map<T, T> &hierarchy, set<T> &visited, T v, T parent)
	{
		if (hierarchy.end() == hierarchy.find(v)) {
			/*
			 * Not in hierarchy at all; add the relationship.
			 */
			hierarchy[v] = parent;
		} else {
			/*
			 * Already in hierarchy. Check if it is a back-edge.
			 * Hierarchy table for graph 1 <--> 2
			 * vertex | parent
			 *   1    |  -1
			 *   2    |   1
			 * This function is called with v = 1, parent = 2.
			 */
			typename map<T, T>::const_iterator it = hierarchy.find(parent);
			if (it != hierarchy.end()) {
				if (it->second == v) {
					/*
					 * It is a back-edge. Ignore it.
					 */
					return false;
				} else {
					/*
					 * Print the cycle and return true.
					 */
					cout << v << " " << parent << " ";
					while (it != hierarchy.end()) {
						cout << it->second << " ";
						it = hierarchy.find(it->second);
						if (it->first == v)
							break;
					}
					cout << endl;
					return true;
				}
			}
		}

		if (visited.end() == visited.find(v)) {
			/*
			 * Not visited yet; mark as visited.
			 */
			visited.insert(v);

			/*
			 * Dive deeper.
			 */
			const vertex &V = get_vertex(v);
			typename vector<T>::const_iterator it;
			for (it = V.adjacent.begin(); it != V.adjacent.end(); ++it) {
				if (is_cyclic(hierarchy, visited, *it, v))
					return true;
			}
		}

		/* Backtrack */
		hierarchy.erase(v);
		return false;
	}

public:
	enum class traversal_order
	{
		depth_first,
		breadth_first
	};

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
	 * Is there a cycle in the graph?
	 * @param parent parent vertex if the first vertex usually -1 for T = int.
	 */
	bool is_cyclic(T parent)
	{
		map<T, T> hierarchy;
		set<T> visited;

		typename vector<vertex>::const_iterator it;
		for (it = vertices.begin(); it != vertices.end(); ++it) {
			if (is_cyclic(hierarchy, visited, it->vrtx, parent))
				return true;
		}

		return false;
	}

	/*
	 * Is the graph a directed acyclic graph?
	 * @param parent parent vertex if the first vertex usually -1 for T = int.
	 */
	bool is_dag(T parent)
	{
		if (!directed)
			return false;
		return !is_cyclic(parent);
	}

	/*
	 * Serialize a graph.
	 */
	void serialize(ostream &os)
	{
		set<T> visited;

		cout << (directed ? "1" : "0") << endl;
		
		typename vector<vertex>::const_iterator it;
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

static int
usage(const char *progname)
{
	cerr << progname << " -in <file> " << endl
		<< "    [-dump]                                 Dump the graph read from file." << endl
		<< "    [-degree -v <vertex>]                   Degree of the vertex." << endl
		<< "    [-path_exists -v <vertex1, vertex2>]    Path exists between vertex1 and vertex2." << endl
		<< "    [-get_paths -v <vertex1, vertex2>]      Path between vertex1 and vertex2." << endl
		<< "    [-serialize]                            Serialize the graph." << endl
		<< "    [-dfs]                                  Depth first search traversal." << endl
		<< "    [-bfs]                                  Breadth first search traversal." << endl
		<< "    [-is_cyclic]                            Is there a cycle in the graph?" << endl
		<< "    [-is_dag]                               Is the graph directed acyclic graph?" << endl;
	return 1;
}

enum operation
{
	NONE,
	DUMP,
	DEGREE,
	PATH_EXISTS,
	GET_PATHS,
	SERIALIZE,
	DFS,
	BFS,
	IS_CYCLIC,
	IS_DAG
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
		} else if (strcmp(argv[i], "-degree") == 0) {
			op = DEGREE;
		} else if (strcmp(argv[i], "-path_exists") == 0) {
			op = PATH_EXISTS;
		} else if (strcmp(argv[i], "-get_paths") == 0) {
			op = GET_PATHS;
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

	switch (op) {
		case DUMP:
			g.dump();
			break;

		case DEGREE:
			cout << g.degree(v1) << endl;
			break;

		case PATH_EXISTS:
			cout << boolalpha << g.path_exists(v1, v2) << endl;
			break;

		case GET_PATHS:
			g.get_paths(v1, v2);
			break;

		case SERIALIZE:
			g.serialize(cout);
			break;

		case DFS:
			g.traverse(graph<int>::traversal_order::depth_first, visitor);
			break;

		case BFS:
			g.traverse(graph<int>::traversal_order::breadth_first, visitor);
			break;

		case IS_CYCLIC:
			cout << boolalpha << g.is_cyclic(-1) << endl;
			break;

		case IS_DAG:
			cout << boolalpha << g.is_dag(-1) << endl;
			break;
			
		default:
			break;
	}

	return 0;
}
