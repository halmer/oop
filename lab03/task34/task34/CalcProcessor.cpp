#include "stdafx.h"
#include "CalcProcessor.h"

using namespace std;

const size_t NUMBUCKETS = 10;
const bool VARS = false;
const bool FNS = true;

CCalcProcessor::CCalcProcessor()
	: m_symbolTable(NUMBUCKETS)
{
}

CCalcProcessor::Data::Data(idIterator lhs, idIterator rhs, function<double(double, double)> fn, boost::optional<double> value)
	: lhs(lhs)
	, rhs(rhs)
	, fn(fn)
	, value(value)
{
}

bool CCalcProcessor::CreateVar(string const & varId)
{
	if (m_symbolTable.find(varId) != m_symbolTable.end())
	{
		return false;
	}

	m_symbolTable.emplace(varId, Data());
	return true;
}

bool CCalcProcessor::SetVar(string const & varId, string const & id)
{
	if (m_symbolTable.find(id) == m_symbolTable.end())
	{
		return false;
	}

	return SetVar(varId, *GetValue(id));
}

bool CCalcProcessor::SetVar(string const & varId, double value)
{
	boost::optional<double> val = value;
	if (isnan(*val))
	{
		val.reset();
	}

	auto it = m_symbolTable.find(varId);
	if (it != m_symbolTable.end())
	{
		if (it->second.fn)
		{
			return false;
		}
		it->second.value = val;
	}
	else
	{
		m_symbolTable.emplace(varId, Data({}, {}, {}, val));
	}
	
	return true;
}

bool CCalcProcessor::CreateFn(string const & fnId, string const & id)
{
	if (m_symbolTable.find(fnId) != m_symbolTable.end())
	{
		return false;
	}

	auto itId = m_symbolTable.find(id);
	if (itId == m_symbolTable.end())
	{
		return false;
	}

	m_symbolTable.emplace(fnId, Data(itId, itId, [](double x, double) { return x; }, {}));
	return true;
}

bool CCalcProcessor::CreateFn(string const & fnId, string const & lhsId, string const & op, string const & rhsId)
{
	if (m_symbolTable.find(fnId) != m_symbolTable.end())
	{
		return false;
	}

	auto itLhsId = m_symbolTable.find(lhsId);
	if (itLhsId == m_symbolTable.end())
	{
		return false;
	}

	auto itRhsId = m_symbolTable.find(rhsId);
	if (itRhsId == m_symbolTable.end())
	{
		return false;
	}

	if (op == "+")
	{
		m_symbolTable.emplace(fnId, Data(itLhsId, itRhsId, plus<double>(), {}));
	}
	else if (op == "-")
	{
		m_symbolTable.emplace(fnId, Data(itLhsId, itRhsId, minus<double>(), {}));
	}
	else if (op == "*")
	{
		m_symbolTable.emplace(fnId, Data(itLhsId, itRhsId, multiplies<double>(), {}));
	}
	else if (op == "/")
	{
		m_symbolTable.emplace(fnId, Data(itLhsId, itRhsId, divides<double>(), {}));
	}

	return true;
}

boost::optional<double>CCalcProcessor::GetValue(string const & id) const
{
	auto it = m_symbolTable.find(id);
	if (it == m_symbolTable.end())
	{
		return boost::none;
	}

	if (it->second.fn)
	{
		CalculateFn(id);
	}

	if (it->second.value)
	{
		return *it->second.value;
	}

	return numeric_limits<double>::quiet_NaN();
}

map<string, double> CCalcProcessor::GetVarsValues() const
{
	return GetValues(VARS);
}

std::map<std::string, double> CCalcProcessor::GetFnsValues() const
{
	return GetValues(FNS);
}

std::map<std::string, double> CCalcProcessor::GetValues(bool isGetFns) const
{
	if (isGetFns)
	{
		CalculateFns();
	}
	
	map<string, double> values;
	for (auto const & item : m_symbolTable)
	{
		if ((item.second.fn && isGetFns) || (!item.second.fn && !isGetFns))
		{
			if (item.second.value)
			{
				values.emplace(item.first, *item.second.value);
			}
			else
			{
				values.emplace(item.first, numeric_limits<double>::quiet_NaN());
			}
		}
	}

	return values;
}

void CCalcProcessor::CalculateFn(string const & fnId) const
{
	unordered_set<string> ids(m_symbolTable.size());

	Calculate(ids, fnId);
}

void CCalcProcessor::CalculateFns() const
{
	unordered_set<string> ids(m_symbolTable.size());

	for (auto it = m_symbolTable.begin(); it != m_symbolTable.end(); ++it)
	{
		it = find_if(it, m_symbolTable.end(),
			[&ids](auto id) { return id.second.fn ? ids.find(id.first) == ids.end() : false; }
		);

		if (it == m_symbolTable.end())
		{
			break;
		}

		ids.emplace(it->first);
		Calculate(ids, it->first);
	}
}

void CCalcProcessor::Calculate(unordered_set<string> & ids, string const & id) const
{
	vector<Data*> idsData;
	idsData.reserve(m_symbolTable.size());
	idsData.push_back(&m_symbolTable[id]);

	for (auto it = idsData.begin(); it != idsData.end(); ++it)
	{
		if ((*it)->lhs->second.fn)
		{
			string lhsId = (*it)->lhs->first;
			auto itLhsId = ids.find(lhsId);
			if (itLhsId == ids.end() || lhsId != *itLhsId)
			{
				ids.emplace(lhsId);
				idsData.push_back(&(*it)->lhs->second);
			}
		}

		if ((*it)->rhs->second.fn)
		{
			string rhsId = (*it)->rhs->first;
			auto itRhsId = ids.find(rhsId);
			if (itRhsId == ids.end() || rhsId != *itRhsId)
			{
				ids.emplace(rhsId);
				idsData.push_back(&(*it)->rhs->second);
			}
		}
	}

	for (auto & item : boost::adaptors::reverse(idsData))
	{
		auto lhsValue = item->lhs->second.value;
		auto rhsValue = item->rhs->second.value;
		if (lhsValue && rhsValue)
		{
			item->value = item->fn(*lhsValue, *rhsValue);
		}
	}
}
