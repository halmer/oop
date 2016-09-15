#pragma once
#include "Shape.h"

class CEllipseShape : public CShape
{
public:
	CEllipseShape(CRect const & rect);
	ShapeType GetType() const override;
};
