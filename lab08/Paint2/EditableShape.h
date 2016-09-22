#pragma once
#include "IEditableShape.h"

class CEditableShape : public IEditableShape
{
public:
	CEditableShape(CRect const & rect, ShapeType type);
	void SetRect(CRect const & rect) override;
	void Offset(CPoint const & delta, OffsetType type) override;
	CRect GetRect() const override;
	ShapeType GetType() const override;
	void Commit() const override;

	void DoOnShapeChange(ShapeChangeSignal::slot_type const & handler) override;

private:
	CRect m_rect;
	ShapeType m_type;
	ShapeChangeSignal m_shapeChange;
};
