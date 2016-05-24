#include "stdafx.h"
#include "ExpandTemplate.h"

using namespace std;
using namespace std::chrono;

map<string, string> CreateReplaceMap(int ind, char * str[])
{
	map<string, string> replaceMap;
	for (int i = 3; i < ind; i += 2)
	{
		replaceMap[str[i]] = str[i + 1];
	}

	return replaceMap;
}

int main(int argc, char * argv[])
{
	if (argc % 2 == 0 || argc < 3)
	{
		cout << "Usage: task26.exe <input file> <output file> [<param> <value> [<param> <value> ...]]" << endl;
		return 1;
	}

	ifstream fin(argv[1]);
	ofstream fout(argv[2]);
	if (!fin.is_open() || !fout.is_open())
	{
		cout << "Error opening file(s)" << endl;
		return 1;
	}

	auto replaceMap = CreateReplaceMap(argc, argv);
	string origStr;
	for (string str; getline(fin, str);)
	{
		origStr += str + '\n';
	}

	auto start = steady_clock::now();

	CExpandTemplate expTpl;
	expTpl.AddReplaceMap(replaceMap);
	string resultStr = expTpl.Replace(origStr);

	auto end = steady_clock::now();
	cout << duration_cast<milliseconds>(end - start).count() << endl;

	fout << resultStr;

	return 0;
}
