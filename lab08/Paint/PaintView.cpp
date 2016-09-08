#include "stdafx.h"
#ifndef SHARED_HANDLERS
#include "Paint.h"
#endif

#include "PaintDoc.h"
#include "PaintView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

using namespace std;

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
{
}

CPaintView::~CPaintView()
{
}

void CPaintView::OnInitialUpdate()
{
	CPaintDoc * pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
	{
		return;
	}
	
	CScrollView::OnInitialUpdate();
	SetScroll();
	pDoc->InitViewSignal(this);
}

BOOL CPaintView::PreCreateWindow(CREATESTRUCT& cs)
{
	return CScrollView::PreCreateWindow(cs);
}

void CPaintView::OnUpdate(CView* /*pSender*/, LPARAM lHint, CObject* /*pHint*/)
{
	switch (lHint)
	{
	case REDRAW_UPDATESCROLL:
		SetScroll();
		break;
	case REDRAW_UPDATESCROLL_RESETSELECT:
		SetScroll();
		m_selectedShape = nullptr;
		m_shapeSelection(m_selectedShape);
		break;
	case REDRAW:
		break;
	}
	
	Invalidate();
}

static map<ShapeType, function<void(CDC*, CRect)>> const drawingShapes{
	{ ShapeType::Rectangle,
		bind(static_cast<BOOL(CDC::*)(LPCRECT)>(&CDC::Rectangle), placeholders::_1, placeholders::_2)
	},
	{ ShapeType::Ellipse,
		bind(static_cast<BOOL(CDC::*)(LPCRECT)>(&CDC::Ellipse), placeholders::_1, placeholders::_2)
	},
	{ ShapeType::Triangle,
		[](CDC * pDC, CRect rect) {
			vector<CPoint> points;
			points.reserve(3);
			points.emplace_back(rect.left, rect.bottom);
			points.emplace_back((rect.left + rect.right) / 2, rect.top);
			points.emplace_back(rect.right, rect.bottom);
			pDC->Polygon(points.data(), points.size());
		}
	}
};

void CPaintView::OnDraw(CDC* pDC)
{
	CPaintDoc * pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
	{
		return;
	}

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
	auto & shapes = pDoc->GetShapesData();
	for (auto const & shape : shapes)
	{
		auto it = drawingShapes.find(shape->GetType());
		if (it != drawingShapes.end())
		{
			it->second(&dcMem, shape->GetFrameRect());
		}
	}
	if (m_selectedShape)
	{
		m_frame.DrawFrame(&dcMem);
	}

	pDC->BitBlt(clientRect.left, clientRect.top, clientRect.Width(), clientRect.Height(),
				&dcMem, clientRect.left, clientRect.top, SRCCOPY);
	
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
	m_createRectangle(GetRectInViewCenter());
}

void CPaintView::OnButtonTriangle()
{	
	m_createTriangle(GetRectInViewCenter());
}

void CPaintView::OnButtonEllipse()
{	
	m_createEllipse(GetRectInViewCenter());
}

void CPaintView::OnButtonUndo()
{
	m_buttonUndo();
}

void CPaintView::OnButtonRedo()
{
	m_buttonRedo();
}

void CPaintView::OnLButtonDown(UINT nFlags, CPoint point)
{
	SetCapture();

	if (m_selectedShape && m_selectedMark)
	{
		return;
	}

	point.Offset(GetDeviceScrollPosition());
	m_selectedShape = GetShapeContainingPoint(point);
	if (m_selectedShape)
	{
		m_frame.SetFrame(m_selectedShape->GetFrameRect());
	}

	Invalidate();

	m_shapeSelection(m_selectedShape);

	CScrollView::OnLButtonDown(nFlags, point);
}

void CPaintView::OnLButtonUp(UINT nFlags, CPoint point)
{
	ReleaseCapture();

	m_lButtonUp(GetOffsetType());

	SetScroll();

	CScrollView::OnLButtonUp(nFlags, point);
}

void CPaintView::OnMouseMove(UINT nFlags, CPoint currPoint)
{	 
	currPoint.Offset(GetDeviceScrollPosition());
	if (IsOverMaxPosition(currPoint))
	{
		return;
	}

	if (nFlags & MK_LBUTTON && (m_selectedMark || m_selectedShape))
	{
		m_offsetSelectedShape(GetOffsetType(), currPoint);

		if (m_selectedMark)
		{
			ChangeCursor(true, true);
		}

		m_frame.SetFrame(m_movingShape->GetFrameRect());
	}
	else
	{
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
	if (nChar == VK_DELETE)
	{
		m_keyDown();
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

CRect CPaintView::GetRectInViewCenter() const
{
	CRect clientRect;
	GetClientRect(&clientRect);
	clientRect.OffsetRect(GetDeviceScrollPosition());

	CPoint point = clientRect.CenterPoint();
	point.Offset(-WIDTH / 2, -HEIGHT / 2);

	return { point, CSize(WIDTH, HEIGHT) };
}

OffsetType CPaintView::GetOffsetType() const
{
	if (m_selectedMark)
	{
		switch (*m_selectedMark)
		{
		case 0:
			return OffsetType::TopLeft;
		case 1:
			return OffsetType::TopRight;
		case 2:
			return OffsetType::BottomLeft;
		case 3:
			return OffsetType::BottomRight;
		}
	}

	return OffsetType::AllSide;
}

void CPaintView::SetAppCursor(CPoint const & point)
{
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
		if (GetShapeContainingPoint(point))
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
			m_frame.IsInvertedCursor()
				? SetCursor(m_cursorSizeNESW)
				: SetCursor(m_cursorSizeNWSE);
		}
		else
		{
			m_frame.IsInvertedCursor()
				? SetCursor(m_cursorSizeNWSE)
				: SetCursor(m_cursorSizeNESW);
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

static map<ShapeType, function<bool(CRect const &, CPoint const &)>> const pointInShape{
	{ ShapeType::Rectangle,
		[](CRect const & rect, CPoint const & point) {
			CRgn rgn;
			rgn.CreateRectRgnIndirect(&rect);
			return rgn.PtInRegion(point) != 0;
		}
	},
	{ ShapeType::Ellipse,
		[](CRect const & rect, CPoint const & point) {
			CRgn rgn;
			rgn.CreateEllipticRgnIndirect(&rect);
			return rgn.PtInRegion(point) != 0;
		}
	},
	{ ShapeType::Triangle,
		[](CRect const & rect, CPoint const & point) {
			vector<CPoint> points;
			points.reserve(3);
			points.emplace_back(rect.left, rect.bottom);
			points.emplace_back((rect.left + rect.right) / 2, rect.top);
			points.emplace_back(rect.right, rect.bottom);
			CRgn rgn;
			rgn.CreatePolygonRgn(points.data(), points.size(), WINDING);
			return rgn.PtInRegion(point) != 0;
		}
	}
};

shared_ptr<CShape> CPaintView::GetShapeContainingPoint(CPoint const & point) const
{
	CPaintDoc * pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
	{
		return nullptr;
	}

	auto & shapes = pDoc->GetShapesData();
	for (auto const & shape : boost::adaptors::reverse(shapes))
	{
		auto it = pointInShape.find(shape->GetType());
		if (it != pointInShape.end())
		{
			if (it->second(shape->GetFrameRect(), point))
			{
				return shape;
			}
		}
	}

	return nullptr;
}

void CPaintView::SetScroll()
{
	CPaintDoc * pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
	{
		return;
	}

	int maxRight = 0;
	int maxBottom = 0;

	auto & shapes = pDoc->GetShapesData();
	for (auto const & shape : shapes)
	{
		CPoint point = shape->GetBottomRight();
		maxRight = max(maxRight, point.x);
		maxBottom = max(maxBottom, point.y);
	}

	SetScrollSizes(MM_TEXT, CSize(maxRight, maxBottom));
}

void CPaintView::SetMovingShape(shared_ptr<CShape> const & shape)
{
	m_movingShape = shape;
}

void CPaintView::DoOnButtonRectangle(CreateShapeSignal::slot_type const & slot)
{
	m_createRectangle.connect(slot);
}

void CPaintView::DoOnButtonTriangle(CreateShapeSignal::slot_type const & slot)
{
	m_createTriangle.connect(slot);
}

void CPaintView::DoOnButtonEllipse(CreateShapeSignal::slot_type const & slot)
{
	m_createEllipse.connect(slot);
}

void CPaintView::DoOnShapeSelection(ShapeSelectionSignal::slot_type const & slot)
{
	m_shapeSelection.connect(slot);
}

void CPaintView::DoOnMouseMoveWithLButtonDown(OffsetSelectedShapeSignal::slot_type const & slot)
{
	m_offsetSelectedShape.connect(slot);
}

void CPaintView::DoOnLButtonUp(LButtonUpSignal::slot_type const & slot)
{
	m_lButtonUp.connect(slot);
}

void CPaintView::DoOnButtonUndo(UndoRedoSignal::slot_type const & slot)
{
	m_buttonUndo.connect(slot);
}

void CPaintView::DoOnButtonRedo(UndoRedoSignal::slot_type const & slot)
{
	m_buttonRedo.connect(slot);
}

void CPaintView::DoOnKeyDeleteDown(KeyDeleteDownSignal::slot_type const & slot)
{
	m_keyDown.connect(slot);
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
