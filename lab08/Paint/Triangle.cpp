#include "stdafx.h"
#include "Triangle.h"

using namespace std;
using namespace std::placeholders;

CTriangle::CTriangle(CRect const & rect)
	: CShape(rect)
{
}

DrawCommand CTriangle::DrawShape() const
{
	m_points.clear();
	m_points.emplace_back(m_rect.left, m_rect.bottom);
	m_points.emplace_back((m_rect.left + m_rect.right) / 2, m_rect.top);
	m_points.emplace_back(m_rect.right, m_rect.bottom);

	return bind(&CDC::Polygon, _1, m_points.data(), m_points.size());
}

bool CTriangle::IsPointInShape(CPoint const & point) const
{
	m_points.clear();
	m_points.emplace_back(m_rect.left, m_rect.bottom);
	m_points.emplace_back((m_rect.left + m_rect.right) / 2, m_rect.top);
	m_points.emplace_back(m_rect.right, m_rect.bottom);
	
	CRgn rgn;
	rgn.CreatePolygonRgn(m_points.data(), m_points.size(), WINDING);

	return rgn.PtInRegion(point) != 0;
}

string CTriangle::GetType() const
{
	return "Triangle";
}
