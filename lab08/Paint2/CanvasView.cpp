#include "stdafx.h"
#include "CanvasView.h"
#include "ShapeView.h"

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
}

void CCanvasView::SelectShape(std::shared_ptr<IShapeView> const & shape)
{
	auto it = find(m_shapes.begin(), m_shapes.end(), shape);
	if (it == m_shapes.end())
	{
		m_selectedShape = nullptr;
		return;
	}

	m_selectedShape = shape;
}

std::shared_ptr<ISelectionView> CCanvasView::GetSelection()
{
	return std::shared_ptr<ISelectionView>();
}

size_t CCanvasView::GetShapeCount() const
{
	return m_shapes.size();
}

std::shared_ptr<IShapeView> CCanvasView::GetShapeAtIndex(size_t index) const
{
	return m_shapes.at(index);
}

CCanvasView::CCanvasView()
	: m_pen(PS_SOLID, 2, RGB(255, 0, 0))
	, m_brush(RGB(255, 255, 0))
{

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
}

void CCanvasView::HandleMouseDown(const CPoint & point)
{
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

void CCanvasView::HandleMouseUp(const CPoint & point)
{
	if (m_capturedShape)
	{
		m_capturedShape->HandleMouseUp(point);
		m_mousePressPoint = boost::none;
	}
}

void CCanvasView::HandleMouseMove(UINT nFlags, const CPoint & point)
{
	if (nFlags & MK_LBUTTON)
	{
		if (m_capturedShape && m_mousePressPoint)
		{
			m_capturedShape->HandleDrag(point, *m_mousePressPoint);
			m_mousePressPoint = point;
		}
	}
}
