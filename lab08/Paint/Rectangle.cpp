#include "stdafx.h"
#include "Rectangle.h"

using namespace std;

CRectangle::CRectangle(CRect const & rect)
	: CShape(rect)
{
}

DrawCommand CRectangle::DrawShape() const
{
	return bind(static_cast<BOOL(CDC::*)(LPCRECT)>(&CDC::Rectangle), placeholders::_1, cref(m_rect));
}

bool CRectangle::IsPointInShape(CPoint const & point) const
{
	CRgn rgn;
	rgn.CreateRectRgnIndirect(&m_rect);

	return rgn.PtInRegion(point) != 0;
}

ShapeType CRectangle::GetType() const
{
	return ShapeType::Rectangle;
}

std::shared_ptr<CShape> CRectangle::Clone() const
{
	return make_shared<CRectangle>(*this);
}
