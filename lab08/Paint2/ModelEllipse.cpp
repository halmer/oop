#include "stdafx.h"
#include "ModelEllipse.h"

CModelEllipse::CModelEllipse(CRect const & rect)
	: CShape(rect)
{
}

ShapeType CModelEllipse::GetType() const
{
	return ShapeType::Ellipse;
}
