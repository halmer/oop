#pragma once
#include "Lexer.h"

class CCalculator;
using CommandCalculator = std::function<void (CCalculator*)>;

class CParser : boost::noncopyable
{
public:
	CommandCalculator CreateCommand(std::string const & expr) const;
	
private:
	CommandCalculator OperatorVar() const;
	CommandCalculator OperatorLet() const;
	CommandCalculator OperatorFn() const;
	CommandCalculator OperatorPrint() const;
	CommandCalculator OperatorPrintvars() const;
	CommandCalculator OperatorPrintfns() const;

	CLexer m_lexer;
	mutable std::vector<Token> m_tokens;
};
