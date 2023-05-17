#include "adssp_utils.h"

using namespace std;


double CalculateTransCost(vector<double> unitcost, vector<int> x) {
	double s = 0.0;
	for (size_t i = 0; i < x.size(); i++) {
		s += unitcost[i] * x[i];
	}
	return s;
}

double CalculateFixedCost(vector<double> fixedcost, vector<int> x) {
	double s = 0.0;
	for (size_t i = 0; i < x.size(); i++) {
		if (x[i] > 1e-5) {
			s += fixedcost[i];
		}
	}
	return s;
}

tuple<double, double, double> CalculateCost(vector<double> unitcost, vector<double> fixedcoat, vector<int> x) {
	double Transcost = CalculateTransCost(unitcost, x), Fixedcost = CalculateFixedCost(fixedcoat, x);
	return make_tuple(Transcost + Fixedcost, Transcost, Fixedcost);
}

bool VectorEqual(vector<int> v1, vector<int> v2) {
	if (v1.size() != v2.size()) { return false; }
	for (size_t i = 0; i < v1.size(); i++) {
		if (v1[i] != v2[i]) { return false; }
	}
	return true;
}

void readMatFromFile(string filename, vector<vector<double>>& output) {
	ifstream fs(filename);
	fs.unsetf(ios::skipws);
	while (fs.good())
	{
		string line_str;
		getline(fs, line_str);
		if (line_str.size() < 2) { continue; }
		istringstream ss(line_str);
		vector<double> line_data;
		while (ss.good()) {
			float v;
			ss >> v;
			line_data.push_back(v);
		}
		output.push_back(line_data);
	}
}
