#pragma once
#include <vector>
#include <iostream>
//g
using namespace std;

class Decision
{
	double eps = 1E-10;
	// ������� �������� � ��������� �������

	
	int AheadGauss();
	int BackGauss();

public:
	int LU_f();
	vector<vector<double>> A;
	vector<double> f;
	void Change_f(vector<double>& _f);
	Decision(vector<vector<double>>& _A, vector<double>& _f);
	int SLAU(vector<double>& q);
};

