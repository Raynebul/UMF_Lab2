#include "Decision.h"

//g
// Конструктор
Decision::Decision(vector<vector<double>>& _A, vector<double>& _f)
{
	A = _A;
	f = _f;
}

// LU-разложение
int Decision::LU_f()
{
	A[0][2] /= A[0][1]; // au/di
	for (int i = 1; i < A.size(); i++)
	{
		A[i][1] -= A[i][0] * A[i - 1][2]; //di - al*au
		if (abs(A[i][1]) < eps)
			return 1;
		else
			A[i][2] /= A[i][1]; //au/di
	}
	return 0;
}

// Ly = F 
int Decision::AheadGauss()
{
	f[0] /= A[0][1]; //f[0] / di[0] 
	for (int i = 1; i < A.size(); i++)
		f[i] = (f[i] - A[i][0] * f[i - 1]) / A[i][1]; //(f - al*f)/di
	return 0;
}

// Ux = y
int Decision::BackGauss()
{
	for (int i = A.size() - 2; i >= 0; i--)
		f[i] -= A[i][2] * f[i + 1]; //f - au*f
	return 0;
}

// Смена функции
void Decision::Change_f(vector<double>& _f)
{
	f.clear();
	f = _f;
}

// Получение ответа
int Decision::SLAU(vector<double>& q)
{
	AheadGauss();
	BackGauss();
	q = f;
	return 0;
}
