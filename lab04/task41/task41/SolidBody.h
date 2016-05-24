#pragma once
#include "Body.h"

class CSolidBody : public CBody
{
public:
	CSolidBody(std::string const & type, std::string const & id, double density);

	double GetDensity() const override;
	double GetMass() const override;

private:
	double m_density;
};
