#include "stdafx.h"
#include "MyStringList.h"

using namespace std;

CMyStringList::CMyStringList()
	: m_first(new Node)
	, m_last(new Node)
{
	m_first->next = m_last.get();
	m_last->prev = m_first.get();
}

CMyStringList::CMyStringList(initializer_list<string> const & initList)
	: CMyStringList(initList.begin(), initList.end())
{
}

CMyStringList::CMyStringList(CMyStringList const & other)
	: CMyStringList(other.begin(), other.end())
{
}

void CMyStringList::DeleteList()
{
	Node * deleteNode = m_first->next;
	while (deleteNode != m_last.get())
	{
		Node * nextNode = deleteNode->next;
		delete  deleteNode;
		deleteNode = nextNode;
	}
}

CMyStringList::~CMyStringList()
{
	DeleteList();
}

CMyStringList & CMyStringList::operator=(CMyStringList const & rhs)
{
	if (this != &rhs)
	{
		CMyStringList tmp(rhs);
		swap(m_first, tmp.m_first);
		swap(m_last, tmp.m_last);
		swap(m_size, tmp.m_size);
	}

	return *this;
}

void CMyStringList::PushFront(string const & str)
{
	Insert(begin(), str);
}

void CMyStringList::PushBack(string const & str)
{
	Insert(end(), str);
}

CMyStringList::Node::Node(Node * node, string const & str)
	: data(str)
	, prev(node->prev)
	, next(node)
{
}

CMyStringList::iterator CMyStringList::Insert(const_iterator pos, string const & str)
{
	Node * newNode = new Node(pos.m_node, str);
	pos.m_node->prev = newNode;
	newNode->prev->next = newNode;
	++m_size;

	return iterator(newNode);
}

CMyStringList::iterator CMyStringList::Erase(const_iterator pos)
{
	assert(m_size > 0);

	Node * eraseNode = pos.m_node;
	eraseNode->prev->next = eraseNode->next;
	eraseNode->next->prev = eraseNode->prev;
	Node * nextNode = eraseNode->next;
	delete eraseNode;
	--m_size;

	return iterator(nextNode);
}

size_t CMyStringList::Size() const
{
	return m_size;
}

bool CMyStringList::Empty() const
{
	return m_size == 0;
}

void CMyStringList::Clear()
{
	DeleteList();
 	m_first->next = m_last.get();
 	m_last->prev = m_first.get();
	m_size = 0;
}

CMyStringList::iterator CMyStringList::begin()
{
	return iterator(m_first->next);
}

CMyStringList::const_iterator CMyStringList::begin() const
{
	return const_iterator(m_first->next);
}

CMyStringList::iterator CMyStringList::end()
{
	return iterator(m_last.get());
}

CMyStringList::const_iterator CMyStringList::end() const
{
	return const_iterator(m_last.get());
}

CMyStringList::reverse_iterator CMyStringList::rbegin()
{
	return reverse_iterator(end());
}

CMyStringList::const_reverse_iterator CMyStringList::rbegin() const
{
	return const_reverse_iterator(end());
}

CMyStringList::reverse_iterator CMyStringList::rend()
{
	return reverse_iterator(begin());
}

CMyStringList::const_reverse_iterator CMyStringList::rend() const
{
	return const_reverse_iterator(begin());
}

CMyStringList::const_iterator CMyStringList::cbegin() const
{
	return const_iterator(m_first->next);
}

CMyStringList::const_iterator CMyStringList::cend() const
{
	return const_iterator(m_last.get());
}

CMyStringList::const_reverse_iterator CMyStringList::crbegin() const
{
	return const_reverse_iterator(end());
}

CMyStringList::const_reverse_iterator CMyStringList::crend() const
{
	return const_reverse_iterator(begin());
}

CMyStringList::const_iterator::const_iterator(Node * node)
	: m_node(node)
{
}

string const & CMyStringList::const_iterator::operator*() const
{
	assert(m_node);
	assert(m_node->next);

	return m_node->data;
}

string const * CMyStringList::const_iterator::operator->() const
{
	return &**this;
}

CMyStringList::const_iterator & CMyStringList::const_iterator::operator++()
{
	assert(m_node);
	assert(m_node->next);

	m_node = m_node->next;
	return *this;
}

CMyStringList::const_iterator CMyStringList::const_iterator::operator++(int)
{
	auto copy = *this;
	++*this;
	return copy;
}

CMyStringList::const_iterator & CMyStringList::const_iterator::operator--()
{
	assert(m_node);
	assert(m_node->prev);
	assert(m_node->prev->prev);
	
	m_node = m_node->prev;
	return *this;
}

CMyStringList::const_iterator CMyStringList::const_iterator::operator--(int)
{
	auto copy = *this;
	--*this;
	return copy;
}

bool CMyStringList::const_iterator::operator==(const_iterator const & rhs) const
{
	return m_node == rhs.m_node;
}

bool CMyStringList::const_iterator::operator!=(const_iterator const & rhs) const
{
	return m_node != rhs.m_node;
}

CMyStringList::iterator::iterator(Node * node)
	: m_node(node)
{
}

string & CMyStringList::iterator::operator*() const
{
	assert(m_node);
	assert(m_node->next);

	return m_node->data;
}

string * CMyStringList::iterator::operator->() const
{
	return &**this;
}

CMyStringList::iterator & CMyStringList::iterator::operator++()
{
	assert(m_node);
	assert(m_node->next);

	m_node = m_node->next;
	return *this;
}

CMyStringList::iterator CMyStringList::iterator::operator++(int)
{
	auto copy = *this;
	++*this;
	return copy;
}

CMyStringList::iterator & CMyStringList::iterator::operator--()
{
	assert(m_node);
	assert(m_node->prev);
	assert(m_node->prev->prev);

	m_node = m_node->prev;
	return *this;
}

CMyStringList::iterator CMyStringList::iterator::operator--(int)
{
	auto copy = *this;
	--*this;
	return copy;
}

bool CMyStringList::iterator::operator==(iterator const & rhs) const
{
	return m_node == rhs.m_node;
}

bool CMyStringList::iterator::operator!=(iterator const & rhs) const
{
	return m_node != rhs.m_node;
}

CMyStringList::iterator::operator CMyStringList::const_iterator() const
{
	return const_iterator(m_node);
}
