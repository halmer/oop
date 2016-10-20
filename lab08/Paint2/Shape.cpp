#include "stdafx.h"
#include "Shape.h"

CShape::CShape(CRect const & rect, ShapeType type, HANDLE bitmap /*= nullptr*/)
	: m_rect(rect)
	, m_type(type)
	, m_bitmap(bitmap)
{
}

CShape::~CShape()
{
	if (m_bitmap)
	{
		DeleteObject(m_bitmap);
	}
}

void CShape::SetRect(CRect const & rect)
{
	m_rect = rect;
	m_changeShape(this);
}

CRect CShape::GetRect() const
{
	return m_rect;
}

ShapeType CShape::GetType() const
{
	return m_type;
}

HANDLE CShape::GetBitmap() const
{
	return m_bitmap;
}

boost::signals2::connection CShape::DoOnShapeChange(ShapeChangeSignal::slot_type const & handler)
{
	return m_changeShape.connect(handler);
}

#pragma warning(disable:4503)
