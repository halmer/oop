#include "stdafx.h"
#include "ModelRectangle.h"

CModelRectangle::CModelRectangle(CRect const & rect)
	: CModelShape(rect)
{
}

ShapeType CModelRectangle::GetType() const
{
	return ShapeType::Rectangle;
}
