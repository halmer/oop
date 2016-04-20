#include "stdafx.h"
#include "MyFile.h"

using namespace std;

bool ReverseAndCopyFile(CMyFile & fin, CMyFile & fout)
{
	int64_t sizeFile = fin.GetLength();
	bool error = false;
	if (sizeFile > 0)
	{
		vector<int> data;
		data.reserve(sizeFile);
		for (int ch; (ch = fin.GetChar()) != EOF;)
		{
			if (ch == CMyFile::FILE_ERROR)
			{
				error = true;
				break;
			}
			data.push_back(ch);
		}
		if (!error)
		{
			reverse(data.begin(), data.end());
			for (int const ch : data)
			{
				if (!fout.PutChar(ch))
				{
					error = true;
					break;
				}
			}
		}
	}

	return (!(sizeFile < 0) && !error);
}

int main(int argc, char * argv[])
{
	if (argc != 3)
	{
		cout << "Usage: task33.exe <input file> <output file>" << endl;
		return 1;
	}
	
	CMyFile fin(argv[1]);
	CMyFile fout(argv[2], "w");
	if (!fin.IsOpened() || !fout.IsOpened())
	{
		cout << "Error opening file(s)" << endl;
		return 1;
	}

	if (!ReverseAndCopyFile(fin, fout))
	{
		cout << "Copy error" << endl;
	}

	return 0;
}
