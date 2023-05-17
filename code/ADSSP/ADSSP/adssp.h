#ifndef ADAPTIVE_DYNAMIC_SLOPE_SCALING_PROCEDURE_H_
#define ADAPTIVE_DYNAMIC_SLOPE_SCALING_PROCEDURE_H_

#include"dssp.h"

class ADSSP
{
public:
	/**
	* This function is the major solving process of ADSSP.
	*/
	Graph Solve(Graph& graph);

	/**
	* Initialization of ADSP
	* Input:
	*  - del_rule: the keyword to assign the deleting rule in ADSSP. the instructs contains "random", "max_fixed", "max_capa", and "max_unit". 
	*  - del_ratio: the deleting ratio of selected edges in once deleting operation. 
	*  - min_del_num: the minimum number to stop deleting, which means when the number of deletable edges is less than this number, the current operation will delete all deletable edges. 
	*  - dssp_ini_rule: the rule of constructing the initial edge cost in DSSP. The origin method offers several, but in my paper, I only use the rule "max_flow". 
	*  - _multi: A multiplier that amplifies non integer costs, since the MCF solver in or-tools can only handle the problem with integer inputs. 
	*  - dssp_max_iter: The maximum number of iterations in DSSP. 
	*/
	ADSSP(string del_rule = "max_unit", float del_ratio = 0.5, int min_del_num = 3, string dssp_ini_rule = "multi_unit", int _multi = 1e2, int dssp_max_iter = 3);
	~ADSSP() {}

private:
	float DELRATIO;
	string DELRULE, DSSP_ITER_EDGECOST_INITIALIZATION;
	int MINDELNUM, MULTI, DSSP_MAX_ITER;
};

#endif // !ADAPTIVE_DYNAMIC_SLOPE_SCALING_PROCEDURE_H_
