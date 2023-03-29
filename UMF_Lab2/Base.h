#pragma once
#include <stdio.h>
#include <fstream>
#include <vector>
#include <string>
#include "Decision.h"

using namespace std;

class f_elem_method
{
	vector<double> grid;
	vector<vector<double>> A;
	vector<double> f;

	int MAXITER = 10000;
	double f1, fn; // bounder
	double EPS = 1E-10;
	double w = 0.6;

	double lambda(double x);
	double gamma(double x);
	double fun(double x, double u);
	double d_fun(double x, double x1, double x2, double q1, double q2, int num);

	int Loc_Matrix(vector<vector<double>>& A_loc, vector<double>& b_loc, int id);
	int G_Loc_Matrix(double h, double lamdda1, double lamdda2, vector<vector<double>>& G_loc);
	int M_Loc_Matrix(double h, double gamma, vector<vector<double>>& M_loc);
	int b_Loc(double h, double f1, double f2, vector<double>& b_loc);

	int End_Matrix();
	int Get_f();
	int Create_grid(string file);
	double Get_Disparity();

public:

	vector<double> q0, q1;
	int Decision_task(string file);
};
