#include "stdafx.h"
#include "ExpandTemplate.h"

using namespace std;

CExpandTemplate::CExpandTemplate()
	: m_curState{ nullptr }
	, m_patterns{ " " }
	, m_values{ " " }
{
}

void CExpandTemplate::AddReplaceMap(map<string, string> const & replaceMap)
{
	if (!m_root.link.empty())
	{
		Reset();
	}

	for (auto const & item : replaceMap)
	{
		if (item.first.empty())
		{
			continue;
		}

		Node * curNode = &m_root;
		for (auto ch : item.first)
		{
			Node * sonNode = GetNodeLinkingChar(curNode, ch);
			if (!sonNode)
			{
				curNode->link.emplace(ch, Node());
				curNode->link[ch].suffix = curNode;
				sonNode = &curNode->link[ch];
			}
			curNode = sonNode;
		}

		curNode->numTerminal = m_patterns.size();
		m_patterns.push_back(item.first);
		m_values.push_back(item.second);
	}

	CreateSuffixLink();
}

CExpandTemplate::Node * CExpandTemplate::GetNodeLinkingChar(Node * node, char ch) const
{
	auto it = node->link.find(ch);
	return (it != node->link.end()) ? &it->second : nullptr;
}

void CExpandTemplate::CreateSuffixLink()
{
	queue<Node*> queueNode;
	queueNode.push(&m_root);
	while (!queueNode.empty())
	{
		Node * curNode = queueNode.front();
		queueNode.pop();
		for (auto & item : curNode->link)
		{
			char ch = item.first;
			Node * sonNode = &item.second;

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

void CExpandTemplate::FillingString(uint32_t viewerInd, uint32_t & pass, string const & tpl, string & resultStr)
{
	if (pass == 0)
	{
		if (m_replacement[viewerInd] > 0)
		{
			resultStr += m_values[m_replacement[viewerInd]];
			pass = m_patterns[m_replacement[viewerInd]].size() - 1;
		}
		else
		{
			resultStr += tpl[-m_replacement[viewerInd]];
		}
	}
	else
	{
		--pass;
	}
}

void CExpandTemplate::FindReplacement(uint32_t viewerInd, uint32_t viewerSize)
{
	Node * termNode = m_curState;
	while (termNode)
	{
		uint32_t numPattern = termNode->numTerminal;
		if (numPattern)
		{
			if ((viewerInd < m_patterns[numPattern].size()))
			{
				m_replacement[viewerSize + viewerInd - m_patterns[numPattern].size()] = numPattern;
			}
			else
			{
				m_replacement[viewerInd - m_patterns[numPattern].size()] = numPattern;
			}
		}

		termNode = termNode->terminal;
	}
}

string CExpandTemplate::Replace(string const & tpl)
{
	if (tpl.empty() || m_root.link.empty())
	{
		return tpl;
	}

	auto patternMaxSize = max_element(m_patterns.begin(), m_patterns.end(),
		[](string const & lhs, string const & rhs){
			return lhs.size() < rhs.size(); 
	});
	uint32_t viewerSize = min(patternMaxSize->size(), tpl.size());
	m_replacement.resize(viewerSize);
	m_curState = &m_root;
	for (uint32_t i = 0; i < viewerSize; ++i)
	{
		NextState(tpl[i]);
		m_replacement[i] = -static_cast<int>(i);
		FindReplacement(i + 1, viewerSize);
	}

	uint32_t viewerInd = 0;
	uint32_t pass = 0;
	string resultStr;
	resultStr.reserve(tpl.size());
	for (uint32_t i = viewerSize; i < tpl.size(); ++i)
	{
		NextState(tpl[i]);
		FillingString(viewerInd, pass, tpl, resultStr);
		m_replacement[viewerInd] = -static_cast<int>(i);
		FindReplacement(viewerInd + 1, viewerSize);
		viewerInd = (viewerInd == viewerSize - 1) ? 0 : viewerInd + 1;
	}

	for (uint32_t i = viewerInd; i < viewerSize; ++i)
	{
		FillingString(i, pass, tpl, resultStr);
	}

	for (uint32_t i = 0; i < viewerInd; ++i)
	{
		FillingString(i, pass, tpl, resultStr);
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
	m_curState = &m_root;
}

void CExpandTemplate::Reset()
{
	m_root.link.clear();
	m_curState = nullptr;
	m_patterns.clear();
	m_values.clear();
	m_patterns.push_back(" ");
	m_values.push_back(" ");	
}
