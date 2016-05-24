#include "stdafx.h"
#include "SolidBody.h"

CSolidBody::CSolidBody(std::string const & type, std::string const & id, double density)
	: CBody(type, id)
	, m_density(density)
{
}

double CSolidBody::GetDensity() const
{
	return m_density;
}

double CSolidBody::GetMass() const
{
	return GetDensity() * GetVolume();
}
