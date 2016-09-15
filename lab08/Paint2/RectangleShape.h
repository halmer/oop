#pragma once
#include "Shape.h"

class CRectangleShape : public CShape
{
public:
	CRectangleShape(CRect const & rect);
	ShapeType GetType() const override;
};
