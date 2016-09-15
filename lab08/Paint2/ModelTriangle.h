#pragma once
#include "ModelShape.h"

class CModelTriangle : public CModelShape
{
public:
	CModelTriangle(CRect const & rect);
	ShapeType GetType() const override;
};
