#include "stdafx.h"
#include "Calculator.h"

using namespace std;

CCalculator::CCalculator(istream & is, ostream & os)
	: m_in(is)
	, m_out(os)
{
}

void CCalculator::Run()
{
	for (string expr; getline(m_in, expr, '\n');)
	{
		if(!expr.empty())
		{
			auto command = m_parser.CreateCommand(expr);
			if (!command)
			{
				m_out << "invalid syntax\n";
				continue;
			}
			command(this);
		}
	}
}

void CCalculator::CreateVariable(string const & varId)
{
	if(!m_proc.CreateVar(varId))
	{ 
		m_out << varId << " already exists\n";
	}
}

void CCalculator::SetVariable(std::string const & varId, std::string const & id)
{
	if (!m_proc.SetVar(varId, id))
	{
		m_out << "invalid identifier\n";
	}
}

void CCalculator::SetVariable(std::string const & varId, double value)
{
	if (!m_proc.SetVar(varId, value))
	{
		m_out << "invalid identifier\n";
	}
}

void CCalculator::CreateFunction(string const & fnId, string const & id)
{
	if (!m_proc.CreateFn(fnId, id))
	{
		m_out << "invalid identifier\n";
	}
}


void CCalculator::CreateFunction(string const & fnId, string const & lhsId, string const & op, string const & rhsId)
{
	if (!m_proc.CreateFn(fnId, lhsId, op, rhsId))
	{
		m_out << "invalid identifier\n";
	}
}

void CCalculator::PrintId(string const & id) const
{
	auto value = m_proc.GetValue(id);
	if (value)
	{
		m_out << fixed << setprecision(2) << *value << endl;
	}
	else
	{
		m_out << id << " is not declared\n";
	}
}

void CCalculator::PrintVariables() const
{
	Print(m_proc.GetVarsValues());
}

void CCalculator::PrintFunctions() const
{
	Print(m_proc.GetFnsValues());
}

void CCalculator::Print(std::map<std::string, double> const & values) const
{
	for (auto const & item : values)
	{
		m_out << item.first << ":" << fixed << setprecision(2) << item.second << endl;
	}
}
