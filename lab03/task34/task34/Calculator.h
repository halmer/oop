#pragma once
#include "Parser.h"
#include "CalcProcessor.h"
#include "ICommandContext.h"

class CCalculator : public ICommandContext, private boost::noncopyable
{
public:
	CCalculator(std::istream & is, std::ostream & os);

	void Run();
private:
	void CreateVariable(std::string const & varId) override;
	void SetVariable(std::string const & varId, std::string const & id) override;
	void SetVariable(std::string const & varId, double value) override;
	void CreateFunction(std::string const & fnId, std::string const & id) override;
	void CreateFunction(std::string const & fnId, std::string const & lhsId, std::string const & op, std::string const & rhsId) override;
	void PrintId(std::string const & id) const override;
	void PrintVariables() const override;
	void PrintFunctions() const override;
	void Print(std::map<std::string, double> const & values) const;

	CParser m_parser;
	CCalcProcessor m_proc;
	std::istream & m_in;
	std::ostream & m_out;
};
