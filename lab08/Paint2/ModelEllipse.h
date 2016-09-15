#pragma once
#include "ModelShape.h"

class CModelEllipse : public CModelShape
{
public:
	CModelEllipse(CRect const & rect);
	ShapeType GetType() const override;
};
