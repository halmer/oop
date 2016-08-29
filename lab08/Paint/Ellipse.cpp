#include "stdafx.h"
#include "Ellipse.h"

using namespace std;
using namespace std::placeholders;

CEllipse::CEllipse(CRect const & rect)
	: CShape(rect)
{
}

DrawCommand CEllipse::DrawShape() const
{
	return bind(static_cast<BOOL(CDC::*)(LPCRECT)>(&CDC::Ellipse), _1, cref(m_rect));
}

bool CEllipse::IsPointInShape(CPoint const & point) const
{
	CRgn rgn;
	rgn.CreateEllipticRgnIndirect(&m_rect);
	
	return rgn.PtInRegion(point) != 0;
}

string CEllipse::GetType() const
{
	return "Ellipse";
}
