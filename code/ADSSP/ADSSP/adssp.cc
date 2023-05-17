#include"adssp.h"

#include<iostream>
#include<fstream>
#include<algorithm>

#include<random>
#include <chrono>

using std::min_element;
using std::shuffle;
using std::ifstream;
using std::ios;
using std::istringstream;

inline void file_to_string(vector<string>& record, const string& line, char delimiter);
inline float string_to_float(string str);

inline void file_to_string(vector<float>& record, const string& line, char delimiter)
{
	int linepos = 0;
	char c;
	int linemax = line.length();
	string curstring;
	record.clear();
	while (linepos < linemax)
	{
		c = line[linepos];
		if (isdigit(c) || c == '.') {
			curstring += c;
		}
		else if (c == delimiter && curstring.size()) {
			record.push_back(string_to_float(curstring));
			curstring = "";
		}
		++linepos;
	}
	if (curstring.size())
		record.push_back(string_to_float(curstring));
	return;
}

inline float string_to_float(string str) {
	int i = 0, len = str.length();
	float sum = 0;
	while (i < len) {
		if (str[i] == '.') break;
		sum = sum * 10 + str[i] - '0';
		++i;
	}
	++i;
	float t = 1, d = 1;
	while (i < len) {
		d *= 0.1;
		t = str[i] - '0';
		sum += t * d;
		++i;
	}
	return sum;
}


ADSSP::ADSSP(string del_rule, float del_ratio, int min_del_num, string dssp_ini_rule, int _multi, int dssp_max_iter)
{
	DELRATIO = del_ratio;
	DELRULE = del_rule;
	MINDELNUM = min_del_num;
	DSSP_ITER_EDGECOST_INITIALIZATION = dssp_ini_rule;
	MULTI = _multi;
	DSSP_MAX_ITER = dssp_max_iter;
}

Graph ADSSP::Solve(Graph& graph) {
	vector<Graph> Res_list;
	vector<double> TCosts;
	Graph OptGraph;
	double OptCost = 1e12;
	size_t k = 0;
	const int ORI_EDGE_NUM = graph.Edges.size();
	for (auto& it : graph.Edges)
	{
		it.second.fixedcost = (int)(it.second.fixedcost * MULTI);
		it.second.unitcost = (int)(it.second.unitcost * MULTI);
	}
	double _multi = 1;

	while (true)
	{
		DSSP mdl_dssp(graph, DSSP_ITER_EDGECOST_INITIALIZATION, DSSP_MAX_ITER);
		dssp_ret res_temp = mdl_dssp.Solve(graph);
		Res_list.clear();
		TCosts.clear();
		Res_list = res_temp.first;
		TCosts = res_temp.second;
		if (Res_list.size() == 0)
		{
			std::cout << "the result of DSSP is empty!" << std::endl;
		}
		auto minposition = min_element(TCosts.begin(), TCosts.end());
		Graph optgraph = *(Res_list.begin() + (minposition - TCosts.begin()));
		if (OptCost > *minposition)
		{
			OptGraph = Graph(optgraph);
		}

		k++;
		graph = Graph(optgraph);
		vector<pair<int, int>> del_fixededge_list, del_list;
		for (const auto& it : graph.Edges) {
			if ((it.second.fixedcost > 0) && (it.second.optflow < 1e-5))
			{
				del_fixededge_list.push_back(it.first);
			}
		}
		int del_fixededge_num = del_fixededge_list.size();
		if (del_fixededge_num < 1) { break; }
		int del_num;
		if (del_fixededge_num > MINDELNUM)
		{
			del_num = std::max((int)std::round(del_fixededge_num * DELRATIO), MINDELNUM);
		}
		else
		{
			del_num = del_fixededge_num;
		}
		if (DELRULE == "whole")
		{
			del_list = del_fixededge_list;
		}
		else if (DELRULE == "random")
		{
			unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
			shuffle(del_fixededge_list.begin(), del_fixededge_list.end(), std::default_random_engine(seed));
			del_list.assign(del_fixededge_list.begin(), del_fixededge_list.begin() + del_num);
		}
		else if (DELRULE == "max_fixed")
		{
			vector<pair<pair<int, int>, double>> temp_;
			for (size_t i = 0; i < del_fixededge_list.size(); i++) {
				temp_.push_back(make_pair(del_fixededge_list[i], graph.Edges[del_fixededge_list[i]].fixedcost));
			}
			using iid = pair<pair<int, int>, double>;
			std::sort(temp_.begin(), temp_.end(), [](iid left, iid right)->bool {return left.second > right.second; });
			for (size_t i = 0; i < del_num; i++) {
				del_list.push_back(temp_[i].first);
			}
		}
		else if (DELRULE == "max_capa")
		{
			vector<pair<pair<int, int>, double>> temp_;
			for (size_t i = 0; i < del_fixededge_list.size(); i++) {
				temp_.push_back(make_pair(del_fixededge_list[i], graph.Edges[del_fixededge_list[i]].fixedcost / graph.Edges[del_fixededge_list[i]].capacity + graph.Edges[del_fixededge_list[i]].unitcost));
			}
			using iid = pair<pair<int, int>, double>;
			std::sort(temp_.begin(), temp_.end(), [](iid left, iid right)->bool {return left.second > right.second; });
			for (size_t i = 0; i < del_num; i++) {
				del_list.push_back(temp_[i].first);
			}
		}
		else if (DELRULE == "max_unit")
		{
			vector<pair<pair<int, int>, double>> temp_;
			for (size_t i = 0; i < del_fixededge_list.size(); i++) {
				temp_.push_back(make_pair(del_fixededge_list[i], graph.Edges[del_fixededge_list[i]].fixedcost + graph.Edges[del_fixededge_list[i]].unitcost));
			}
			using iid = pair<pair<int, int>, double>;
			std::sort(temp_.begin(), temp_.end(), [](iid left, iid right)->bool {return left.second > right.second; });
			for (size_t i = 0; i < del_num; i++) {
				del_list.push_back(temp_[i].first);
			}
		}
		if (DSSP_ITER_EDGECOST_INITIALIZATION == "multi_unit")
		{
			_multi = (del_fixededge_num - del_num) / ORI_EDGE_NUM;
		}
		std::cout << "the deleting edge number is: " << del_list.size() << std::endl;
		for (auto& it : del_list) {
			graph.Edges.erase(it);
		}
	}

	for (auto& it : OptGraph.Edges)
	{
		it.second.fixedcost = it.second.fixedcost / MULTI;
		it.second.unitcost = it.second.unitcost / MULTI;
	}
	return OptGraph;
}