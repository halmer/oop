#include "stdafx.h"
#include "ModelTriangle.h"

CModelTriangle::CModelTriangle(CRect const & rect)
	: CModelShape(rect)
{
}

ShapeType CModelTriangle::GetType() const
{
	return ShapeType::Triangle;
}
