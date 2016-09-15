#pragma once
#include "ModelShape.h"

class CModelRectangle : public CModelShape
{
public:
	CModelRectangle(CRect const & rect);
	ShapeType GetType() const override;
};
