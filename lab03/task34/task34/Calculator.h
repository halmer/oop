#pragma once
#include "Parser.h"
#include "CalcProcessor.h"

class CCalculator : boost::noncopyable
{
public:
	CCalculator(std::istream & is, std::ostream & os);

	void Run();
	void CreateVariable(std::string const & varId);
	void SetVariable(std::string const & varId, std::string const & id);
	void SetVariable(std::string const & varId, double value);
	void CreateFunction(std::string const & fnId, std::string const & id);
	void CreateFunction(std::string const & fnId, std::string const & lhsId, std::string const & op, std::string const & rhsId);
	void PrintId(std::string const & id) const;
	void PrintVariables() const;
	void PrintFunctions() const;
	
private:
	void Print(std::map<std::string, double> const & values) const;

	CParser m_parser;
	CCalcProcessor m_proc;
	std::istream & m_in;
	std::ostream & m_out;
};
