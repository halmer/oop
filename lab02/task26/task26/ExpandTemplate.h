#pragma once

class CExpandTemplate
{
public:
	CExpandTemplate();
	CExpandTemplate(CExpandTemplate const &) = delete;
	CExpandTemplate & operator=(CExpandTemplate const &) = delete;
	~CExpandTemplate();

	void AddReplaceMap(std::map<std::string, std::string> const & replaceMap);
	std::string Replace(std::string const & str);

private:
	struct Node
	{
		std::map<char, Node*> link;
		Node * suffix = nullptr;
		Node * terminal = nullptr;
		uint8_t numTerminal = 0;
	};

	Node * GetNodeLinkingChar(Node const * node, char ch) const;
	void CreateSuffixLink();
	std::vector<uint8_t> FindAllReplacement(std::string const & str);
	void NextState(char ch);
	void Reset();
	
	Node * m_root;
	Node * m_curState;
	std::vector<std::string> m_patterns;
	std::vector<std::string> m_values;
	std::vector<Node *> m_ptrVector;
};
