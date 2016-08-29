#pragma once
#include "Shape.h"

class CTriangle : public CShape
{
public:
	CTriangle(CRect const & rect);
	
	DrawCommand DrawShape() const override;
	bool IsPointInShape(CPoint const & point) const override;
	std::string GetType() const override;

private:
	mutable std::vector<CPoint> m_points;
};
