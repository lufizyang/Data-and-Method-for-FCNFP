#ifndef DYNAMIC_SLOPE_SCALING_PROCEDURE_H_
#define DYNAMIC_SLOPE_SCALING_PROCEDURE_H_

#include<string>
#include"mincostflow.h"
#include"adssp_utils.h"

using std::string;

typedef pair<vector<Graph>, vector<double>> dssp_ret;

class DSSP
{
public:
	/**
	* This function is the major solving process of DSSP
	*/
	dssp_ret Solve(Graph& graph);

	DSSP(Graph& graph, string cost_ini = "maxflow", int max_iter = 30);
	~DSSP() {};

private:
	int MAXITER; 
};



#endif // !DYNAMIC_SLOPE_SCALING_PROCEDURE_H_
