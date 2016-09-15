#pragma once
#include "Shape.h"

class CModelRectangle : public CShape
{
public:
	CModelRectangle(CRect const & rect);
	ShapeType GetType() const override;
};
