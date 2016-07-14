#pragma once

class CMyStringList
{
	struct Node;
public:
	class const_iterator : public std::iterator<std::bidirectional_iterator_tag, std::string const>
	{
		friend CMyStringList;
	public:
		const_iterator() = default;
		explicit const_iterator(Node * node);

		std::string const & operator*() const;
		std::string const * operator->() const;
		const_iterator & operator++();
		const_iterator operator++(int);
		const_iterator & operator--();
		const_iterator operator--(int);
		bool operator==(const_iterator const & rhs) const;
		bool operator!=(const_iterator const & rhs) const;

	private:
		Node * m_node = nullptr;
	};

	class iterator : public std::iterator<std::bidirectional_iterator_tag, std::string>
	{
		friend CMyStringList;
	public:
		iterator() = default;
		explicit iterator(Node * node);

		std::string & operator*() const;
		std::string * operator->() const;
		iterator & operator++();
		iterator operator++(int);
		iterator & operator--();
		iterator operator--(int);
		bool operator==(iterator const & rhs) const;
		bool operator!=(iterator const & rhs) const;
		operator const_iterator() const;
		
	private:
		Node * m_node = nullptr;
	};

public:
	CMyStringList();
	template<class Iter>
	CMyStringList(Iter first, Iter last);
	CMyStringList(std::initializer_list<std::string> const & initList);
	CMyStringList(CMyStringList const & other);
	~CMyStringList();

	CMyStringList & operator=(CMyStringList const & rhs);

	void PushFront(std::string const & str);
	void PushBack(std::string const & str);
	iterator Insert(const_iterator pos, std::string const & str);
	iterator Erase(const_iterator pos);
	size_t Size() const;
	bool Empty() const;
	void Clear();

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
	struct Node
	{
		Node() = default;
		Node(Node * node, std::string const & str);

		std::string data;
		Node * prev = nullptr;
		Node * next = nullptr;
	};

	void DeleteList();

	std::unique_ptr<Node> m_first;
	std::unique_ptr<Node> m_last;
	size_t m_size = 0;
};

template<class Iter>
CMyStringList::CMyStringList(Iter first, Iter last)
	: CMyStringList()
{
	while (first != last)
	{
		Insert(end(), *first++);
	}
}
