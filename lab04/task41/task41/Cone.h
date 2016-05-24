#pragma once
#include "SolidBody.h"

class CCone final : public CSolidBody
{
public:
	CCone(std::string const & id, double density, double radius, double height);

	double GetRadius() const;
	double GetHeight() const;
	double GetVolume() const override;

protected:
	void AppendProperties(std::ostream & strm) const override;

private:
	double m_radius;
	double m_height;
};
