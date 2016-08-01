#include "stdafx.h"
#include "Calculator.h"

using namespace std;
using namespace std::chrono;

int main()
{
	ofstream fout("out.txt");
	if (fout)
	{
		CCalculator calc(cin, fout);

		auto start = steady_clock::now();
		calc.Run();
		auto end = steady_clock::now();
		cout << duration_cast<milliseconds>(end - start).count() << endl;
	}

	return 0;
}
