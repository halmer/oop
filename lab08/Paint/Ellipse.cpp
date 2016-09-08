#include "stdafx.h"
#include "Ellipse.h"

using namespace std;

CEllipse::CEllipse(CRect const & rect)
	: CShape(rect)
{
}

DrawCommand CEllipse::DrawShape() const
{
	return bind(static_cast<BOOL(CDC::*)(LPCRECT)>(&CDC::Ellipse), placeholders::_1, cref(m_rect));
}

bool CEllipse::IsPointInShape(CPoint const & point) const
{
	CRgn rgn;
	rgn.CreateEllipticRgnIndirect(&m_rect);
	
	return rgn.PtInRegion(point) != 0;
}

ShapeType CEllipse::GetType() const
{
	return ShapeType::Ellipse;
}

std::shared_ptr<CShape> CEllipse::Clone() const
{
	return make_shared<CEllipse>(*this);
}
