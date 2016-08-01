#include "stdafx.h"
#include "Lexer.h"

using namespace std;
using boost::optional;
using boost::none;

Token::Token(TokenType type, string const & value)
	: type(type)
	, value(value)
{
}

CLexer::CLexer()
	: m_rgx(R"(^\s*(var|let|fn|print(?:vars|fns)?)(?:\s+([a-zA-Z_]\w*)\s*(?:=\s*(?:(-?\d*\.?\d+)|([a-zA-Z_]\w*))\s*(?:([+\-*/])\s*([a-zA-Z_]\w*))?)?)?\s*$)")
{
}

vector<Token> CLexer::GetTokensRgx(string const & expr) const
{
	vector<Token> tokens;
	tokens.reserve(6);
	smatch match;

	if (regex_search(expr, match, m_rgx))
	{
		if (match[1].matched)
		{
			tokens.emplace_back(TokenType::Operator, match[1]);
		}
		if (match[2].matched)
		{
			tokens.emplace_back(TokenType::Id, match[2]);
		}
		if (match[3].matched)
		{
			tokens.emplace_back(TokenType::Number, match[3]);
		}
		if (match[4].matched)
		{
			tokens.emplace_back(TokenType::Id, match[4]);
		}
		if (match[5].matched)
		{
			tokens.emplace_back(TokenType::BinaryOperator, match[5]);
		}
		if (match[6].matched)
		{
			tokens.emplace_back(TokenType::Id, match[6]);
		}
	}

	return tokens;
}

vector<Token> CLexer::GetTokens(string const & expr) const
{
	vector<Token> tokens;
	tokens.reserve(6);
	vector<optional<string>> lexemes = ParseLexemes(expr);

	if (!lexemes.empty())
	{
		if (lexemes[0])
		{
			tokens.emplace_back(TokenType::Operator, *lexemes[0]);
		}
		if (lexemes[1])
		{
			tokens.emplace_back(TokenType::Id, *lexemes[1]);
		}
		if (lexemes[2])
		{
			tokens.emplace_back(TokenType::Number, *lexemes[2]);
		}
		if (lexemes[3])
		{
			tokens.emplace_back(TokenType::Id, *lexemes[3]);
		}
		if (lexemes[4])
		{
			tokens.emplace_back(TokenType::BinaryOperator, *lexemes[4]);
		}
		if (lexemes[5])
		{
			tokens.emplace_back(TokenType::Id, *lexemes[5]);
		}
	}

	return tokens;
}

vector<optional<string>> CLexer::ParseLexemes(string const & expr) const
{
	m_expr = expr;
	m_begin = m_curr = m_expr.begin();
	vector<optional<string>> lexemes(6);

	for (size_t i = 0; i < 1; ++i)
	{
		if (!(lexemes[0] = LexemeOperator()))
		{
			break;
		}
		if (!(lexemes[1] = LexemeId()))
		{
			break;
		}
		if (!LexemeAssignment())
		{
			break;
		}
		if (!(lexemes[3] = LexemeId()))
		{
			if (!(lexemes[2] = LexemeNumber()))
			{
				break;
			}
		}
		if (!(lexemes[4] = LexemeBinaryOperator()))
		{
			break;
		}
		if (!(lexemes[5] = LexemeId()))
		{
			break;
		}
		SkipSpaces();
	}

	if (!IsFullParse())
	{
		lexemes.resize(0);
	}

	return lexemes;
}

static unordered_set<string> const operators{ "var", "let", "fn", "print", "printvars", "printfns" };

optional<string> CLexer::LexemeOperator() const
{
	SkipSpaces();

	m_curr = find_if_not(m_begin, m_expr.cend(),
		[](char ch) { return isalpha(ch); }
	);
	if (m_curr != m_begin)
	{
		auto it = operators.find(string(m_begin, m_curr));
		if (it != operators.end())
		{
			return *it;
		}
	}

	return none;
}

optional<string> CLexer::LexemeId() const
{
	SkipSpaces();

	if (!IsFullParse() && !isdigit(*m_begin))
	{
		m_curr = find_if_not(m_begin, m_expr.cend(),
			[](char ch) { return (isalnum(ch) || ch == '_'); }
		);
		if (m_curr != m_begin)
		{
			return string(m_begin, m_curr);
		}
	}

	return none;
}

optional<string> CLexer::LexemeNumber() const
{
	if (!IsFullParse() && (isdigit(*m_begin) || *m_begin == '-' || *m_begin == '.'))
	{
		m_curr = find_if_not(m_begin + 1, m_expr.cend(),
			[](char ch) { return (isdigit(ch) || ch == '.'); }
		);
		return string(m_begin, m_curr);
	}

	return none;
}

static unordered_set<string> const binaryOperators{ "+", "-", "*", "/" };

optional<string> CLexer::LexemeBinaryOperator() const
{
	SkipSpaces();

	if (!IsFullParse())
	{
		auto it = binaryOperators.find(string(m_begin, m_curr + 1));
		if (it != binaryOperators.end())
		{
			++m_curr;
			return *it;
		}
	}

	return none;
}

bool CLexer::LexemeAssignment() const
{
	SkipSpaces();

	if (!IsFullParse() && *m_begin == '=')
	{
		++m_curr;
		return true;
	}

	return false;
}

void CLexer::SkipSpaces() const
{
	m_begin = m_curr;
	m_curr = find_if_not(m_begin, m_expr.cend(),
		[](char ch) { return isspace(ch); }
	);
	m_begin = m_curr;
}

bool CLexer::IsFullParse() const
{
	return m_curr == m_expr.end();
}
