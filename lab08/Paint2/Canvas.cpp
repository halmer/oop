#include "stdafx.h"
#include "Canvas.h"
#include "IShape.h"

void CCanvas::InsertShape(std::shared_ptr<IShape> const & shape, boost::optional<size_t> pos)
{
	if (pos && *pos > m_shapes.size())
	{
		return;
	}
	
	auto connection = shape->DoOnShapeChange(std::bind(&CCanvas::ChangeShape, this, std::placeholders::_1));

	if (pos)
	{
		m_shapes.insert(m_shapes.begin() + *pos, { shape, connection });
	}
	else
	{
		m_shapes.push_back({ shape, connection });
	}

	
	m_insertShape(shape, pos);
}

void CCanvas::DeleteShape(std::shared_ptr<IShape> const & shape)
{
	auto it = find_if(m_shapes.begin(), m_shapes.end(),
		[&shape](Data const & data) { return data.shape == shape; });
	if (it == m_shapes.end())
	{
		return;
	}
	
	it->connection.disconnect();
	m_shapes.erase(it);
	
	m_deleteShape(shape);
}

size_t CCanvas::GetShapeCount() const
{
	return m_shapes.size();
}

std::shared_ptr<IShape> CCanvas::GetShapeAtIndex(size_t index) const
{
	if (index >= m_shapes.size())
	{
		return nullptr;
	}

	return m_shapes[index].shape;
}

void CCanvas::DoOnInsertShape(InsertShapeSignal::slot_type const & handler)
{
	m_insertShape.connect(handler);
}

void CCanvas::DoOnDeleteShape(DeleteShapeSignal::slot_type const & handler)
{
	m_deleteShape.connect(handler);
}

void CCanvas::DoOnChangeShape(ChangeShapeSignal::slot_type const & handler)
{
	m_changeShape.connect(handler);
}

void CCanvas::ChangeShape(IShape const * shape)
{
	auto it = find_if(m_shapes.begin(), m_shapes.end(),
		[&shape](Data const & data) { return data.shape.get() == shape; });
	if (it == m_shapes.end())
	{
		return;
	}

	m_changeShape(it->shape);
}

#pragma warning(disable:4503)
