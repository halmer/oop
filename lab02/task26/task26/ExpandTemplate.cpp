#include "stdafx.h"
#include "ExpandTemplate.h"

using namespace std;

CExpandTemplate::CExpandTemplate()
	: m_curState{ nullptr }
	, m_patterns{ " " }
	, m_values{ " " }
{
	m_root = new Node;
}

CExpandTemplate::~CExpandTemplate()
{
	for (auto ptr : m_ptrVector)
	{
		delete ptr;
	}

	delete m_root;
}

void CExpandTemplate::AddReplaceMap(map<string, string> const & replaceMap)
{
	if (!m_ptrVector.empty())
	{
		Reset();
	}

	for (auto const & item : replaceMap)
	{
		if (item.first.empty())
		{
			continue;
		}

		Node * curNode = m_root;
		for (auto ch : item.first)
		{
			Node * sonNode = GetNodeLinkingChar(curNode, ch);
			if (!sonNode)
			{
				m_ptrVector.push_back(new Node);
				sonNode = m_ptrVector.back();
				sonNode->suffix = curNode;
				curNode->link[ch] = sonNode;
			}
			curNode = sonNode;
		}

		curNode->numTerminal = m_patterns.size();
		m_patterns.push_back(item.first);
		m_values.push_back(item.second);
	}

	CreateSuffixLink();
}

CExpandTemplate::Node * CExpandTemplate::GetNodeLinkingChar(Node const * node, char ch) const
{
	auto it = node->link.find(ch);
	return (it != node->link.end()) ? it->second : nullptr;
}

void CExpandTemplate::CreateSuffixLink()
{
	queue<Node*> queueNode;
	queueNode.push(m_root);
	while (!queueNode.empty())
	{
		Node * curNode = queueNode.front();
		queueNode.pop();
		for (auto const & item : curNode->link)
		{
			char ch = item.first;
			Node * sonNode = item.second;

			Node * suffixNode = curNode->suffix;
			while (suffixNode)
			{
				if (GetNodeLinkingChar(suffixNode, ch))
				{
					sonNode->suffix = GetNodeLinkingChar(suffixNode, ch);
					break;
				}
				suffixNode = suffixNode->suffix;
			}
	
			sonNode->terminal = (sonNode->suffix->numTerminal) ? sonNode->suffix
															   : sonNode->suffix->terminal;
			queueNode.push(sonNode);
		}
	}
}

vector<uint8_t> CExpandTemplate::FindAllReplacement(string const & str)
{
	vector<uint8_t> replacement;
	replacement.resize(str.size());

	m_curState = m_root;
	int curPosStr = 0;
	for (auto ch : str)
	{
		NextState(ch);
		++curPosStr;
		if (m_curState->numTerminal)
		{
			replacement[curPosStr - m_patterns[m_curState->numTerminal].size()] = m_curState->numTerminal;
		}

		Node * termNode = m_curState->terminal;
		while (termNode)
		{
			replacement[curPosStr - m_patterns[termNode->numTerminal].size()] = termNode->numTerminal;
			termNode = termNode->terminal;
		}
	}
	
	return replacement;
}

string CExpandTemplate::Replace(string const & str)
{
	if (str.empty() || m_ptrVector.empty())
	{
		return str;
	}

	vector<uint8_t> replacement = FindAllReplacement(str);

	string resultStr;
	resultStr.reserve(str.size());
	for (size_t i = 0; i < replacement.size();)
	{
		if (replacement[i])
		{
			resultStr += m_values[replacement[i]];
			i += m_patterns[replacement[i]].size();
		}
		else
		{
			resultStr += str[i++];
		}
	}

	return resultStr;
}

void CExpandTemplate::NextState(char ch)
{
	while (m_curState)
	{
		if (GetNodeLinkingChar(m_curState, ch))
		{
			m_curState = GetNodeLinkingChar(m_curState, ch);
			return;
		}
		m_curState = m_curState->suffix;
	}
	m_curState = m_root;
}

void CExpandTemplate::Reset()
{
	for (auto item : m_ptrVector)
	{
		delete item;
	}
	m_ptrVector.clear();

	delete m_root;
	m_root = new Node;

	m_curState = nullptr;
	m_patterns.clear();
	m_values.clear();
	m_patterns.push_back(" ");
	m_values.push_back(" ");
}
