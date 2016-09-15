#include "stdafx.h"
#include "EllipseShape.h"

CEllipseShape::CEllipseShape(CRect const & rect)
	: CShape(rect)
{
}

ShapeType CEllipseShape::GetType() const
{
	return ShapeType::Ellipse;
}
