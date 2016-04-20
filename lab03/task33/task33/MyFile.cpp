#include "stdafx.h"
#include "MyFile.h"
#include <set>

using namespace std;

bool checkParam(string const & fileName, string const & mode)
{
	static const set<string> modeType{
		"r", "w", "a",
		"r+", "w+", "a+",
		"rb", "wb", "ab",
		"rb+", "wb+", "ab+",
	};
	auto it = modeType.find(mode);

	return (!fileName.empty() && it != modeType.end());
}

CMyFile::CMyFile()
	: m_file(nullptr)
{
}

CMyFile::CMyFile(string const & fileName, string const & mode)
	: m_file(nullptr)
{
	if (checkParam(fileName, mode))
	{
		fopen_s(&m_file, fileName.c_str(), mode.c_str());
	}
}

CMyFile::~CMyFile()
{
	Close();
}

bool CMyFile::Open(string const & fileName, string const & mode)
{
	Close();
	if (checkParam(fileName, mode))
	{
		fopen_s(&m_file, fileName.c_str(), mode.c_str());
	}

	return IsOpened();
}

void CMyFile::Close()
{
	if (m_file)
	{
		fclose(m_file);
		m_file = nullptr;
	}
}

bool CMyFile::IsOpened() const
{
	return (m_file != nullptr);
}

bool CMyFile::IsEndOfFile() const
{
	return (IsOpened() && feof(m_file));
}

int CMyFile::GetChar()
{
	if (!IsOpened())
	{
		return FILE_ERROR;
	}

	clearerr(m_file);
	int ch = getc(m_file);
	if (ferror(m_file))
	{
		ch = FILE_ERROR;
	}
	
	return ch;
}

bool CMyFile::PutChar(int ch)
{
	return (IsOpened() && putc(ch, m_file) == ch);
}

int64_t CMyFile::GetLength() const
{
	int64_t length = -1LL;
	if (IsOpened())
	{
		int64_t curPosition = GetPosition();
		if ((_fseeki64(m_file, 0, SEEK_END) == 0) && curPosition >= 0)
		{
			length = GetPosition();
			_fseeki64(m_file, curPosition, SEEK_SET);
		}
	}
	
	return length;
}

bool CMyFile::Seek(int64_t offset, int origin)
{
	return (IsOpened()) ? (_fseeki64(m_file, offset, origin) == 0) : false;
}

int64_t CMyFile::GetPosition() const
{
	return (IsOpened()) ? _ftelli64(m_file) : -1LL;
}

int64_t CMyFile::Write(void const * buffer, size_t size, size_t count)
{
	return (IsOpened() && buffer) ? fwrite(buffer, size, count, m_file) : -1LL;
}

int64_t CMyFile::Read(void * buffer, size_t size, size_t count)
{
	return (IsOpened() && buffer) ? fread(buffer, size, count, m_file) : -1LL;
}
