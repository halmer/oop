#include "stdafx.h"
#include "Triangle.h"

using namespace std;

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

	return bind(&CDC::Polygon, placeholders::_1, m_points.data(), boost::numeric_cast<int>(m_points.size()));
}

bool CTriangle::IsPointInShape(CPoint const & point) const
{
	m_points.clear();
	m_points.emplace_back(m_rect.left, m_rect.bottom);
	m_points.emplace_back((m_rect.left + m_rect.right) / 2, m_rect.top);
	m_points.emplace_back(m_rect.right, m_rect.bottom);
	
	CRgn rgn;
	rgn.CreatePolygonRgn(m_points.data(), boost::numeric_cast<int>(m_points.size()), WINDING);

	return rgn.PtInRegion(point) != 0;
}

ShapeType CTriangle::GetType() const
{
	return ShapeType::Triangle;
}

std::shared_ptr<CShape> CTriangle::Clone() const
{
	return make_shared<CTriangle>(*this);
}
