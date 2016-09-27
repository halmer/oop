#pragma once
#include "IShapeView.h"

class CShapeView : public IShapeView
{
public:
	CShapeView(CRect const & rect, ShapeViewType type);
	void SetRect(CRect const & rect) override;
	void DrawShape(CDC * pDC) override;
	bool IsPointInShape(CPoint const & point) const override;

private:
	CRect m_rect;
	ShapeViewType m_type;
};
