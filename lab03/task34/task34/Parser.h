#pragma once
#include "Lexer.h"
#include "ICommandContext.h"

using CalculatorCommand = std::function<void(ICommandContext*)>;

class CParser : boost::noncopyable
{
public:
	CalculatorCommand CreateCommand(std::string const & expr) const;
	
private:
	CalculatorCommand OperatorVar() const;
	CalculatorCommand OperatorLet() const;
	CalculatorCommand OperatorFn() const;
	CalculatorCommand OperatorPrint() const;
	CalculatorCommand OperatorPrintvars() const;
	CalculatorCommand OperatorPrintfns() const;

	CLexer m_lexer;
	mutable std::vector<Token> m_tokens;
};
