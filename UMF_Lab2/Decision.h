#pragma once
#include <vector>
#include <iostream>

using namespace std;

class Decision
{
	double eps = 1E-10;

	vector<vector<double>> A;
	vector<double> f;
	int Ahead();
	int Back();

public:
	int LU_f();
	void Change_f(vector<double>& _f);
	Decision(vector<vector<double>>& _A, vector<double>& _f);
	int Answer(vector<double>& q);
};

