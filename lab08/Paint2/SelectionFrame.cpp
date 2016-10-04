#include "stdafx.h"
#include "SelectionFrame.h"

CSelectionFrame::CSelectionFrame()
	: m_controlPoints(4, CRect(0, 0, 7, 7))
	, m_controlPointPen(PS_SOLID, 1, RGB(56, 93, 138))
	, m_controlPointBrush(RGB(219, 239, 255))
	, m_frameBrush(RGB(56, 93, 138))
{
}

void CSelectionFrame::SetFrame(CRect const & rect)
{
	m_frame = rect;
}

CRect CSelectionFrame::GetFrame() const
{
	return m_frame;
}

ControlPointType CSelectionFrame::GetSelectedControlPoint(CPoint const & point) const
{
	CRgn rgn;
	for (size_t controlPointNumber = 0; controlPointNumber < m_controlPoints.size(); ++controlPointNumber)
	{
		rgn.CreateEllipticRgnIndirect(m_controlPoints[controlPointNumber]);
		if (rgn.PtInRegion(point))
		{
			switch (controlPointNumber)
			{
			case 0:
				return ControlPointType::NorthWest;
			case 1:
				return ControlPointType::NorthEast;
			case 2:
				return ControlPointType::SouthWest;
			case 3:
				return ControlPointType::SouthEast;
			}
		}
		rgn.DeleteObject();
	}

	return ControlPointType::NotSelected;
}

void CSelectionFrame::DrawFrame(CDC * pDC)
{
	CPen * oldPen = pDC->SelectObject(&m_controlPointPen);
	CBrush * oldBrush = pDC->SelectObject(&m_controlPointBrush);

	CRect frame = m_frame;
	frame.NormalizeRect();
	pDC->FrameRect(frame, &m_frameBrush);

	InitControlPoints();
	for (auto const & controlPoint : m_controlPoints)
	{
		pDC->Ellipse(controlPoint);
	}

	pDC->SelectObject(oldPen);
	pDC->SelectObject(oldBrush);
}

bool CSelectionFrame::IsInvertedCursor() const
{
	if (   (m_frame.left > m_frame.right && m_frame.top < m_frame.bottom)
		|| (m_frame.left < m_frame.right && m_frame.top > m_frame.bottom))
	{
		return true;
	}

	return false;
}

void CSelectionFrame::HandleDrag(ControlPointType type, CPoint const & delta)
{
	m_dragSignal(type, delta);
}

void CSelectionFrame::HandleDragEnd()
{
	m_dragEndSignal();
}

Connection CSelectionFrame::DoOnControlPointDrag(DragSignal::slot_type const & handler)
{
	return m_dragSignal.connect(handler);
}

Connection CSelectionFrame::DoOnDragEnd(DragEndSignal::slot_type const & handler)
{
	return m_dragEndSignal.connect(handler);
}

void CSelectionFrame::InitControlPoints()
{
	m_controlPoints[0].MoveToXY(m_frame.left - 3, m_frame.top - 3);
	m_controlPoints[1].MoveToXY(m_frame.right - 4, m_frame.top - 3);
	m_controlPoints[2].MoveToXY(m_frame.left - 3, m_frame.bottom - 4);
	m_controlPoints[3].MoveToXY(m_frame.right - 4, m_frame.bottom - 4);
}

#pragma warning (disable:4503)
