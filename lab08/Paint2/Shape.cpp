#include "stdafx.h"
#include "Shape.h"

CShape::CShape(CRect const & rect)
	: m_rect(rect)
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
