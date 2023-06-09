#include "Base.h"

double f_elem_method::fun(double x, double u)
{
	// !!!��������� ���!!! ��� ������������� �����
	// ����������
	return -12*x*x+u;
	//return -2 * (x * x / u) + u;
	//return -12*x*x + u;                               // U = const
	//return u;                             // U = x
}

// �����������
double f_elem_method::d_fun(double x, double x1, double x2, double q1, double q2, int num)
{
	double h_x = x2 - x1;
	double psi_1 = (x2 - x) / h_x;
	double psi_2 = (x - x1) / h_x;
	double res = 0;

	switch (num)
	{
	case 1:// ����������� �� ������ ����������
		//��������� �����������
		//res = psi_1;
		res = 0;
		//res = psi_1; // f = u
		break;

	case 2: //����������� �� 2 ����������
		res = 0;
		//res = psi_2;
		//res = psi_2; // f = u
		break;

	default:
		break;
	}
	return res;
}

// �����
double f_elem_method::gamma(double x)
{
	return 1;
}

// ������
double f_elem_method::lambda(double x)
{
	return 1;
}

// �������� �����
int f_elem_method::Create_grid(string filename)
{
	ifstream in(filename);
	double X, k_x;
	int N_x;
	int count_x;
	in >> count_x;
	grid.resize(count_x);

	in >> grid[0];

	// ����� ������ �� x, ��� ��� ����������
	for (int curr_count_x = 0; curr_count_x < count_x - 1; )
	{
		in >> X >> N_x >> k_x;
		double h_x;
		k_x = sqrt(sqrt(k_x));
		// ���� ����������� �����
		if (k_x == 1)
		{
			h_x = (X - grid[curr_count_x]) / N_x;
			for (int p = 1; p < N_x; p++)
				grid[curr_count_x + p] = grid[curr_count_x] + h_x * p;
			curr_count_x += N_x;
		}
		// ���� ������������� �����
		else
		{
			h_x = (X - grid[curr_count_x]) * (k_x - 1) / (pow(k_x, N_x) - 1);
			double pow_kx = 1;
			for (int p = 0; p < N_x - 1; curr_count_x++, p++)
			{
				grid[curr_count_x + 1] = grid[curr_count_x] + h_x * pow_kx;
				pow_kx *= k_x;
			}
			curr_count_x++;
		}
		grid[curr_count_x] = X;
		cout << "Step " << h_x << endl;
	}

	//cout.imbue(locale("Russian"));
	for (int i = 0; i < grid.size(); i++)
		cout << grid[i] << endl;

	// ���������
	f.resize(count_x);
	in >> f1 >> fn; // 1

	// q0
	q0.resize(count_x);
	for (int i = 0; i < count_x; i++)
		in >> q0[i];
	return 0;
}

// ������� ����
int f_elem_method::M_Loc_Matrix(double h, double gamma, vector<vector<double>>& M_loc)
{
	double k = gamma * h / 6;
	M_loc[0][0] += k * 2;
	M_loc[0][1] += k;
	M_loc[1][0] += k;
	M_loc[1][1] += k * 2;
	return 0;
}

// ������� ��������
int f_elem_method::G_Loc_Matrix(double h, double lamdda1, double lamdda2, vector<vector<double>>& G_loc)
{
	double k = (lamdda1 + lamdda2) / (2 * h);
	G_loc[0][0] = k;
	G_loc[0][1] = -k;
	G_loc[1][0] = -k;
	G_loc[1][1] = k;
	return 0;
}

// ��������� ������
int f_elem_method::b_Loc(double h, double f1, double f2, vector<double>& b_loc)
{
	double k = h / 6;
	b_loc[0] = k * (2 * f1 + f2);
	b_loc[1] = k * (f1 + 2 * f2);
	return 0;
}

// ���������� ��������� �������
int f_elem_method::Loc_Matrix(vector<vector<double>>& A_loc, vector<double>& b_loc, int id)
{
	double h = grid[id + 1] - grid[id];
	double lambda1 = lambda(grid[id]);
	double lambda2 = lambda(grid[id + 1]);
	double gamma1 = gamma(grid[id]);
	double f1 = fun(grid[id], q0[id]);
	double f2 = fun(grid[id + 1], q0[id + 1]);

	G_Loc_Matrix(h, lambda1, lambda2, A_loc);
	M_Loc_Matrix(h, gamma1, A_loc);
	b_Loc(h, f1, f2, b_loc);

	return 0;
}

// ���������� �������, ����������
int f_elem_method::End_Matrix()
{
	int n = grid.size();
	// ������� A[i]
	A.clear();
	A.resize(n);
	f.clear();
	f.resize(n);

	// ��������� ������� 
	vector<vector<double>> A_loc(2);

	A_loc[0].resize(2);
	A_loc[1].resize(2);
	// ������
	vector<double> b_loc(2);
	for (int i = 0; i < n; i++)
		A[i].resize(3);

	for (int i = 0; i < n - 1; i++)
	{
		// ���������� ��������� �������
		Loc_Matrix(A_loc, b_loc, i);
		A[i][1] += A_loc[0][0];
		A[i][2] += A_loc[0][1];
		A[i + 1][0] += A_loc[1][0];
		A[i + 1][1] += A_loc[1][1];
		f[i] += b_loc[0];
		f[i + 1] += b_loc[1];
	}

	// bounder
	A[0][0] = 0;
	A[0][1] = 1;
	A[0][2] = 0;
	f[0] = f1;

	A[n - 1][0] = 0;
	A[n - 1][1] = 1;
	A[n - 1][2] = 0;
	f[n - 1] = fn;

	return 0;
}

int f_elem_method::Get_new_f()
{
	int n = grid.size();
	f.clear();
	f.resize(n);

	vector<double> b_loc(2);

	for (int i = 0; i < n - 1; i++)
	{
		double h = grid[i + 1] - grid[i],
			f1 = fun(grid[i], q0[i]),
			f2 = fun(grid[i + 1], q0[i + 1]);
		b_Loc(h, f1, f2, b_loc);

		f[i] += b_loc[0];
		f[i + 1] += b_loc[1];
	}

	// bounder
	f[0] = f1;
	f[n - 1] = fn;

	return 0;
}

int f_elem_method::Get_new_A()
{
	int n = grid.size();
	// ������� A[i]
	A.clear();
	A.resize(n);
	vector<vector<double>> A_loc(2);

	A_loc[0].resize(2);
	A_loc[1].resize(2);
	for (int i = 0; i < n; i++)
		A[i].resize(3);
	for (int i = 0; i < n - 1; i++)
	{
		// ���������� ��������� �������
		//Loc_Matrix(A_loc, b_loc, i);
		double h = grid[i + 1] - grid[i];
		double lambda1 = lambda(grid[i]);
		double lambda2 = lambda(grid[i + 1]);
		double gamma1 = gamma(grid[i]);
		//double f1 = fun(grid[id], q0[id]);
		//double f2 = fun(grid[id + 1], q0[id + 1]);

		G_Loc_Matrix(h, lambda1, lambda2, A_loc);
		M_Loc_Matrix(h, gamma1, A_loc);
		A[i][1] += A_loc[0][0];
		A[i][2] += A_loc[0][1];
		A[i + 1][0] += A_loc[1][0];
		A[i + 1][1] += A_loc[1][1];
		//f[i] += b_loc[0];
		//f[i + 1] += b_loc[1];
	}

	// bounder
	A[0][0] = 0;
	A[0][1] = 1;
	A[0][2] = 0;

	A[n - 1][0] = 0;
	A[n - 1][1] = 1;
	A[n - 1][2] = 0;

	return 0;
}

double f_elem_method::Get_Disparity()
{
	double stop = 0, temp = 0;
	int k = grid.size() - 1;
	temp = A[0][1] * q0[0];
	temp += A[0][2] * q0[1];
	stop += (temp - f[0]) * (temp - f[0]);

	for (int i = 1; i < k; i++)
	{
		temp = A[i][0] * q0[i - 1];
		temp += A[i][1] * q0[i];
		temp += A[i][2] * q0[i + 1];
		stop += (temp - f[i]) * (temp - f[i]); // ||a-f||
	}

	temp = A[k][0] * q0[k - 1];
	temp += A[k][1] * q0[k];
	stop += (temp - f[k]) * (temp - f[k]); // ||a-f||

	temp = 0;
	for (int i = 0; i < grid.size(); i++)
		temp += f[i] * f[i]; // ||f||
	stop /= temp; // eps ||a-f||/||f||
	return stop;
}

int f_elem_method::Decision_task(string file)
{
	// �������� �����
	Create_grid(file);
	int iter;
	double stop = 10;
	// ���������� �������, ����������
	End_Matrix();
	Decision t(A, f);
	if (t.LU_f())
	{
		cout << "LU-factorization failed" << endl;
		return -1;
	}
	//A = t.A;
	//q0 = f;
	//q0 = f;
	q0 = f;
	for (iter = 0; iter < MAXITER; iter++)
	{
		//A = t.A;

		stop = Get_Disparity();
		cout << iter << " " << sqrt(stop) << endl;
		if (stop > EPS*EPS)
		{
			//Decision t1(A, f);
			//End_Matrix();
			//t.LU_f();

			q1 = q0;
			t.SLAU(q0);
			//End_Matrix();
			
			for (int i = 0; i < q0.size(); i++)
				q0[i] = q1[i] + w * (q0[i] - q1[i]);
				
			Get_new_A();
			Get_new_f(); 
			t.A = A;
			//t.f = f;			
			t.LU_f();
			//A = t.A;
			t.Change_f(f);
			
		}
		else
			return iter;
	}
	return iter;
}

int f_elem_method::Loc_Matrix_Newton(vector<vector<double>>& A_loc, vector<double>& b_loc, int id)
{
	double h = grid[id + 1] - grid[id];
	double lambda_1 = lambda(grid[id]);
	double lambda_2 = lambda(grid[id + 1]);
	double gamma_1 = gamma(grid[id]);
	double f1 = fun(grid[id], q0[id]);
	double f2 = fun(grid[id + 1], q0[id + 1]);
	double f0_q0 = d_fun(grid[id], grid[id], grid[id + 1], q0[id], q0[id + 1], 1);
	double f1_q1 = d_fun(grid[id + 1], grid[id], grid[id + 1], q0[id], q0[id + 1], 2);

	G_Loc_Matrix(h, lambda_1, lambda_2, A_loc);
	M_Loc_Matrix(h, gamma_1, A_loc);

	A_loc[0][0] -= h / 6 * (2 * f0_q0); // db(0)/dq(0)
	A_loc[0][1] -= h / 6 * (f1_q1); // db(0)/dq(1)
	A_loc[1][0] -= h / 6 * (f0_q0); // db(1)/dq(0)
	A_loc[1][1] -= h / 6 * (2 * f1_q1); // db(1)/dq(1)

	b_Loc(h, f1, f2, b_loc);

	b_loc[0] -= q0[id] * (h / 6 * (2 * f0_q0)) + q0[id + 1] * (h / 6 * (f1_q1));
	b_loc[1] -= q0[id] * (h / 6 * (f0_q0)) + q0[id + 1] * (h / 6 * (2 * f1_q1));

	return 0;
}

int f_elem_method::End_Matrix_Newton()
{
	int n = grid.size();
	A.clear();
	A.resize(n);
	f.clear();
	f.resize(n);
	vector<vector<double>> A_loc(2);
	A_loc[0].resize(2);
	A_loc[1].resize(2);
	vector<double> b_loc(2);
	for (int i = 0; i < n; i++)
		A[i].resize(3);

	for (int i = 0; i < n - 1; i++)
	{
		Loc_Matrix_Newton(A_loc, b_loc, i);
		A[i][1] += A_loc[0][0];
		A[i][2] += A_loc[0][1];
		A[i + 1][0] += A_loc[1][0];
		A[i + 1][1] += A_loc[1][1];
		f[i] += b_loc[0];
		f[i + 1] += b_loc[1];
	}

	// ���������
	A[0][0] = 0;
	A[0][1] = 1;
	A[0][2] = 0;
	f[0] = f1;

	A[n - 1][0] = 0;
	A[n - 1][1] = 1;
	A[n - 1][2] = 0;
	f[n - 1] = fn;

	return 0;
}

int f_elem_method::Get_f_Newton()
{
	int n = grid.size();
	f.clear();
	f.resize(n);

	vector<double> b_loc(2);

	for (int i = 0; i < n - 1; i++)
	{
		double f0_q0 = d_fun(grid[i], grid[i], grid[i + 1], q0[i], q0[i + 1], 1);
		double f0_q1 = d_fun(grid[i], grid[i], grid[i + 1], q0[i], q0[i + 1], 2);
		double f1_q0 = d_fun(grid[i + 1], grid[i], grid[i + 1], q0[i], q0[i + 1], 1);
		double f1_q1 = d_fun(grid[i + 1], grid[i], grid[i + 1], q0[i], q0[i + 1], 2);

		double h = grid[i + 1] - grid[i];
		double f1 = fun(grid[i], q0[i]);
		double f2 = fun(grid[i + 1], q0[i + 1]);

		b_Loc(h, f1, f2, b_loc);
		b_loc[0] -= q0[i] * (1 / 6 * (2 * f0_q0 + f1_q0)) + q0[i + 1] * (1 / 6 * (2 * f0_q1 + f1_q1));
		b_loc[1] -= q0[i] * (1 / 6 * (f0_q0 + 2 * f1_q0)) + q0[i + 1] * (1 / 6 * (f0_q1 + 2 * f1_q1));

		f[i] += b_loc[0];
		f[i + 1] += b_loc[1];
	}

	// ���������
	f[0] = f1;
	f[n - 1] = fn;

	return 0;
}

int f_elem_method::Decision_task_Newton(string file)
{
	Create_grid(file);
	int iter;
	double stop = 10;

	for (iter = 0; iter < MAXITER; iter++)
	{
		End_Matrix_Newton();
		stop = Get_Disparity();
		cout << sqrt(stop) << endl;
		if (stop > EPS * EPS)
		{
			Decision t(A, f);
			if (t.LU_f())
			{
				cout << "LU-factorization failed" << endl;
				return -1;
			}
			q1 = q0;
			t.SLAU(q0);
			/*
			for (int i = 0; i < q0.size(); i++)
				q0[i] = q1[i] + w * (q0[i] - q1[i]); */
		}
		else
			return iter;
	}
	return iter;
}
