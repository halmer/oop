#include "stdafx.h"
#include "Parser.h"
#include "Calculator.h"

using namespace std;
using namespace std::placeholders;

CommandCalculator CParser::CreateCommand(string const & expr) const
{
	m_tokens = m_lexer.GetTokens(expr);

	if (!m_tokens.empty() && m_tokens[0].type == TokenType::Operator)
	{
		if (m_tokens[0].value == "var")
		{
			return OperatorVar();
		}
		if (m_tokens[0].value == "let")
		{
			return OperatorLet();
		}
		if (m_tokens[0].value == "fn")
		{
			return OperatorFn();
		}
		if (m_tokens[0].value == "print")
		{
			return OperatorPrint();
		}
		if (m_tokens[0].value == "printvars")
		{
			return OperatorPrintvars();
		}
		if (m_tokens[0].value == "printfns")
		{
			return OperatorPrintfns();
		}
	}

	return {};
}

CommandCalculator CParser::OperatorVar() const
{
	if (m_tokens.size() == 2 && m_tokens[1].type == TokenType::Id)
	{
		return bind(&CCalculator::CreateVariable, _1, ref(m_tokens[1].value));
	}

	return {};
}

CommandCalculator CParser::OperatorLet() const
{
	if (m_tokens.size() == 3 && m_tokens[1].type == TokenType::Id)
	{
		if (m_tokens[2].type == TokenType::Id)
		{
			return bind(static_cast<void (CCalculator::*)(string const &, string const &)>(&CCalculator::SetVariable),
				_1, ref(m_tokens[1].value), ref(m_tokens[2].value));
		}
		if (m_tokens[2].type == TokenType::Number)
		{
			return bind(static_cast<void (CCalculator::*)(string const &, double)>(&CCalculator::SetVariable),
				_1, ref(m_tokens[1].value), stod(m_tokens[2].value));
		}
	}

	return {};
}

CommandCalculator CParser::OperatorFn() const
{
	if (m_tokens.size() == 3
		&& m_tokens[1].type == TokenType::Id
		&& m_tokens[2].type == TokenType::Id)
	{
		return bind(static_cast<void (CCalculator::*)(string const &, string const &)>(&CCalculator::CreateFunction),
			_1, ref(m_tokens[1].value), ref(m_tokens[2].value));
	}
	if (m_tokens.size() == 5
		&& m_tokens[1].type == TokenType::Id
		&& m_tokens[2].type == TokenType::Id 
		&& m_tokens[3].type == TokenType::BinaryOperator
		&& m_tokens[4].type == TokenType::Id)
	{
		return bind(static_cast<void (CCalculator::*)(string const &, string const &, string const &, string const &)>(&CCalculator::CreateFunction),
			_1, ref(m_tokens[1].value), ref(m_tokens[2].value), ref(m_tokens[3].value), ref(m_tokens[4].value));
	}

	return {};
}

CommandCalculator CParser::OperatorPrint() const
{
	if (m_tokens.size() == 2 && m_tokens[1].type == TokenType::Id)
	{
		return bind(&CCalculator::PrintId, _1, ref(m_tokens[1].value));
	}

	return {};
}

CommandCalculator CParser::OperatorPrintvars() const
{
	if (m_tokens.size() == 1)
	{
		return bind(&CCalculator::PrintVariables, _1);
	}

	return {};
}

CommandCalculator CParser::OperatorPrintfns() const
{
	if (m_tokens.size() == 1)
	{
		return bind(&CCalculator::PrintFunctions, _1);
	}

	return {};
}
