#include "stdafx.h"
#include "Selection.h"
#include "Shape.h"

CSelection::CSelection(std::shared_ptr<IShape> const & shape)
{
	m_shape = std::make_shared<CShape>(shape->GetRect(), shape->GetType());
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
