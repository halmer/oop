#pragma once
#include "ISelectionFrame.h"

class CSelectionFrame : public ISelectionFrame
{
public:
	CSelectionFrame();
	void SetFrame(CRect const & rect) override;
	CRect GetFrame() const;
	ControlPointType GetSelectedControlPoint(CPoint const & point) const override;
	void DrawFrame(CDC * pDC) override;
	bool IsInvertedCursor() const override;

	void HandleDrag(ControlPointType type, CPoint const & delta);
	void HandleDragEnd();
	
	Connection DoOnControlPointDrag(DragSignal::slot_type const & handler) override;
	Connection DoOnDragEnd(DragEndSignal::slot_type const & handler) override;

private:
	void InitControlPoints();

	CRect m_frame;
	std::vector<CRect> m_controlPoints;
	CPen m_controlPointPen;
	CBrush m_controlPointBrush;
	CBrush m_frameBrush;
	DragSignal m_dragSignal;
	DragEndSignal m_dragEndSignal;
};
