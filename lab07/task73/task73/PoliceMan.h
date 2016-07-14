#pragma once
#include "PersonImpl.h"
#include "IPoliceMan.h"

class CPoliceMan : public CPersonImpl<IPoliceMan>
{
public:
	CPoliceMan(std::string const & name, std::string const & department);
	std::string GetDepartmentName() const override;

private:
	std::string m_department;
};
