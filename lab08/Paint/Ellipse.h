#pragma once
#include "Shape.h"

class CEllipse : public CShape
{
public:
	CEllipse(CRect const & rect);
	
	DrawCommand DrawShape() const override;
	bool IsPointInShape(CPoint const & point) const override;
	std::string GetType() const override;
};
