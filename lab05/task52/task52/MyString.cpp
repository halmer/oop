#include "stdafx.h"
#include "MyString.h"

using namespace std;

CMyString::CMyString()
{
}

CMyString::CMyString(char const * pStr)
	: CMyString(pStr, strlen(pStr))
{
}

CMyString::CMyString(char const * pStr, size_t size)
{
	if (size)
	{
		m_first = m_alloc.allocate(size + 1);
		char * end = uninitialized_copy_n(pStr, size, m_first);
		*end = 0;
		m_size = size;
	}
}

CMyString::CMyString(std::string const & stlStr)
	: CMyString(stlStr.data(), stlStr.size())
{
}

CMyString::CMyString(CMyString const & other)
	: CMyString(other.m_first, other.m_size)
{
}

CMyString::CMyString(CMyString && other) noexcept
	: m_first(other.m_first)
	, m_size(other.m_size)
{
	other.m_first = nullptr;
	other.m_size = 0;
}

CMyString::~CMyString()
{
	if (m_first)
	{
		m_alloc.deallocate(m_first, m_size + 1);
	}
}

size_t CMyString::Size() const
{
	return m_size;
}

char * CMyString::GetStringPtr(size_t offset) const
{
	return m_first ? (m_first + offset) : "";
}

char const * CMyString::Data() const
{
	return GetStringPtr();
}

CMyString CMyString::SubStr(size_t start, size_t size) const
{
	if (m_size <= start)
	{
		return CMyString();
	}
	size_t minSize = min(m_size - start, size);

	return CMyString(m_first + start, minSize);
}

void CMyString::Clear()
{
	if (m_first)
	{
		this->~CMyString();
		m_first = nullptr;
		m_size = 0;
	}
}

CMyString & CMyString::operator=(CMyString const & rhs)
{
	if (this != &rhs)
	{
		CMyString tmp(rhs);
		swap(m_first, tmp.m_first);
		swap(m_size, tmp.m_size);
	}

	return *this;
}

CMyString & CMyString::operator=(CMyString && rhs) noexcept
{
	if (this != &rhs)
	{
		this->~CMyString();
		m_first = rhs.m_first;
		m_size = rhs.m_size;
		rhs.m_first = nullptr;
		rhs.m_size = 0;
	}

	return *this;
}

CMyString CMyString::Concat(CMyString const & lhs, CMyString const & rhs)
{
	CMyString res;
	res.m_size = lhs.Size() + rhs.Size();
	char * tmp = res.m_alloc.allocate(res.m_size + 1);
	char * end = uninitialized_copy_n(lhs.Data(), lhs.Size(), tmp);
	end = uninitialized_copy_n(rhs.Data(), rhs.Size(), end);
	*end = 0;
	res.m_first = tmp;

	return res;
}

CMyString & CMyString::operator+=(CMyString const & rhs)
{
	if (rhs.Size())
	{
		*this = Concat(*this, rhs);
	}

	return *this;
}

CMyString const operator+(CMyString const & lhs, CMyString const & rhs)
{
	if (lhs.Size() && rhs.Size())
	{
		return CMyString::Concat(lhs, rhs);
	}

	return (lhs.Size()) ? lhs : rhs;
}

char & CMyString::operator[](size_t index)
{
	assert(index < m_size);
	return m_first[index];
}

char const & CMyString::operator[](size_t index) const
{
	assert(index < m_size);
	return m_first[index];
}

static int Compare(CMyString const & lhs, CMyString const & rhs)
{
	if (lhs.Size() == rhs.Size())
	{
		return memcmp(lhs.Data(), rhs.Data(), rhs.Size());
	}

	int cmp = memcmp(lhs.Data(), rhs.Data(), min(lhs.Size(), rhs.Size()));
	if (cmp != 0)
	{
		return cmp;
	}

	return (lhs.Size() > rhs.Size()) ? 1 : -1;
}

bool operator==(CMyString const & lhs, CMyString const & rhs)
{
	return (Compare(lhs, rhs) == 0);
}

bool operator!=(CMyString const & lhs, CMyString const & rhs)
{
	return (Compare(lhs, rhs) != 0);
}

bool operator<(CMyString const & lhs, CMyString const & rhs)
{
	return (Compare(lhs, rhs) < 0);
}

bool operator>(CMyString const & lhs, CMyString const & rhs)
{
	return (Compare(lhs, rhs) > 0);
}

bool operator<=(CMyString const & lhs, CMyString const & rhs)
{
	return (Compare(lhs, rhs) <= 0);
}

bool operator>=(CMyString const & lhs, CMyString const & rhs)
{
	return (Compare(lhs, rhs) >= 0);
}

ostream & operator<<(ostream & os, CMyString const & myStr)
{
	copy_n(myStr.Data(), myStr.Size(), ostream_iterator<char>(os));
	return os;
}

istream & operator>>(istream & is, CMyString & myStr)
{
	string str;
	bool isFoundChar = false;
	for (char ch; !is.eof() && !is.fail() && is.get(ch);)
	{
		if (isspace(ch))
		{
			if (isFoundChar)
			{
				break;
			}
		}
		else
		{
			str += ch;
			isFoundChar = true;
		}
	}
	myStr = str;

	return is;
}

CMyString::iterator CMyString::begin()
{
	return iterator(GetStringPtr());
}

CMyString::const_iterator CMyString::begin() const
{
	return const_iterator(GetStringPtr());
}

CMyString::iterator CMyString::end()
{
	return iterator(GetStringPtr(m_size));
}

CMyString::const_iterator CMyString::end() const
{
	return const_iterator(GetStringPtr(m_size));
}

CMyString::reverse_iterator CMyString::rbegin()
{
	return reverse_iterator(end());
}

CMyString::const_reverse_iterator CMyString::rbegin() const
{
	return const_reverse_iterator(end());
}

CMyString::reverse_iterator CMyString::rend()
{
	return reverse_iterator(begin());
}

CMyString::const_reverse_iterator CMyString::rend() const
{
	return const_reverse_iterator(begin());
}

CMyString::const_iterator CMyString::cbegin() const
{
	return const_iterator(GetStringPtr());
}

CMyString::const_iterator CMyString::cend() const
{
	return const_iterator(GetStringPtr(m_size));
}

CMyString::const_reverse_iterator CMyString::crbegin() const
{
	return const_reverse_iterator(end());
}

CMyString::const_reverse_iterator CMyString::crend() const
{
	return const_reverse_iterator(begin());
}

CMyString::iterator::iterator(char * ptr)
	: m_ptr(ptr)
{
}

char & CMyString::iterator::operator*() const
{
	assert(m_ptr != nullptr);
	return *m_ptr;
}

CMyString::iterator & CMyString::iterator::operator++()
{
	++m_ptr;
	return *this;
}

CMyString::iterator CMyString::iterator::operator++(int)
{
	auto tmp = *this;
	++m_ptr;
	return tmp;
}

CMyString::iterator & CMyString::iterator::operator--()
{
	--m_ptr;
	return *this;
}

CMyString::iterator CMyString::iterator::operator--(int)
{
	auto tmp = *this;
	--m_ptr;
	return tmp;
}

CMyString::iterator & CMyString::iterator::operator+=(ptrdiff_t n)
{
	m_ptr += n;
	return *this;
}

CMyString::iterator CMyString::iterator::operator+(ptrdiff_t n) const
{
	auto tmp = *this;
	tmp += n;
	return tmp;
}

CMyString::iterator operator+(ptrdiff_t n, CMyString::iterator const & it)
{
	auto tmp = it + n;
	return tmp;
}

CMyString::iterator & CMyString::iterator::operator-=(ptrdiff_t n)
{
	m_ptr -= n;
	return *this;
}

CMyString::iterator CMyString::iterator::operator-(ptrdiff_t n) const
{
	auto tmp = *this;
	tmp -= n;
	return tmp;
}

ptrdiff_t CMyString::iterator::operator-(iterator const & rhs) const
{
	return m_ptr - rhs.m_ptr;
}

char & CMyString::iterator::operator[](ptrdiff_t n) const
{
	return m_ptr[n];
}

bool CMyString::iterator::operator==(iterator const & rhs) const
{
	return m_ptr == rhs.m_ptr;
}

bool CMyString::iterator::operator!=(iterator const & rhs) const
{
	return m_ptr != rhs.m_ptr;
}

bool CMyString::iterator::operator<(iterator const & rhs) const
{
	return m_ptr < rhs.m_ptr;
}

bool CMyString::iterator::operator>(iterator const & rhs) const
{
	return m_ptr > rhs.m_ptr;
}

bool CMyString::iterator::operator<=(iterator const & rhs) const
{
	return m_ptr <= rhs.m_ptr;
}

bool CMyString::iterator::operator>=(iterator const & rhs) const
{
	return m_ptr >= rhs.m_ptr;
}

CMyString::const_iterator::const_iterator(char const * ptr)
	: m_ptr(ptr)
{
}

char const & CMyString::const_iterator::operator*() const
{
	assert(m_ptr != nullptr);
	return *m_ptr;
}

CMyString::const_iterator & CMyString::const_iterator::operator++()
{
	++m_ptr;
	return *this;
}

CMyString::const_iterator CMyString::const_iterator::operator++(int)
{
	auto tmp = *this;
	++m_ptr;
	return tmp;
}

CMyString::const_iterator & CMyString::const_iterator::operator--()
{
	--m_ptr;
	return *this;
}

CMyString::const_iterator CMyString::const_iterator::operator--(int)
{
	auto tmp = *this;
	--m_ptr;
	return tmp;
}

CMyString::const_iterator & CMyString::const_iterator::operator+=(ptrdiff_t n)
{
	m_ptr += n;
	return *this;
}

CMyString::const_iterator CMyString::const_iterator::operator+(ptrdiff_t n) const
{
	auto tmp = *this;
	tmp += n;
	return tmp;
}

CMyString::const_iterator operator+(ptrdiff_t n, CMyString::const_iterator const & it)
{
	auto tmp = it + n;
	return tmp;
}

CMyString::const_iterator & CMyString::const_iterator::operator-=(ptrdiff_t n)
{
	m_ptr -= n;
	return *this;
}

CMyString::const_iterator CMyString::const_iterator::operator-(ptrdiff_t n) const
{
	auto tmp = *this;
	tmp -= n;
	return tmp;
}

ptrdiff_t CMyString::const_iterator::operator-(const_iterator const & rhs) const
{
	return m_ptr - rhs.m_ptr;
}

char const & CMyString::const_iterator::operator[](ptrdiff_t n) const
{
	return m_ptr[n];
}

bool CMyString::const_iterator::operator==(const_iterator const & rhs) const
{
	return m_ptr == rhs.m_ptr;
}

bool CMyString::const_iterator::operator!=(const_iterator const & rhs) const
{
	return m_ptr != rhs.m_ptr;
}

bool CMyString::const_iterator::operator<(const_iterator const & rhs) const
{
	return m_ptr < rhs.m_ptr;
}

bool CMyString::const_iterator::operator>(const_iterator const & rhs) const
{
	return m_ptr > rhs.m_ptr;
}

bool CMyString::const_iterator::operator<=(const_iterator const & rhs) const
{
	return m_ptr <= rhs.m_ptr;
}

bool CMyString::const_iterator::operator>=(const_iterator const & rhs) const
{
	return m_ptr >= rhs.m_ptr;
}
