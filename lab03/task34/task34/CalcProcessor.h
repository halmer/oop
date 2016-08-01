#pragma once

class CCalcProcessor : boost::noncopyable
{
public:
	CCalcProcessor();

	bool CreateVar(std::string const & varId);
	bool SetVar(std::string const & varId, std::string const & id);
	bool SetVar(std::string const & varId, double value);
	bool CreateFn(std::string const & fnId, std::string const & id);
	bool CreateFn(std::string const & fnId, std::string const & lhsId, std::string const & op, std::string const & rhsId);
	boost::optional<double> GetValue(std::string const & id) const;
	std::map<std::string, double> GetVarsValues() const;
	std::map<std::string, double> GetFnsValues() const;

private:
	struct Data
	{
		using idIterator = std::unordered_map<std::string, Data>::iterator;
		Data() = default;
		Data(idIterator lhs, idIterator rhs, std::function<double(double, double)> fn, boost::optional<double> value);
			
		idIterator lhs;
		idIterator rhs;
		std::function<double(double, double)> fn;
		boost::optional<double> value;
	};
	
	std::map<std::string, double> GetValues(bool isGetFns) const;
	void CalculateFn(std::string const & fnId) const;
	void CalculateFns() const;
	void Calculate(std::unordered_set<std::string> & ids, std::string const & id) const;
	
	mutable std::unordered_map<std::string, Data> m_symbolTable;
};
