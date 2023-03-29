#include "Base.h"
#include "Decision.h"

int main()
{
	f_elem_method t;
	// метод Ньютона
	int iter1 = t.Decision_task_N("test.txt");
	ofstream fout("fout_N.txt");

	fout.precision(16); //16 знаков
	fout.imbue(locale("Russian")); // , => .

	fout << iter1 << endl;

	for (int i = 0; i < t.q0.size(); i++)
		fout << t.q0[i] << endl;
	return 0;
}
