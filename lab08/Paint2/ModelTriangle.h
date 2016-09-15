#pragma once
#include "Shape.h"

class CModelTriangle : public CShape
{
public:
	CModelTriangle(CRect const & rect);
	ShapeType GetType() const override;
};
