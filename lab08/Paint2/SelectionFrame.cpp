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

void CSelectionFrame::ResizeFrame(ControlPointType type, CPoint const & delta)
{
	switch (type)
	{
	case ControlPointType::NorthEast:
		m_frame.TopLeft() += delta;
		break;
	case ControlPointType::NorthWest:
		m_frame.top += delta.y;
		m_frame.right += delta.x;
		break;
	case ControlPointType::SouthEast:
		m_frame.bottom += delta.y;
		m_frame.left += delta.x;
		break;
	case ControlPointType::SouthWest:
		m_frame.BottomRight() += delta;
		break;
	case ControlPointType::NotSelected:
		break;
	}
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
				return ControlPointType::NorthEast;
			case 1:
				return ControlPointType::NorthWest;
			case 2:
				return ControlPointType::SouthEast;
			case 3:
				return ControlPointType::SouthWest;
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

void CSelectionFrame::InitControlPoints()
{
	m_controlPoints[0].MoveToXY(m_frame.left - 3, m_frame.top - 3);
	m_controlPoints[1].MoveToXY(m_frame.right - 4, m_frame.top - 3);
	m_controlPoints[2].MoveToXY(m_frame.left - 3, m_frame.bottom - 4);
	m_controlPoints[3].MoveToXY(m_frame.right - 4, m_frame.bottom - 4);
}
