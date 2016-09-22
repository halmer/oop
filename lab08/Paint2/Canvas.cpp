#include "stdafx.h"
#include "Canvas.h"
#include "IShape.h"

void CCanvas::InsertShape(std::shared_ptr<IShape> const & shape, boost::optional<size_t> pos)
{
	if (pos && *pos > m_shapes.size())
	{
		return;
	}
	
	if (pos)
	{
		m_shapes.insert(m_shapes.begin() + *pos, shape);
	}
	else
	{
		m_shapes.push_back(shape);
	}

	
	m_insertShape(shape, pos);
}

void CCanvas::DeleteShape(std::shared_ptr<IShape> const & shape)
{
	auto it = find(m_shapes.begin(), m_shapes.end(), shape);
	if (it == m_shapes.end())
	{
		return;
	}
	
	m_shapes.erase(it);
	
	m_deleteShape(shape);
}

size_t CCanvas::GetShapeCount() const
{
	return m_shapes.size();
}

std::shared_ptr<IShape> CCanvas::GetShapeAtIndex(size_t index) const
{
	return m_shapes.at(index);
}

void CCanvas::DoOnInsertShape(InsertShapeSignal::slot_type const & handler)
{
	m_insertShape.connect(handler);
}

void CCanvas::DoOnDeleteShape(DeleteShapeSignal::slot_type const & handler)
{
	m_deleteShape.connect(handler);
}

#pragma warning(disable:4503)
