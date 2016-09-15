#pragma once
#include "IEditableShape.h"

class CEditableShape : public IEditableShape
{
public:
	CEditableShape(CRect const & rect);
	void SetRect(CRect const & rect) override;
	CRect GetRect() const override;
	void Commit() const override;

	void DoOnShapeChange(ShapeChangeSignal::slot_type const & handler) override;

private:
	CRect m_rect;
	ShapeChangeSignal m_shapeChange;
};
