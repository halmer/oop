#include "stdafx.h"
#include "Sphere.h"

CSphere::CSphere(std::string const & id, double density, double radius)
	: CSolidBody("Sphere", id, density)
	, m_radius(radius)
{
}

double CSphere::GetRadius() const
{
	return m_radius;
}

double CSphere::GetVolume() const
{
	return (pow(m_radius, 3) * M_PI) * 4 / 3;
}

void CSphere::AppendProperties(std::ostream & strm) const
{
	strm << "\tradius = " << GetRadius() << std::endl;
}
