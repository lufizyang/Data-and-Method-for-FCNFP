#ifndef GRAPH_DEFINE_H_
#define GRAPH_DEFINE_H_

#include<vector>
#include<map>
#include<tuple>
#include<string>
#include<utility>
#include<limits>

using std::vector;
using std::map;
using std::pair;
using std::make_pair;
using std::tuple;
using std::make_tuple;


struct GraphNode
{
	int name;
	int supply;
};
using Node = struct GraphNode;

struct GraphEdge
{
	int tail;
	int head;
	int capacity;
	int optflow;
	double unitcost, fixedcost, edgecost;
};
using Edge = struct GraphEdge;

class Graph
{
public:
	map<int, Node> Nodes;
	map<pair<int, int>, Edge> Edges;
	void AddNode(int name, int supply);
	void AddEdge(int tail, int head, double unitcost, double fixedcost, int capacity = -1, double edgecost = -1.0);

	Graph() {}
	Graph(const Graph& old);
	~Graph() {}

	Graph& operator=(const Graph& right);

private:
	const int INTINF = std::numeric_limits<int>::infinity();
	const float FLOATINF = std::numeric_limits<float>::infinity();
};

tuple<double, double, double> FCNFCost(Graph graph, int MULTI = 1);

tuple<double, double, double> FCNFCost(vector<double> ucost, vector<double> fcost, vector<double> x);



#endif // !GRAPH_DEFINE_H_
