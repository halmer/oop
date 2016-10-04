#include "stdafx.h"
#include "ShapeView.h"

using namespace std;

CShapeView::CShapeView(CRect const & rect, ShapeViewType type)
	: m_rect(rect)
	, m_type(type)
{
}

void CShapeView::SetRect(CRect const & rect)
{
	m_rect = rect;
}

CRect CShapeView::GetRect() const
{
	return m_rect;
}

static map<ShapeViewType, function<void(CDC*, CRect)>> const drawingShapes{
	{ ShapeViewType::Rectangle,
		bind(static_cast<BOOL(CDC::*)(LPCRECT)>(&CDC::Rectangle), placeholders::_1, placeholders::_2)
	},
	{ ShapeViewType::Ellipse,
		bind(static_cast<BOOL(CDC::*)(LPCRECT)>(&CDC::Ellipse), placeholders::_1, placeholders::_2)
	},
	{ ShapeViewType::Triangle,
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

void CShapeView::DrawShape(CDC * pDC)
{
	auto it = drawingShapes.find(m_type);
	if (it != drawingShapes.end())
	{
		it->second(pDC, m_rect);
	}
}

bool CShapeView::IsPointInShape(CPoint const & point) const
{
	if (!HitTest(point))
	{
		return false;
	}

	return true;
}

bool CShapeView::HandleMouseDown(CPoint const & point)
{
	if (!HitTest(point))
	{
		return false;
	}

	m_mousePressSignal(*this);

	return true;
}

void CShapeView::HandleMouseUp(CPoint const & point)
{
	m_mouseReleaseSignal(*this, HitTest(point));
}

void CShapeView::HandleDrag(CPoint const & point, CPoint const & oldPoint)
{
	CRect targetRect = m_rect;
	targetRect.OffsetRect(point - oldPoint);
	m_mouseDragSignal(*this, targetRect);
}

void CShapeView::HandleResizeShape(CRect const & rect)
{
	m_resizeShapeSignal(*this, rect);
}

void CShapeView::HandleDeleteShape()
{
	m_deleteShapeSignal(*this);
}

Connection CShapeView::DoOnMousePress(MousePressSignal::slot_type const & handler)
{
	return m_mousePressSignal.connect(handler);
}

Connection CShapeView::DoOnMouseRelease(MouseReleaseSignal::slot_type const & handler)
{
	return m_mouseReleaseSignal.connect(handler);
}

Connection CShapeView::DoOnMouseDrag(MouseDragSignal::slot_type const & handler)
{
	return m_mouseDragSignal.connect(handler);
}

Connection CShapeView::DoOnResizeShape(ResizeShapeSignal::slot_type const & handler)
{
	return m_resizeShapeSignal.connect(handler);
}

Connection CShapeView::DoOnDeleteShape(DeleteShapeSignal::slot_type const & handler)
{
	return m_deleteShapeSignal.connect(handler);
}

static map<ShapeViewType, function<bool(CRect const &, CPoint const &)>> const pointInShape{
	{ ShapeViewType::Rectangle,
		[](CRect const & rect, CPoint const & point) {
			CRgn rgn;
			rgn.CreateRectRgnIndirect(&rect);
			return rgn.PtInRegion(point) != 0;
		}
	},
	{ ShapeViewType::Ellipse,
		[](CRect const & rect, CPoint const & point) {
			CRgn rgn;
			rgn.CreateEllipticRgnIndirect(&rect);
			return rgn.PtInRegion(point) != 0;
		}
	},
	{ ShapeViewType::Triangle,
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

bool CShapeView::HitTest(CPoint const & point) const
{
	auto it = pointInShape.find(m_type);
	if (it != pointInShape.end())
	{
		if (it->second(m_rect, point))
		{
			return true;
		}
	}
	
	return false;
}

#pragma warning (disable:4503)
