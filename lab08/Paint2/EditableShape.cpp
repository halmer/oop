#include "stdafx.h"
#include "EditableShape.h"

CEditableShape::CEditableShape(CRect const & rect)
	: m_rect(rect)
{
}

void CEditableShape::SetRect(CRect const & rect)
{
	m_rect = rect;
}

CRect CEditableShape::GetRect() const
{
	return m_rect;
}

void CEditableShape::Commit() const
{
	m_shapeChange();
}

void CEditableShape::DoOnShapeChange(ShapeChangeSignal::slot_type const & handler)
{
	m_shapeChange.connect(handler);
}
