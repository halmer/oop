#include "stdafx.h"
#include "Shape.h"

CShape::CShape(CRect const & rect)
	: m_rect(rect)
{
}

CShape::~CShape()
{
}

void CShape::Offset(CPoint const & delta, OffsetType type)
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

CRect CShape::GetFrameRect() const
{
	return m_rect;
}

CPoint CShape::GetBottomRight() const
{
	CRect rect = m_rect;
	rect.NormalizeRect();
	return rect.BottomRight();
}
