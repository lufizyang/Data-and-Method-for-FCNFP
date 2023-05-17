#include"dssp.h"
#include<algorithm>
#include<iostream>

using std::find;
using std::get;

DSSP::DSSP(Graph& graph, string cost_ini, int max_iter)
{
	MAXITER = max_iter;
	if (cost_ini == "trans")
	{
		for (auto iter = graph.Edges.begin(); iter != graph.Edges.end(); iter++) { iter->second.edgecost = iter->second.unitcost; }
	}
	else if (cost_ini == "unitflow")
	{
		for (auto iter = graph.Edges.begin(); iter != graph.Edges.end(); iter++) { iter->second.edgecost = iter->second.unitcost + iter->second.fixedcost; }
	}
	else // cost_ini = "max_flow"
	{
		for (auto iter = graph.Edges.begin(); iter != graph.Edges.end(); iter++) { iter->second.edgecost = iter->second.unitcost + int(iter->second.fixedcost / iter->second.capacity); }
	}
}

dssp_ret DSSP::Solve(Graph& graph) {
	size_t k = 0;
	vector<Graph> Res;
	vector<double> TotalCost, TransCost, FixedCost;
	map<pair<int, int>, double> MaxhisEdgeCost; // save the max edgecost in history of a edge.
	vector<int> LastFlows;
	for (const auto& it : graph.Edges) {
		MaxhisEdgeCost[make_pair(it.first.first, it.first.second)] = 0.0;
	}

	// Test the Flow whether is balanced or not
	double supply_sum = 0.0;
	for (auto& it : graph.Nodes)
	{
		supply_sum += it.second.supply;
	}
	if (supply_sum != 0)
	{
		std::cout << "the supply of probem is unbalanced. It is " << supply_sum << std::endl;
	}

	// Initialize the class MCF
	while (k <= MAXITER)
	{
		k++;
		MinCostFlow mcf(graph);
		int is_solved = mcf.Solve(graph);
		if (!is_solved) { return make_pair(Res, TotalCost); }
		Res.push_back(Graph(graph));
		tuple<double, double, double> cost_k = FCNFCost(graph, 1e4);
		double totalcost = get<0>(cost_k), unitcost = get<1>(cost_k), fixedcost = get<2>(cost_k);

		if (find(TotalCost.begin(), TotalCost.end(), totalcost) != TotalCost.end())
		{
			auto index_ = find(TotalCost.begin(), TotalCost.end(), totalcost);
			if ((*(TransCost.begin() + (index_ - TotalCost.begin())) == unitcost) && (*(FixedCost.begin() + (index_ - TotalCost.begin())) == fixedcost))
			{
				TotalCost.push_back(totalcost);
				TransCost.push_back(unitcost);
				FixedCost.push_back(fixedcost);
				break;
			}
		}
		TotalCost.push_back(totalcost);
		TransCost.push_back(unitcost);
		FixedCost.push_back(fixedcost);
		vector<int> NowFlows;
		for (const auto& it : graph.Edges) {
			NowFlows.push_back(it.second.optflow);
		}
		if ((Res.size() > 1) && (VectorEqual(NowFlows, LastFlows)))	{ break; } else { LastFlows = NowFlows; }
		for (auto& it : graph.Edges)
		{
			if (it.second.optflow > 1e-5)
			{
				it.second.edgecost = it.second.unitcost + it.second.fixedcost / it.second.optflow;
				if (MaxhisEdgeCost[make_pair(it.second.tail, it.second.head)] < it.second.edgecost)
				{
					MaxhisEdgeCost[make_pair(it.second.tail, it.second.head)] = it.second.edgecost;
				}
			}
			else
			{
				if (MaxhisEdgeCost[make_pair(it.second.tail, it.second.head)] > 1e-5) {
					it.second.edgecost = MaxhisEdgeCost[make_pair(it.second.tail, it.second.head)];
				}
			}
		}
	}
	return make_pair(Res, TotalCost);
}


