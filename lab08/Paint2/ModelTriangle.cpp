#include "stdafx.h"
#include "ModelTriangle.h"

CModelTriangle::CModelTriangle(CRect const & rect)
	: CShape(rect)
{
}

ShapeType CModelTriangle::GetType() const
{
	return ShapeType::Triangle;
}
