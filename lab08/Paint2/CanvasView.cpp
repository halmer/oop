#include "stdafx.h"
#include "CanvasView.h"
#include "ShapeView.h"

CCanvasView::CCanvasView()
	: m_pen(PS_SOLID, 2, RGB(255, 0, 0))
	, m_brush(RGB(255, 255, 0))
	, m_selectedControlPoint(ControlPointType::NotSelected)
{
}

std::shared_ptr<IShapeView> CCanvasView::InsertShape(ShapeViewType type, CRect const & rect, boost::optional<size_t> position)
{
	auto shape = std::make_shared<CShapeView>(rect, type);
	
	if (position)
	{
		if (*position > m_shapes.size())
		{
			return nullptr;
		}
		
		m_shapes.insert(m_shapes.begin() + *position, shape);
	}
	else
	{
		m_shapes.push_back(shape);
	}

	return shape;
}

void CCanvasView::DeleteShape(std::shared_ptr<IShapeView> const & shape)
{
	auto it = find(m_shapes.begin(), m_shapes.end(), shape);
	if (it == m_shapes.end())
	{
		return;
	}

	m_shapes.erase(it);
	m_capturedShape = nullptr;
}

void CCanvasView::DeleteSelection()
{
	DeleteShape(m_capturedShape);
}

void CCanvasView::SelectShape(std::shared_ptr<IShapeView> const & shape)
{
	auto it = find(m_shapes.begin(), m_shapes.end(), shape);
	if (it == m_shapes.end())
	{
		m_capturedShape = nullptr;
		return;
	}

	m_capturedShape = *it;
}

std::shared_ptr<ISelectionFrame> CCanvasView::GetSelection()
{
	return std::shared_ptr<ISelectionFrame>();
}

size_t CCanvasView::GetShapeCount() const
{
	return m_shapes.size();
}

std::shared_ptr<IShapeView> CCanvasView::GetShapeAtIndex(size_t index) const
{
	return m_shapes.at(index);
}

CRect CCanvasView::GetSize() const
{
	int maxLeft = 0, maxTop = 0, maxRight = 0, maxBottom = 0;

	for (auto const & shape : m_shapes)
	{
		CRect rect = shape->GetRect();
		rect.NormalizeRect();

		CPoint point = rect.TopLeft();
		maxLeft = max(maxLeft, point.x);
		maxTop = max(maxTop, point.y);
		
		point = rect.BottomRight();
		maxRight = max(maxRight, point.x);
		maxBottom = max(maxBottom, point.y);
	}

	return { maxLeft, maxTop, maxRight, maxBottom };
}

void CCanvasView::Draw(CDC & dc)
{
	CPen * oldPen = dc.SelectObject(&m_pen);
	CBrush * oldBrush = dc.SelectObject(&m_brush);

	for (auto & shape : m_shapes)
	{
		shape->DrawShape(&dc);
	}

	dc.SelectObject(oldPen);
	dc.SelectObject(oldBrush);

	if (m_capturedShape)
	{
		m_frame.SetFrame(m_capturedShape->GetRect());
		m_frame.DrawFrame(&dc);
	}
}

void CCanvasView::HandleMouseDown(CPoint const & point)
{
	if (m_capturedShape)
	{
		m_selectedControlPoint = m_frame.GetSelectedControlPoint(point);
		if (m_selectedControlPoint != ControlPointType::NotSelected)
		{
			m_mousePressPoint = point;
			return;
		}
	}
	
	for (auto const & shape : boost::adaptors::reverse(m_shapes))
	{
		if (shape->HandleMouseDown(point))
		{
			m_capturedShape = shape;
			m_mousePressPoint = point;
			return;
		}
	}

	m_mousePressPoint = boost::none;
	m_capturedShape.reset();
}

void CCanvasView::HandleMouseUp(CPoint const & point)
{
	if (m_capturedShape)
	{
		m_capturedShape->HandleMouseUp(point);
		m_mousePressPoint = boost::none;
		m_selectedControlPoint = ControlPointType::NotSelected;
	}
}

void CCanvasView::HandleMouseMove(UINT nFlags, CPoint const & point)
{
	if (nFlags & MK_LBUTTON && m_mousePressPoint)
	{
		if (m_selectedControlPoint != ControlPointType::NotSelected)
		{
			m_frame.ResizeFrame(m_selectedControlPoint, point - *m_mousePressPoint);
			m_capturedShape->HandleResizeShape(m_frame.GetFrame());
			m_mousePressPoint = point;
		}
		else if (m_capturedShape)
		{
			m_capturedShape->HandleDrag(point, *m_mousePressPoint);
			m_mousePressPoint = point;
		}
	}
}

void CCanvasView::HandleOnKeyDown(UINT nChar)
{
	if ((nChar == VK_DELETE) && m_capturedShape)
	{
		m_capturedShape->HandleDeleteShape();
		m_capturedShape = nullptr;
	}
}
