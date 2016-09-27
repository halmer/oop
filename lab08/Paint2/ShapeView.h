#pragma once
#include "IShapeView.h"

class CShapeView : public IShapeView
{
public:
	CShapeView(CRect const & rect, ShapeViewType type);
	void SetRect(CRect const & rect) override;
	void DrawShape(CDC * pDC) override;

	bool HandleMouseDown(CPoint const & point);
	void HandleMouseUp(CPoint const & point);
	void HandleDrag(CPoint const& currentPoint, CPoint const& oldPoint);

	Connection DoOnMousePress(const MousePressSignal::slot_type & handler) override;
	Connection DoOnMouseRelease(const MouseReleaseSignal::slot_type & handler) override;
	Connection DoOnMouseDrag(const MouseDragSignal::slot_type & handler) override;

private:
	bool HitTest(CPoint const & point) const;

private:
	CRect m_rect;
	ShapeViewType m_type;
	MousePressSignal m_mousePressSignal;
	MouseReleaseSignal m_mouseReleaseSignal;
	MouseDragSignal m_mouseDragSignal;
};
