#pragma once
#include "SolidBody.h"

class CParallelepiped final : public CSolidBody
{
public:
	CParallelepiped(std::string const & id, double density, double width, double height, double depth);

	double GetWidth() const;
	double GetHeight() const;
	double GetDepth() const;
	double GetVolume() const override;

protected:
	void AppendProperties(std::ostream & strm) const override;

private:
	double m_width;
	double m_height;
	double m_depth;
};
