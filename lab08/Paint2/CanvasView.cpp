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
