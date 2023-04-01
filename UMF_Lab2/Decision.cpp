#include "Decision.h"


// Конструктор
Decision::Decision(vector<vector<double>>& _A, vector<double>& _f)
{
	A = _A;
	f = _f;
}

// LU-разложение
int Decision::LU_f()
{
	A[0][2] /= A[0][1];
	for (int i = 1; i < A.size(); i++)
	{
		A[i][1] -= A[i][0] * A[i - 1][2];
		if (abs(A[i][1]) < eps)
			return 1;
		else
			A[i][2] /= A[i][1];
	}
	return 0;
}

// Ly = F 
int Decision::Ahead()
{
	f[0] /= A[0][1];
	for (int i = 1; i < A.size(); i++)
		f[i] = (f[i] - A[i][0] * f[i - 1]) / A[i][1];
	return 0;
}

// Ux = y
int Decision::Back()
{
	for (int i = A.size() - 2; i >= 0; i--)
		f[i] -= A[i][2] * f[i + 1];
	return 0;
}

// Смена функции
void Decision::Change_f(vector<double>& _f)
{
	f.clear();
	f = _f;
}

// Получение ответа
int Decision::Answer(vector<double>& q)
{
	Ahead();
	Back();
	q = f;
	return 0;
}
