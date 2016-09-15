#include "stdafx.h"
#include "Selection.h"
#include "IShape.h"

CSelection::CSelection(std::shared_ptr<IShape> const & shape)
{
	if (shape->GetType() == ShapeType::Rectangle)
	{
		m_shape = std::make_shared<CRectangleShape>(shape->GetRect());
	}
	else if (shape->GetType() == ShapeType::Triangle)
	{
		m_shape = std::make_shared<CTriangleShape>(shape->GetRect());
	}
	else if (shape->GetType() == ShapeType::Ellipse)
	{
		m_shape = std::make_shared<CEllipseShape>(shape->GetRect());
	}
}

std::shared_ptr<IShape> CSelection::GetShape()
{
	return m_shape;
}

void CSelection::MoveControlPoint(SelectControlPointType /*type*/, CPoint const & /*coord*/)
{

}

void CSelection::SetRect(CRect const & rect)
{
	m_shape->SetRect(rect);
}

CRect CSelection::GetRect() const
{
	return m_shape->GetRect();
}

void CSelection::Commit() const
{
	m_shapeChange();
}

void CSelection::DoOnShapeChange(ShapeChangeSignal::slot_type const & handler)
{
	m_shapeChange.connect(handler);
}

#pragma warning(disable:4503)