#pragma once
#include "IShapeView.h"

class CShapeView : public IShapeView
{
public:
	CShapeView(CRect const & rect, ShapeViewType type);
	void SetRect(CRect const & rect) override;
	CRect GetRect() const;
	void DrawShape(CDC * pDC) override;

	bool HandleMouseDown(CPoint const & point);
	void HandleMouseUp(CPoint const & point);
	void HandleDrag(CPoint const & currentPoint, CPoint const & oldPoint);
	void HandleResizeShape(CRect const & rect);
	void HandleDeleteShape();

	Connection DoOnMousePress(MousePressSignal::slot_type const & handler) override;
	Connection DoOnMouseRelease(MouseReleaseSignal::slot_type const & handler) override;
	Connection DoOnMouseDrag(MouseDragSignal::slot_type const & handler) override;
	Connection DoOnResizeShape(ResizeShapeSignal::slot_type const & handler) override;
	Connection DoOnDeleteShape(DeleteShapeSignal::slot_type const & handler) override;

private:
	bool HitTest(CPoint const & point) const;

	CRect m_rect;
	ShapeViewType m_type;
	MousePressSignal m_mousePressSignal;
	MouseReleaseSignal m_mouseReleaseSignal;
	MouseDragSignal m_mouseDragSignal;
	ResizeShapeSignal m_resizeShapeSignal;
	DeleteShapeSignal m_deleteShapeSignal;
};
