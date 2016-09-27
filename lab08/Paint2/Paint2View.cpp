#include "stdafx.h"
#ifndef SHARED_HANDLERS
#include "Paint2.h"
#endif

#include "Paint2Doc.h"
#include "Paint2View.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

IMPLEMENT_DYNCREATE(CPaint2View, CScrollView)

BEGIN_MESSAGE_MAP(CPaint2View, CScrollView)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_COMMAND(ID_BUTTON_RECTANGLE, &CPaint2View::OnCreateRectangle)
	ON_COMMAND(ID_BUTTON_TRIANGLE, &CPaint2View::OnCreateTriangle)
	ON_COMMAND(ID_BUTTON_ELLIPSE, &CPaint2View::OnCreateEllipse)
	ON_COMMAND(ID_BUTTON_UNDO, &CPaint2View::OnUndo)
	ON_COMMAND(ID_BUTTON_REDO, &CPaint2View::OnRedo)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_KEYDOWN()
	ON_WM_ERASEBKGND()
	ON_UPDATE_COMMAND_UI(ID_BUTTON_UNDO, &CPaint2View::OnUpdateUndo)
	ON_UPDATE_COMMAND_UI(ID_BUTTON_REDO, &CPaint2View::OnUpdateRedo)
END_MESSAGE_MAP()

CPaint2View::CPaint2View()
	: m_presenter(theApp.m_doc, m_canvasView)
	, m_pen(PS_SOLID, 2, RGB(255, 0, 0))
	, m_brush(RGB(255, 255, 0))
{
	m_presenter.InitView(this);
}

CPaint2View::~CPaint2View()
{
}

void CPaint2View::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();

	SetScrollSizes(MM_TEXT, CSize(100, 100));
}

BOOL CPaint2View::PreCreateWindow(CREATESTRUCT& cs)
{
	return CScrollView::PreCreateWindow(cs);
}

void CPaint2View::OnDraw(CDC * pDC)
{
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

	for (size_t i = 0; i < m_canvasView.GetShapeCount(); ++i)
	{
		m_canvasView.GetShapeAtIndex(i)->DrawShape(&dcMem);
	}
	
	pDC->BitBlt(clientRect.left, clientRect.top, clientRect.Width(), clientRect.Height(),
				&dcMem, clientRect.left, clientRect.top, SRCCOPY);

	dcMem.SelectObject(oldPen);
	dcMem.SelectObject(oldBrush);
	dcMem.SelectObject(oldBmp);
}

void CPaint2View::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CPaint2View::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}

void CPaint2View::OnCreateRectangle()
{
	m_presenter.OnCreateRectangle();
	Invalidate();
}

void CPaint2View::OnCreateTriangle()
{
	m_presenter.OnCreateTriangle();
	Invalidate();
}

void CPaint2View::OnCreateEllipse()
{
	m_presenter.OnCreateEllipse();
	Invalidate();
}

void CPaint2View::OnUndo()
{
	m_presenter.OnUndo();
	Invalidate();
}

void CPaint2View::OnRedo()
{
	m_presenter.OnRedo();
	Invalidate();
}

void CPaint2View::OnLButtonDown(UINT nFlags, CPoint point)
{
	point.Offset(GetDeviceScrollPosition());
	m_presenter.OnLButtonDown(nFlags, point);

	CScrollView::OnLButtonDown(nFlags, point);
}

void CPaint2View::OnLButtonUp(UINT nFlags, CPoint point)
{
	m_presenter.OnLButtonUp(nFlags, point);
	Invalidate();

	CScrollView::OnLButtonUp(nFlags, point);
}

void CPaint2View::OnMouseMove(UINT nFlags, CPoint point)
{
	point.Offset(GetDeviceScrollPosition());
	m_presenter.OnMouseMove(nFlags, point);
	Invalidate();

	CScrollView::OnMouseMove(nFlags, point);
}

void CPaint2View::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	m_presenter.OnKeyDown(nChar, nRepCnt, nFlags);
	Invalidate();

	CScrollView::OnKeyDown(nChar, nRepCnt, nFlags);
}

BOOL CPaint2View::OnEraseBkgnd(CDC* /*pDC*/)
{
	return FALSE;
}

void CPaint2View::OnUpdateUndo(CCmdUI * pCmdUI)
{
	m_presenter.OnUpdateUndo(pCmdUI);
}

void CPaint2View::OnUpdateRedo(CCmdUI * pCmdUI)
{
	m_presenter.OnUpdateRedo(pCmdUI);
}

CPoint CPaint2View::GetPointInViewCenter() const
{
	CRect clientRect;
	GetClientRect(&clientRect);
	clientRect.OffsetRect(GetDeviceScrollPosition());

	return clientRect.CenterPoint();
}

#ifdef _DEBUG
void CPaint2View::AssertValid() const
{
	CScrollView::AssertValid();
}

void CPaint2View::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}

CPaint2Doc* CPaint2View::GetDocument() const
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CPaint2Doc)));
	return (CPaint2Doc*)m_pDocument;//-V2005
}
#endif
