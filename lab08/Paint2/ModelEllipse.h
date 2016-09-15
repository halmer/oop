#pragma once
#include "Shape.h"

class CModelEllipse : public CShape
{
public:
	CModelEllipse(CRect const & rect);
	ShapeType GetType() const override;
};
