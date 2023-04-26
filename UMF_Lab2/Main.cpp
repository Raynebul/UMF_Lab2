#include "Base.h"
#include "Decision.h"
//g
int main()
{
	f_elem_method t;
	// метод простой итерации
	//int iter1 = t.Decision_task("Test.txt");
	int iter1 = t.Decision_task("Test.txt");
	ofstream fout("fout.txt");
	fout.precision(16); //16 знаков
	//fout.imbue(locale("Russian")); // , => .
	fout << iter1 << endl;
	for (int i = 0; i < t.q0.size(); i++)
		fout << t.q0[i] << endl;
	return 0;
}
