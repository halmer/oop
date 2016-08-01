#pragma once

enum class TokenType
{
	Operator,
	BinaryOperator,
	Id,
	Number,
};

struct Token
{
	Token(TokenType type, std::string const & value);

	TokenType type;
	std::string value;
};

class CLexer : boost::noncopyable
{
public:
	CLexer();

	std::vector<Token> GetTokensRgx(std::string const & expr) const;
	std::vector<Token> GetTokens(std::string const & expr) const;
	
private:
	std::vector<boost::optional<std::string>> ParseLexemes(std::string const & expr) const;
	boost::optional<std::string> LexemeOperator() const;
	boost::optional<std::string> LexemeId() const;
	boost::optional<std::string> LexemeNumber() const;
	boost::optional<std::string> LexemeBinaryOperator() const;
	bool LexemeAssignment() const;
	void SkipSpaces() const;
	bool IsFullParse() const;
	
	std::regex m_rgx;
	mutable std::string m_expr;
	mutable std::string::const_iterator m_begin;
	mutable std::string::const_iterator m_curr;
};
