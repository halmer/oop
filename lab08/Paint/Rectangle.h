#pragma once
#include "Shape.h"

class CRectangle : public CShape
{
public:
	CRectangle(CRect const & rect);

	DrawCommand DrawShape() const override;
	bool IsPointInShape(CPoint const & point) const override;
	ShapeType GetType() const override;
	std::shared_ptr<CShape> Clone() const override;
};
