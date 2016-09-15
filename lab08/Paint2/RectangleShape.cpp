#include "stdafx.h"
#include "RectangleShape.h"

CRectangleShape::CRectangleShape(CRect const & rect)
	: CShape(rect)
{
}

ShapeType CRectangleShape::GetType() const
{
	return ShapeType::Rectangle;
}
