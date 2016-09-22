#pragma once
#include "IShape.h"

class CShape : public IShape
{
public:
	CShape(CRect const & rect, ShapeType type);
	void SetRect(CRect const & rect) final;
	CRect GetRect() const final;
	ShapeType GetType() const final;

	boost::signals2::connection DoOnShapeChange(ShapeChangeSignal::slot_type const & handler) final;

private:
	CRect m_rect;
	ShapeType m_type;
	ShapeChangeSignal m_changeShape;
};
