#include "stdafx.h"
#include "Frame.h"

CFrame::CFrame()
	: m_marks(4, CRect(0, 0, 7, 7))
	, m_markPen(PS_SOLID, 1, RGB(56, 93, 138))
	, m_markBrush(RGB(219, 239, 255))
	, m_frameBrush(RGB(56, 93, 138))
{
}

void CFrame::SetFrame(CRect const & rect)
{
	m_frame = rect;
}

void CFrame::DrawFrame(CDC * pDC)
{
	CPen * oldPen = pDC->SelectObject(&m_markPen);
	CBrush * oldBrush = pDC->SelectObject(&m_markBrush);

	CRect tmpFrame = m_frame;
	tmpFrame.NormalizeRect();
	pDC->FrameRect(tmpFrame, &m_frameBrush);

	InitMarks();
	for (auto const & mark : m_marks)
	{
		pDC->Ellipse(mark);
	}

	pDC->SelectObject(oldPen);
	pDC->SelectObject(oldBrush);
}

boost::optional<size_t> CFrame::GetSelectedMark(CPoint const & point) const
{
	CRgn rgn;
	for (size_t i = 0; i < m_marks.size(); ++i)
	{
		rgn.CreateEllipticRgnIndirect(m_marks[i]);
		if (rgn.PtInRegion(point))
		{
			return i;
		}
		rgn.DeleteObject();
	}

	return boost::none;
}

bool CFrame::IsInvertedCursor() const
{
	if ((m_frame.left > m_frame.right && m_frame.top < m_frame.bottom ) ||
		(m_frame.left < m_frame.right && m_frame.top > m_frame.bottom))
	{
		return true;
	}
	
	return false;
}

void CFrame::InitMarks()
{
	m_marks[0].MoveToXY(m_frame.left - 3, m_frame.top - 3);
	m_marks[1].MoveToXY(m_frame.right - 4, m_frame.top - 3);
	m_marks[2].MoveToXY(m_frame.left - 3, m_frame.bottom - 4);
	m_marks[3].MoveToXY(m_frame.right - 4, m_frame.bottom - 4);
}
