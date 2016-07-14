#pragma once

template <class Base>
class CPersonImpl : public Base
{
public:
	CPersonImpl(std::string const & name)
		: m_name(name)
	{}

	std::string GetName() const override
	{
		return m_name;
	}

private:
	std::string m_name;
};
