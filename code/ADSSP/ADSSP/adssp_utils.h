#ifndef ADSSP_UTILS_H_
#define ADSSP_UTILS_H_

#include<vector>
#include<tuple>
#include<string>
#include<iostream>
#include<fstream>
#include<sstream>

using std::vector;
using std::tuple;
using std::string;

/**
* This function is used to calculate the cost on total flow.
* Input:
*  - unitcost: a vector contained the unit cost of all edges, where the index corresponds to the edge
*  - x: a vector contained the flow on all edges
* Output:
*  - s: the total cost on transforming flows on graph
*/
double CalculateTransCost(vector<double> unitcost, vector<int> x);

/**
* This function is used to calculate the fixed cost on total flow.
* Input:
*  - fixedcost: a vector contained the fixed cost of all edges
*  - x: a vector contained the flow on all edges
* Output:
*  - s: the total fixed cost
*/
double CalculateFixedCost(vector<double> fixedcost, vector<int> x);

/**
* This function is used to calculate the total cost, including unit cost and fixed cost.
* Input:
*  - unitcost: a vector contained the unit cost of all edges
*  - fixedcost: a vector contained the fixed cost of all edges
*  - x: a vector contained the flow on all edges
* Output:
*  - a tuple with three items: total cost, flow cost, and fixed cost
*/
tuple<double, double, double> CalculateCost(vector<double> unitcost, vector<double> fixedcoat, vector<int> x);

/**
* This function is used to judge the equivalence between two vector.
* Input:
*  - v1, v2: two vectors with int-type items
* Output:
*  - a bool variable
*/
bool VectorEqual(vector<int> v1, vector<int> v2);

/**
* This function is used to read the instances in txt files.
* Input:
*  - filename: the name of file
*  - &output: 2-Dim vector to save data
*/
void readMatFromFile(string filename, vector<vector<double>>& output);



#endif // !ADSSP_UTILS_H_
