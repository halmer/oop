#pragma once
#include "Shape.h"

class CTriangleShape : public CShape
{
public:
	CTriangleShape(CRect const & rect);
	ShapeType GetType() const override;
};
