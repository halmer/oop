#pragma once
#include "IShape.h"

class CShape : public IShape
{
public:
	CShape(CRect const & rect, ShapeType type, HANDLE bitmap = nullptr);
	~CShape();
	void SetRect(CRect const & rect) final;
	CRect GetRect() const final;
	ShapeType GetType() const final;
	HANDLE GetBitmap() const final;

	boost::signals2::connection DoOnShapeChange(ShapeChangeSignal::slot_type const & handler) final;

private:
	CRect m_rect;
	ShapeType m_type;
	HANDLE m_bitmap;
	ShapeChangeSignal m_changeShape;
};
