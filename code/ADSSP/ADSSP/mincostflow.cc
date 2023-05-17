#include"mincostflow.h"
#include<iostream>

using std::cout;
using std::endl;

int MinCostFlow::Solve(Graph& graph) {
	operations_research::StarGraph stargraph(supplies.size(), arcs.size());
	operations_research::MinCostFlow mcf(&stargraph);
	for (const auto& it : arcs) {
		operations_research::ArcIndex arc = stargraph.AddArc(it.nodes.first, it.nodes.second);
		mcf.SetArcCapacity(arc, it.capacity);
		mcf.SetArcUnitCost(arc, it.unitcost);
	}
	operations_research::FlowQuantity supp = 0, dema = 0;
	for (const auto& it : supplies) {
		mcf.SetNodeSupply(it.first, it.second);
	}
	mcf.Solve();
	if (operations_research::MinCostFlow::OPTIMAL != mcf.status())
	{
		if (operations_research::MinCostFlow::UNBALANCED == mcf.status()) { cout << "***ERROR: test equal to UNBANLANCED***" << endl; }
		if (operations_research::MinCostFlow::BAD_COST_RANGE == mcf.status()) { cout << "***ERROR: Test equal to BAD_COST_RANGE***" << endl; }
		if (operations_research::MinCostFlow::BAD_RESULT == mcf.status()) { cout << "***ERROR: Test equal to BAD_RESULT***" << endl; }
		if (operations_research::MinCostFlow::FEASIBLE == mcf.status()) { cout << "***ERROR: FEASIBLE***" << endl; }
		if (operations_research::MinCostFlow::INFEASIBLE == mcf.status()) { cout << "***ERROR: INFEASIBLE***" << endl; }
		if (operations_research::MinCostFlow::NOT_SOLVED == mcf.status()) { cout << "***ERROR: NOT_SOLVED***" << endl; }
		cout << "***ERROR: There is no solution!***" << endl;
		return 0;
	}
	for (size_t i = 0; i < arcs.size(); i++)
	{
		graph.Edges[make_pair(stargraph.Tail(i), stargraph.Head(i))].optflow = mcf.Flow(i);
	}
	return 1;
}

MinCostFlow::MinCostFlow(Graph& graph) {
	/**
	  * @brif transfer the type of input data.
	  */
	for (auto iter = graph.Edges.begin(); iter != graph.Edges.end(); iter++)
	{
		arcs.push_back(Arc() = { make_pair(int64_t(iter->first.first), int64_t(iter->first.second)), iter->second.capacity, int64_t(iter->second.edgecost) });
	}
	for (auto iter = graph.Nodes.begin(); iter != graph.Nodes.end(); iter++)
	{
		supplies.push_back(make_pair(iter->first, iter->second.supply));
	}
}