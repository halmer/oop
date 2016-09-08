#pragma once
#include "Shape.h"

class CEllipse : public CShape
{
public:
	CEllipse(CRect const & rect);
	
	DrawCommand DrawShape() const override;
	bool IsPointInShape(CPoint const & point) const override;
	ShapeType GetType() const override;
	std::shared_ptr<CShape> Clone() const override;
};
