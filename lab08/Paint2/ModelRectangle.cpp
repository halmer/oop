#include "stdafx.h"
#include "ModelRectangle.h"

CModelRectangle::CModelRectangle(CRect const & rect)
	: CShape(rect)
{
}

ShapeType CModelRectangle::GetType() const
{
	return ShapeType::Rectangle;
}
