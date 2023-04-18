#pragma once
#include <stdio.h>
#include <fstream>
#include <vector>
#include <string>
#include "Decision.h"

using namespace std;

class f_elem_method
{
	// сеткав
	vector<double> grid;
	// Матрица
	vector<vector<double>> A;
	// вектор
	vector<double> f;

	int MAXITER = 10000;
	double f1, fn; // bounder
	double EPS = 1E-15;
	double w = 1;

	double lambda(double x);
	double gamma(double x);
	// наша функция
	double fun(double x, double u);
	// производная функции
	double d_fun(double x, double x1, double x2, double q1, double q2, int num);

	// Локальная матрица
	int Loc_Matrix(vector<vector<double>>& A_loc, vector<double>& b_loc, int id);
	// Матрица жёсткости
	int G_Loc_Matrix(double h, double lamdda1, double lamdda2, vector<vector<double>>& G_loc);
	// Матрица масс
	int M_Loc_Matrix(double h, double gamma, vector<vector<double>>& M_loc);
	// вектор правой части
	int b_Loc(double h, double f1, double f2, vector<double>& b_loc);

	// Сборка глобальной матрицы
	int End_Matrix();

	// 
	int Get_f();
	// Создание сетки
	int Create_grid(string file);
	double Get_Disparity();

public:

	vector<double> q0, q1;
	// Решение
	int Decision_task(string file);
};
