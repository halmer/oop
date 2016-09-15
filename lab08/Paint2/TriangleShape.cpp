#include "stdafx.h"
#include "TriangleShape.h"

CTriangleShape::CTriangleShape(CRect const & rect)
	: CShape(rect)
{
}

ShapeType CTriangleShape::GetType() const
{
	return ShapeType::Triangle;
}
