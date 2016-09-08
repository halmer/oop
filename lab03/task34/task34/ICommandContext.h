#pragma once

class ICommandContext
{
public:
	virtual void CreateVariable(std::string const & varId) = 0;
	virtual void SetVariable(std::string const & varId, std::string const & id) = 0;
	virtual void SetVariable(std::string const & varId, double value) = 0;
	virtual void CreateFunction(std::string const & fnId, std::string const & id) = 0;
	virtual void CreateFunction(std::string const & fnId, std::string const & lhsId, std::string const & op, std::string const & rhsId) = 0;
	virtual void PrintId(std::string const & id) const = 0;
	virtual void PrintVariables() const = 0;
	virtual void PrintFunctions() const = 0;

	virtual ~ICommandContext() = default;
};
