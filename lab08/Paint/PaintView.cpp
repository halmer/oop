#include "stdafx.h"
#ifndef SHARED_HANDLERS
#include "Paint.h"
#endif

#include "PaintDoc.h"
#include "PaintView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

const int WIDTH = 50;
const int HEIGHT = 50;

IMPLEMENT_DYNCREATE(CPaintView, CScrollView)

BEGIN_MESSAGE_MAP(CPaintView, CScrollView)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_COMMAND(ID_BUTTON_RECTANGLE, &CPaintView::OnButtonRectangle)
	ON_COMMAND(ID_BUTTON_TRIANGLE, &CPaintView::OnButtonTriangle)
	ON_COMMAND(ID_BUTTON_ELLIPSE, &CPaintView::OnButtonEllipse)
	ON_COMMAND(ID_BUTTON_UNDO, &CPaintView::OnButtonUndo)
	ON_COMMAND(ID_BUTTON_REDO, &CPaintView::OnButtonRedo)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_ERASEBKGND()
	ON_WM_KEYDOWN()
	ON_UPDATE_COMMAND_UI(ID_BUTTON_UNDO, &CPaintView::OnUpdateButtonUndo)
	ON_UPDATE_COMMAND_UI(ID_BUTTON_REDO, &CPaintView::OnUpdateButtonRedo)
END_MESSAGE_MAP()

CPaintView::CPaintView()
	: m_pen(PS_SOLID, 2, RGB(255, 0, 0))
	, m_brush(RGB(255, 255, 0))
	, m_cursorArrow(AfxGetApp()->LoadStandardCursor(IDC_ARROW))
	, m_cursorSizeAll(AfxGetApp()->LoadStandardCursor(IDC_SIZEALL))
	, m_cursorSizeNWSE(AfxGetApp()->LoadStandardCursor(IDC_SIZENWSE))
	, m_cursorSizeNESW(AfxGetApp()->LoadStandardCursor(IDC_SIZENESW))
	, m_useHistory(false)
{
}

CPaintView::~CPaintView()
{
}

void CPaintView::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();
	SetScrollSizes(MM_TEXT, CSize(0, 0));
}

BOOL CPaintView::PreCreateWindow(CREATESTRUCT& cs)
{
	return CScrollView::PreCreateWindow(cs);
}

void CPaintView::OnUpdate(CView* /*pSender*/, LPARAM lHint, CObject* /*pHint*/)
{
	CPaintDoc * pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
	{
		return;
	}

	if (lHint == 0)
	{
		SetScrollSizes(MM_TEXT, pDoc->GetDocSize());
	}
	else if (lHint == 1)
	{
		m_selectedShape = nullptr;
		SetScrollSizes(MM_TEXT, pDoc->GetDocSize());
	}

	Invalidate();
}

void CPaintView::OnDraw(CDC* pDC)
{
	CPaintDoc * pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	CDC dcMem;
	dcMem.CreateCompatibleDC(pDC);
	
	CRect clientRect;
	GetClientRect(&clientRect);
	pDC->DPtoLP(&clientRect);

	CBitmap bmpMem;
	bmpMem.CreateCompatibleBitmap(pDC, clientRect.right, clientRect.bottom);

	CPen * oldPen = dcMem.SelectObject(&m_pen);
	CBrush * oldBrush = dcMem.SelectObject(&m_brush);
	CBitmap * oldBmp = dcMem.SelectObject(&bmpMem);

	dcMem.FillSolidRect(clientRect, RGB(255, 255, 255));
	pDoc->DrawShapes()(&dcMem);
	if (m_selectedShape)
	{
		m_frame.DrawFrame(&dcMem);
	}

	pDC->BitBlt(clientRect.left, clientRect.top, clientRect.Width(), clientRect.Height(), &dcMem, clientRect.left, clientRect.top, SRCCOPY);
	
	dcMem.SelectObject(oldPen);
	dcMem.SelectObject(oldBrush);
	dcMem.SelectObject(oldBmp);
}

void CPaintView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CPaintView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}

void CPaintView::OnButtonRectangle()
{
	CPaintDoc * pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
	{
		return;
	}

	pDoc->CreateRectangle(CRect(GetTopLeftForShape(), CSize(WIDTH, HEIGHT)));
}

void CPaintView::OnButtonTriangle()
{
	CPaintDoc * pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
	{
		return;
	}

	pDoc->CreateTriangle(CRect(GetTopLeftForShape(), CSize(WIDTH, HEIGHT)));
}

void CPaintView::OnButtonEllipse()
{
	CPaintDoc  * pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
	{
		return;
	}

	pDoc->CreateEllipse(CRect(GetTopLeftForShape(), CSize(WIDTH, HEIGHT)));
}

void CPaintView::OnButtonUndo()
{
	CPaintDoc * pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
	{
		return;
	}

	pDoc->Undo();
}

void CPaintView::OnButtonRedo()
{
	CPaintDoc * pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
	{
		return;
	}

	pDoc->Redo();
}

void CPaintView::OnLButtonDown(UINT nFlags, CPoint point)
{
	CPaintDoc * pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
	{
		return;
	}

	SetCapture();

	point.Offset(GetDeviceScrollPosition());

	if (m_selectedShape && m_selectedMark)
	{
		return;
	}
	
	m_selectedShape = m_selectedShapeWhenMoving;
	if (m_selectedShape)
	{
		m_frame.SetFrame(pDoc->GetFrameRectOfShape(m_selectedShape));
	}

	Invalidate();
	
	CScrollView::OnLButtonDown(nFlags, point);
}

void CPaintView::OnLButtonUp(UINT nFlags, CPoint point)
{
	CPaintDoc * pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
	{
		return;
	}

	ReleaseCapture();

	if (m_useHistory && (*m_lastPoint != *m_firstPoint))
	{
		if (m_selectedMark)
		{
			pDoc->OffsetShape(m_selectedShape, *m_lastPoint - *m_firstPoint, GetOffsetType(*m_selectedMark), m_useHistory);
		}
		else
		{
			pDoc->OffsetShape(m_selectedShape, *m_lastPoint - *m_firstPoint, OffsetType::AllSide, m_useHistory);
		}
	}

	m_firstPoint = boost::none;
	m_lastPoint = boost::none;
	m_useHistory = false;

	SetScrollSizes(MM_TEXT, pDoc->GetDocSize());

	CScrollView::OnLButtonUp(nFlags, point);
}

void CPaintView::OnMouseMove(UINT nFlags, CPoint currPoint)
{
	CPaintDoc * pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
	{
		return;
	}
		 
	currPoint.Offset(GetDeviceScrollPosition());
	if (IsOverMaxPosition(currPoint))
	{
		return;
	}

	if (nFlags & MK_LBUTTON && (m_selectedMark || m_selectedShape))
	{
		if (!m_lastPoint)
		{
			m_firstPoint = currPoint;
			m_lastPoint = currPoint;
			m_useHistory = true;
		}

		if (m_selectedMark)
		{
			pDoc->OffsetShape(m_selectedShape, currPoint - *m_lastPoint, GetOffsetType(*m_selectedMark));
			ChangeCursor(true, true);
		}
		else
		{
			pDoc->OffsetShape(m_selectedShape, currPoint - *m_lastPoint, OffsetType::AllSide);
		}

		m_frame.SetFrame(pDoc->GetFrameRectOfShape(m_selectedShape));
		m_lastPoint = currPoint;
	}
	else
	{
		m_firstPoint = boost::none;
		m_lastPoint = boost::none;
		m_useHistory = false;
		
		SetAppCursor(currPoint);
	}

	CScrollView::OnMouseMove(nFlags, currPoint);
}

BOOL CPaintView::OnEraseBkgnd(CDC* /*pDC*/)
{
	return FALSE;
}

void CPaintView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	CPaintDoc * pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
	{
		return;
	}

	if (nChar == VK_DELETE)
	{
		if (m_selectedShape)
		{
			pDoc->DeleteShape(m_selectedShape);
		}
	}

	CScrollView::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CPaintView::OnUpdateButtonUndo(CCmdUI *pCmdUI)
{
	CPaintDoc * pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
	{
		return;
	}

	pCmdUI->Enable(pDoc->CanUndo());
}

void CPaintView::OnUpdateButtonRedo(CCmdUI *pCmdUI)
{
	CPaintDoc * pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
	{
		return;
	}

	pCmdUI->Enable(pDoc->CanRedo());
}

CPoint CPaintView::GetTopLeftForShape() const
{
	CRect clientRect;
	GetClientRect(&clientRect);
	clientRect.OffsetRect(GetDeviceScrollPosition());

	CPoint point = clientRect.CenterPoint();
	point.Offset(-WIDTH / 2, -HEIGHT / 2);

	return point;
}

OffsetType CPaintView::GetOffsetType(size_t frameMark) const
{
	switch (frameMark)
	{
	case 0:
		return OffsetType::TopLeft;
	case 1:
		return OffsetType::TopRight;
	case 2:
		return OffsetType::BottomLeft;
	case 3:
		return OffsetType::BottomRight;
	default:
		return OffsetType::AllSide;
	}
}

void CPaintView::SetAppCursor(CPoint const & point)
{
	CPaintDoc * pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
	{
		return;
	}

	if (m_selectedShape)
	{
		m_selectedMark = m_frame.GetSelectedMark(point);
		if (m_selectedMark)
		{
			ChangeCursor(true, true);
		}
	}

	if (!m_selectedMark)
	{
		m_selectedShapeWhenMoving = pDoc->GetShapeContainingPoint(point);
		if (m_selectedShapeWhenMoving)
		{
			ChangeCursor(true, false);
		}
		else
		{
			ChangeCursor(false, false);
		}
	}
}

void CPaintView::ChangeCursor(bool isSelectedShape, bool isSelectedMark)
{
	SetClassLong(GetSafeHwnd(), GCL_HCURSOR, NULL);
	if (isSelectedShape && isSelectedMark)
	{
		if (*m_selectedMark == 0 || *m_selectedMark == 3)
		{
			if (m_frame.IsInvertedCursor())
			{
				SetCursor(m_cursorSizeNESW);

			}
			else
			{
				SetCursor(m_cursorSizeNWSE);
			}
		}
		else
		{
			if (m_frame.IsInvertedCursor())
			{
				SetCursor(m_cursorSizeNWSE);
			}
			else
			{
				SetCursor(m_cursorSizeNESW);
			}
		}
	}
	else if (isSelectedShape && !isSelectedMark)
	{
		SetCursor(m_cursorSizeAll);
	}
	else
	{
		SetCursor(m_cursorArrow);
	}
}

bool CPaintView::IsOverMaxPosition(CPoint const & point) const
{
	CRect clientRect;
	GetClientRect(&clientRect);
	clientRect.OffsetRect(GetDeviceScrollPosition());
	CSize size = GetTotalSize();
	int x = max(clientRect.right, size.cx);
	int y = max(clientRect.bottom, size.cy);

	if (point.x > x || point.y > y || point.x < 0 || point.y < 0)
	{
		return true;
	}

	return false;
}

#ifdef _DEBUG
CPaintDoc* CPaintView::GetDocument() const
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CPaintDoc)));
	return (CPaintDoc*)m_pDocument;//-V2005
}
void CPaintView::AssertValid() const
{
	CScrollView::AssertValid();
}

void CPaintView::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}
#endif
