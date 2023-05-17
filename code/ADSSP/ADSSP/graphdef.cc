#include "graphdef.h"

Graph::Graph(const Graph& old)
{
	for (const auto& it : old.Nodes)
	{
		Nodes[it.first] = it.second;
	}
	for (const auto& it : old.Edges)
	{
		Edges[it.first] = it.second;
	}
}

void Graph::AddNode(int name, int supply = -1)
{
	//if (supply == -1)
	//{
	//	supply = INTINF;
	//}
	Nodes[name] = { name, supply };
}

void Graph::AddEdge(int tail, int head, double unitcost, double fixedcost, int capacity, double edgecost) {
	if (capacity == -1)
	{
		capacity = INTINF;
	}
	if (edgecost < 1e-5)
	{
		edgecost = 0.0;
	}
	Edges[make_pair(tail, head)] = { tail, head, capacity, 0, unitcost, fixedcost, edgecost };
}

Graph& Graph::operator=(const Graph& right) {
	for (const auto& it : right.Nodes)
	{
		Nodes[it.first] = it.second;
	}
	for (const auto& it : right.Edges)
	{
		Edges[it.first] = it.second;
	}
	return *this;
}

tuple<double, double, double> FCNFCost(Graph graph, int MULTI) {
	double fc = 0.0, dc = 0.0;
	for (const auto& it : graph.Edges) {
		fc += it.second.unitcost * it.second.optflow / MULTI;
		if (it.second.optflow > 0) {
			dc += it.second.fixedcost / MULTI;
		}
	}
	return make_tuple(dc + fc, fc, dc);
}

tuple<double, double, double> FCNFCost(vector<double> ucost, vector<double> fcost, vector<double> x) {
	double fc = 0.0, dc = 0.0;
	for (size_t i = 0; i < x.size(); i++)
	{
		fc += ucost[i] * x[i];
		if (x[i] > 1e-5) {
			dc += fcost[i];
		}
	}
	return make_tuple(dc + fc, fc, dc);
}
