#pragma once

class CMyFile
{
public:
	static constexpr int FILE_ERROR = -2;

	CMyFile();
	explicit CMyFile(std::string const & fileName, std::string const & mode = "r");
	CMyFile(CMyFile const &) = delete;
	CMyFile & operator=(CMyFile const &) = delete;
	~CMyFile();

	bool Open(std::string const & fileName, std::string const & mode = "r");
	void Close();
	bool IsOpened() const;
	bool IsEndOfFile() const;
	int GetChar();
	bool PutChar(int ch);
	int64_t GetLength() const;
	bool Seek(int64_t offset, int origin = SEEK_SET);
	int64_t GetPosition() const;
	int64_t Write(void const * buffer, size_t size, size_t count);
	int64_t Read(void * buffer, size_t size, size_t count);

private:
	FILE * m_file;
};
