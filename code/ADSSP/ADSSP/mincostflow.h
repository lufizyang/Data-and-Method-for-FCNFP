#ifndef KKK_MIN_COST_FLOW_H_
#define KKK_MIN_COST_FLOW_H_

#include<vector>
#include<map>

#include"graphdef.h"
#include<ortools/graph/min_cost_flow.h>

struct Arc
{
	pair<operations_research::NodeIndex, operations_research::NodeIndex> nodes;
	operations_research::FlowQuantity capacity;
	operations_research::FlowQuantity unitcost;
};

class MinCostFlow
{
public:
	vector<Arc> arcs;
	vector<pair<operations_research::NodeIndex, operations_research::FlowQuantity>> supplies;

	/**
	* This function is used to solve the minimum-cost flow model by the solver in or-tools. I write the code about the transformation process in this function.  
	*/
	int Solve(Graph& graph);
	MinCostFlow(Graph& graph);
	~MinCostFlow() {}

private:

};


#endif // KKK_MIN_COST_FLOW_H_
