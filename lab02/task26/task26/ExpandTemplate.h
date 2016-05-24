#pragma once

class CExpandTemplate
{
public:
	CExpandTemplate();
	CExpandTemplate(CExpandTemplate const &) = delete;
	CExpandTemplate & operator=(CExpandTemplate const &) = delete;

	void AddReplaceMap(std::map<std::string, std::string> const & replaceMap);
	std::string Replace(std::string const & str);

private:
	struct Node
	{
		std::map<char, Node> link;
		Node * suffix = nullptr;
		Node * terminal = nullptr;
		uint32_t numTerminal = 0;
	};

	Node * GetNodeLinkingChar(Node * node, char ch) const;
	void CreateSuffixLink();
	void FillingString(uint32_t viewerInd, uint32_t & pass, std::string const & tpl, std::string & resultStr);
	void FindReplacement(uint32_t viewerInd, uint32_t viewerSize);
	void NextState(char ch);
	void Reset();
	
	Node m_root;
	Node * m_curState;
	std::vector<std::string> m_patterns;
	std::vector<std::string> m_values;
	std::vector<int> m_replacement;
};
