#include "stdafx.h"
#include "Rectangle.h"

using namespace std;
using namespace std::placeholders;

CRectangle::CRectangle(CRect const & rect)
	: CShape(rect)
{
}

DrawCommand CRectangle::DrawShape() const
{
	return bind(static_cast<BOOL(CDC::*)(LPCRECT)>(&CDC::Rectangle), _1, cref(m_rect));
}

bool CRectangle::IsPointInShape(CPoint const & point) const
{
	CRgn rgn;
	rgn.CreateRectRgnIndirect(&m_rect);

	return rgn.PtInRegion(point) != 0;
}

string CRectangle::GetType() const
{
	return "Rectangle";
}
