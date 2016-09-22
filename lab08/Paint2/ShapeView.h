#pragma once
#include "IShapeView.h"
#include "IShape.h"

class CShapeView : public IShapeView
{
public:
	CShapeView(CRect const & rect, ShapeType type);
	void SetRect(CRect const & rect) override;
	void DrawShape(CDC * pDC) override;
	bool IsPointInShape(CPoint const & point) const override;

private:
	CRect m_rect;
	ShapeType m_type;
};
