#include "stdafx.h"
#include "PoliceMan.h"

CPoliceMan::CPoliceMan(std::string const & name, std::string const & department)
	: CPersonImpl(name)
	, m_department(department)
{
}

std::string CPoliceMan::GetDepartmentName() const
{
	return m_department;
}
