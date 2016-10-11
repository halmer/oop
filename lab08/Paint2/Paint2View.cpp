#include "stdafx.h"
#ifndef SHARED_HANDLERS
#include "Paint2.h"
#endif

#include "Paint2Doc.h"
#include "Paint2View.h"
#include "CanvasView.h"
#include "CanvasPresenter.h"

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
	: m_canvasView(std::make_shared<CCanvasView>())
{
}

CPaint2View::~CPaint2View()
{
}

void CPaint2View::OnInitialUpdate()
{
	CPaint2Doc * pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
	{
		return;
	}

	CScrollView::OnInitialUpdate();
	SetScrollSizes(MM_TEXT, CSize(100, 100));
	
	if (!m_presenter)
	{
		pDoc->InitView(this);
	}
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

	CBitmap * oldBmp = dcMem.SelectObject(&bmpMem);
	dcMem.FillSolidRect(clientRect, RGB(255, 255, 255));

	m_canvasView->Draw(dcMem);

	pDC->BitBlt(clientRect.left, clientRect.top, clientRect.Width(), clientRect.Height(),
				&dcMem, clientRect.left, clientRect.top, SRCCOPY);

	dcMem.SelectObject(oldBmp);
}

void CPaint2View::Initialize()
{
	CPaint2Doc * pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
	{
		return;
	}

	m_canvasView = std::make_shared<CCanvasView>();
	m_presenter = std::make_unique<CCanvasPresenter>(pDoc->GetDoc(), m_canvasView);
	m_presenter->InitView(this);
}

void CPaint2View::Update(UpdateType type)
{
	if (type == UpdateType::RedrawUpdateScroll)
	{
		auto size = m_canvasView->GetSize();
		SetScrollSizes(MM_TEXT, CSize(size.BottomRight()));
	}

	Invalidate();
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
	m_presenter->OnCreateRectangle();
}

void CPaint2View::OnCreateTriangle()
{
	m_presenter->OnCreateTriangle();
}

void CPaint2View::OnCreateEllipse()
{
	m_presenter->OnCreateEllipse();
}

void CPaint2View::OnUndo()
{
	m_presenter->OnUndo();
}

void CPaint2View::OnRedo()
{
	m_presenter->OnRedo();
}

void CPaint2View::OnLButtonDown(UINT nFlags, CPoint point)
{
	SetCapture();

	point.Offset(GetDeviceScrollPosition());
	m_canvasView->HandleMouseDown(point);
	Invalidate();

	CScrollView::OnLButtonDown(nFlags, point);
}

void CPaint2View::OnLButtonUp(UINT nFlags, CPoint point)
{
	ReleaseCapture();
	
	point.Offset(GetDeviceScrollPosition());
	m_canvasView->HandleMouseUp(point);

	CScrollView::OnLButtonUp(nFlags, point);
}

void CPaint2View::OnMouseMove(UINT nFlags, CPoint point)
{
	SetClassLong(GetSafeHwnd(), GCL_HCURSOR, NULL);
	
	point.Offset(GetDeviceScrollPosition());
	m_canvasView->HandleMouseMove(nFlags, point);

	CScrollView::OnMouseMove(nFlags, point);
}

void CPaint2View::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	m_canvasView->HandleOnKeyDown(nChar);

	CScrollView::OnKeyDown(nChar, nRepCnt, nFlags);
}

BOOL CPaint2View::OnEraseBkgnd(CDC* /*pDC*/)
{
	return FALSE;
}

void CPaint2View::OnUpdateUndo(CCmdUI * pCmdUI)
{
	m_presenter->OnUpdateUndo(pCmdUI);
}

void CPaint2View::OnUpdateRedo(CCmdUI * pCmdUI)
{
	m_presenter->OnUpdateRedo(pCmdUI);
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
