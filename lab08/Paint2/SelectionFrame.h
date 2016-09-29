#pragma once
#include "ISelectionFrame.h"

class CSelectionFrame : public ISelectionFrame
{
public:
	CSelectionFrame();
	void SetFrame(CRect const & rect) override;
	void ResizeFrame(ControlPointType type, CPoint const & delta) override;
	CRect GetFrame() const;
	ControlPointType GetSelectedControlPoint(CPoint const & point) const override;
	void DrawFrame(CDC * pDC) override;

private:
	void InitControlPoints();

	CRect m_frame;
	std::vector<CRect> m_controlPoints;
	CPen m_controlPointPen;
	CBrush m_controlPointBrush;
	CBrush m_frameBrush;
};
