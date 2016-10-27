#include "stdafx.h"
#include "EditableShape.h"
#include "IHistory.h"

CEditableShape::CEditableShape(std::shared_ptr<IShape> const & shape, IHistory & history)
	: m_rect(shape->GetRect())
	, m_shape(shape)
	, m_history(history)
{
	m_connection = shape->DoOnShapeChange([this](IShape const * shape) {
		m_rect = shape->GetRect();
		m_shapeChange(this);
	});
}

void CEditableShape::SetRect(CRect const & rect)
{
	if (m_rect != rect)
	{
		m_rect = rect;
		m_shapeChange(this);
	}
}

void CEditableShape::Offset(CPoint const & delta, OffsetType type)
{
	CRect rect = m_rect;

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

	if (m_rect != rect)
	{
		m_shapeChange(this);
	}
}

CRect CEditableShape::GetRect() const
{
	return m_rect;
}

ShapeType CEditableShape::GetType() const
{
	return m_shape->GetType();
}

HANDLE CEditableShape::GetBitmap() const
{
	return m_shape->GetBitmap();
}

void CEditableShape::Commit()
{
	auto oldRect = m_shape->GetRect();
	if (m_rect != oldRect)
	{
		m_history.AddCommandAndExecute({
			std::bind(&IShape::SetRect, m_shape, m_rect),
			std::bind(&IShape::SetRect, m_shape, oldRect)
		});
	}
}

boost::signals2::connection CEditableShape::DoOnShapeChange(ShapeChangeSignal::slot_type const & handler)
{
	return m_shapeChange.connect(handler);
}

#pragma warning(disable:4503)
