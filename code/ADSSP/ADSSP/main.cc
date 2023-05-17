#include<iostream>
#include<fstream>
#include<time.h>
#include<ctime>
#include<numeric>

#include"adssp.h"
#include"adssp_utils.h"

using namespace std;

typedef vector<vector<long>> mat_l;
typedef vector<vector<double>> mat_d;
typedef vector<double> vec_d;
typedef vector<long> vec_l;
typedef tuple<mat_l, vec_l, vec_l, vec_d, vec_d> readreturn;
typedef tuple<vec_l, vec_l, vec_l, vec_l, vec_d, vec_d> readflatreturn;

/**
* This function is used to read the txt files and incoming the data ADSSP needed. 
*/
readflatreturn ReadFlatFile(string path, int id) {
	mat_d edta, ndta;
	vec_l tail, head, bvec, cap;
	vec_d ucost, fcost;

	string filename_edta = path + "edta_" + to_string(id + 1) + ".txt";
	string filename_ndta = path + "ndta_" + to_string(id + 1) + ".txt";
	readMatFromFile(filename_edta, edta);
	for (size_t i = 0; i < edta[0].size(); i++) {
		tail.push_back((long)edta[0][i]);
		head.push_back((long)edta[1][i]);
		cap.push_back((long)edta[2][i]);
		fcost.push_back(edta[3][i]);
		ucost.push_back(edta[4][i]);
	}
	readMatFromFile(filename_ndta, ndta);
	for (size_t i = 0; i < ndta[0].size(); i++) {
		bvec.push_back((long)ndta[0][i]);
	}
	return make_tuple(tail, head, bvec, cap, fcost, ucost);
}

int main() {
	int NodeNum = 6000;
	int EdgeNum = 250000;
	size_t M = 1; // the number of samples
	string path = "../../../data/Large-scale instances/scale_" + to_string(NodeNum) + "_" + to_string(EdgeNum) + "/";
	string saving_path = "../../result.txt";

	vector<double> TCost, solvingtime;
	vector<double> dssp_cost, dssp_time;
	cout << "this type of samples have " << NodeNum << " Nodes and " << EdgeNum << " Edges." << endl;

	for (size_t i = 0; i < M; i++) {
		readflatreturn dat = ReadFlatFile(path, i);
		vec_l Tail = get<0>(dat), Head = get<1>(dat), bvec = get<2>(dat), cap = get<3>(dat);
		vec_d fcost = get<4>(dat), ucost = get<5>(dat);
		// Test for bvec is balanced or not
		double bvec_sum = std::accumulate(bvec.begin(), bvec.end(), 0.0);
		if (bvec_sum != 0.0)
		{
			std::cout << "the sum of supply is unbalanced, it is " << bvec_sum << std::endl;
		}

		Graph graph;
		for (size_t i = 0; i < NodeNum; i++) {
			graph.AddNode(i, bvec[i]);
		}

		for (size_t i = 0; i < EdgeNum; i++) {
			graph.AddEdge(Tail[i], Head[i], ucost[i], fcost[i], cap[i]);
		}
		vector<double> cal_uc, cal_fc;
		for (const auto& it : graph.Edges)
		{
			cal_uc.push_back(it.second.unitcost);
			cal_fc.push_back(it.second.fixedcost);
		}

		// DSSP Part
		/* ################################################################################################################# */
		time_t dssp_start, dssp_end;
		Graph dssp_graph(graph);
		DSSP dssp_mdl(dssp_graph, "max_flow", 100);
		dssp_start = clock();
		dssp_ret dssp_res = dssp_mdl.Solve(dssp_graph);
		dssp_end = clock();
		dssp_time.push_back((dssp_end - dssp_start) / (double)CLOCKS_PER_SEC); // 时间单位：秒
		vector<double> xd;
		for (const auto& it : graph.Edges)
		{
			xd.push_back(dssp_graph.Edges[it.first].optflow);
		}
		tuple<double, double, double> dssp_cost_all = FCNFCost(cal_uc, cal_fc, xd);
		dssp_cost.push_back(get<0>(dssp_cost_all));
		std::cout << "****The optimal total cost of DSSP " << i + 1 << " is: " << get<0>(dssp_cost_all) << ", and the solving time is: " << dssp_time[i] << "s." << endl;
		/* ################################################################################################################# */

		// ADSSP Part
		/* ################################################################################################################# */
		time_t start, end;
		ADSSP mdl("max_unit", 0.5, 0.01 * EdgeNum, "max_flow", 1e4, 4);
		Graph optgraph(graph), save_graph(graph);
		Graph adssp_graph(graph);
		start = clock();
		optgraph = mdl.Solve(adssp_graph);
		end = clock();
		solvingtime.push_back((end - start) / (double)CLOCKS_PER_SEC);
		for (auto& it : save_graph.Edges) { it.second.optflow = 0; }
		for (auto& it : optgraph.Edges)
		{
			save_graph.Edges[it.first].unitcost = it.second.unitcost;
			save_graph.Edges[it.first].fixedcost = it.second.fixedcost;
			save_graph.Edges[it.first].optflow = it.second.optflow;
		}
		vector<double> xa;
		for (const auto& it : graph.Edges)
		{
			xa.push_back(save_graph.Edges[it.first].optflow);
		}
		tuple<double, double, double> res_cost_ng = FCNFCost(cal_uc, cal_fc, xa);
		double totalcost_ng = get<0>(res_cost_ng);
		std::cout << "**The optimal total cost of " << i + 1 << " is: " << totalcost_ng << ", the the solving time is: " << solvingtime[i] << "s." << endl;
		TCost.push_back(totalcost_ng);
		/* ################################################################################################################# */
	}

	// output results
	std::ofstream outfile;
	outfile.open(saving_path, std::ios::out | std::ios::trunc);
	// Write the title line
	outfile << "item" << ","
		<< "adssp_time" << ","
		<< "adssp_cost"  << ","
		<< "dssp_time" << ","
		<< "dssp_cost" << std::endl;
	// loop writing data
	for (size_t i = 0; i < dssp_cost.size(); i++) {
		outfile << to_string(i) << ","
			<< to_string(solvingtime[i]) << ","
			<< to_string(TCost[i]) << ","
			<< to_string(dssp_time[i]) << ","
			<< to_string(dssp_cost[i]) << std::endl;
	}
	outfile.close();
	return 0;
}