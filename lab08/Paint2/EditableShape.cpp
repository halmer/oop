#include "stdafx.h"
#include "EditableShape.h"

CEditableShape::CEditableShape(CRect const & rect, ShapeType type)
	: m_rect(rect)
	, m_type(type)
{
}

void CEditableShape::SetRect(CRect const & rect)
{
	m_rect = rect;
}

void CEditableShape::Offset(CPoint const & delta, OffsetType type)
{
	switch (type)
	{
	case OffsetType::AllSide:
		m_rect.OffsetRect(delta);
		break;
	case OffsetType::TopLeft:
		m_rect.TopLeft() += delta;
		break;
	case OffsetType::TopRight:
		m_rect.top += delta.y;
		m_rect.right += delta.x;
		break;
	case OffsetType::BottomLeft:
		m_rect.bottom += delta.y;
		m_rect.left += delta.x;
		break;
	case OffsetType::BottomRight:
		m_rect.BottomRight() += delta;
		break;
	default:
		break;
	}
}

CRect CEditableShape::GetRect() const
{
	return m_rect;
}

ShapeType CEditableShape::GetType() const
{
	return m_type;
}

void CEditableShape::Commit() const
{
	m_shapeChange(this);
}

void CEditableShape::DoOnShapeChange(ShapeChangeSignal::slot_type const & handler)
{
	m_shapeChange.connect(handler);
}

#pragma warning(disable:4503)
