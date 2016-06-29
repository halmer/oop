#pragma once

class CMyString
{
public:
	class iterator : public std::iterator<std::random_access_iterator_tag, char>
	{
	public:
		iterator() = default;
		explicit iterator(char * ptr);

		char & operator*() const;
		iterator & operator++();
		iterator operator++(int);
		iterator & operator--();
		iterator operator--(int);
		iterator & operator+=(ptrdiff_t n);
		iterator operator+(ptrdiff_t n) const;
		iterator & operator-=(ptrdiff_t n);
		iterator operator-(ptrdiff_t n) const;
		ptrdiff_t operator-(iterator const & rhs) const;
		char & operator[](ptrdiff_t n) const;
		bool operator==(iterator const & rhs) const;
		bool operator!=(iterator const & rhs) const;
		bool operator<(iterator const & rhs) const;
		bool operator>(iterator const & rhs) const;
		bool operator<=(iterator const & rhs) const;
		bool operator>=(iterator const & rhs) const;

	private:
		char * m_ptr;
	};

	class const_iterator : public std::iterator<std::random_access_iterator_tag, char const>
	{
	public:
		const_iterator() = default;
		explicit const_iterator(char const * ptr);

		char const & operator*() const;
		const_iterator & operator++();
		const_iterator operator++(int);
		const_iterator & operator--();
		const_iterator operator--(int);
		const_iterator & operator+=(ptrdiff_t n);
		const_iterator operator+(ptrdiff_t n) const;
		const_iterator & operator-=(ptrdiff_t n);
		const_iterator operator-(ptrdiff_t n) const;
		ptrdiff_t operator-(const_iterator const & rhs) const;
		char const & operator[](ptrdiff_t n) const;
		bool operator==(const_iterator const & rhs) const;
		bool operator!=(const_iterator const & rhs) const;
		bool operator<(const_iterator const & rhs) const;
		bool operator>(const_iterator const & rhs) const;
		bool operator<=(const_iterator const & rhs) const;
		bool operator>=(const_iterator const & rhs) const;

	private:
		char const * m_ptr;
	};
	
public:
	CMyString();
	CMyString(char const * pStr);
	CMyString(char const * pStr, size_t size);
	CMyString(std::string const & stlStr);
	CMyString(CMyString const & other);
	CMyString(CMyString && other) noexcept;
	~CMyString();

	size_t Size() const;
	char const * Data() const;
	CMyString SubStr(size_t start, size_t size = SIZE_MAX) const;
	void Clear();

	CMyString & operator=(CMyString const & rhs);
	CMyString & operator=(CMyString && rhs) noexcept;
	CMyString & operator+=(CMyString const & rhs);
	friend CMyString const operator+(CMyString const & lhs, CMyString const & rhs);
	char & operator[](size_t index);
	char const & operator[](size_t index) const;

	using reverse_iterator = std::reverse_iterator<iterator>;
	using const_reverse_iterator = std::reverse_iterator<const_iterator>;
	iterator begin();
	const_iterator begin() const;
	iterator end();
	const_iterator end() const;

	reverse_iterator rbegin();
	const_reverse_iterator rbegin() const;
	reverse_iterator rend();
	const_reverse_iterator rend() const;
	
	const_iterator cbegin() const;
	const_iterator cend() const;
	const_reverse_iterator crbegin() const;
	const_reverse_iterator crend() const;

private:
	static CMyString Concat(CMyString const & lhs, CMyString const & rhs);
	char * GetStringPtr(size_t offset = 0) const;

	std::allocator<char> m_alloc;
	char * m_first = nullptr;
	size_t m_size = 0;
};

bool operator==(CMyString const & lhs, CMyString const & rhs);
bool operator!=(CMyString const & lhs, CMyString const & rhs);
bool operator<(CMyString const & lhs, CMyString const & rhs);
bool operator>(CMyString const & lhs, CMyString const & rhs);
bool operator<=(CMyString const & lhs, CMyString const & rhs);
bool operator>=(CMyString const & lhs, CMyString const & rhs);
std::ostream & operator<<(std::ostream & os, CMyString const & myStr);
std::istream & operator>>(std::istream & is, CMyString & myStr);

CMyString::iterator operator+(ptrdiff_t n, CMyString::iterator const & it);
CMyString::const_iterator operator+(ptrdiff_t n, CMyString::const_iterator const & it);
