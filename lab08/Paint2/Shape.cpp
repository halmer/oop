#include "stdafx.h"
#include "Shape.h"

CShape::CShape(CRect const & rect, ShapeType type)
	: m_rect(rect)
	, m_type(type)
{
}

void CShape::SetRect(CRect const & rect)
{
	m_rect = rect;
}

CRect CShape::GetRect() const
{
	return m_rect;
}

ShapeType CShape::GetType() const
{
	return m_type;
}
