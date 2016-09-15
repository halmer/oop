#include "stdafx.h"
#include "Canvas.h"

void CCanvas::InsertShape(std::shared_ptr<IShape> const & shape, boost::optional<size_t> pos)
{
	if (pos)
	{
		if (*pos > m_shapes.size())
		{
			return;
		}

		m_shapes.insert(m_shapes.begin() + *pos, shape);
	}
	else
	{
		m_shapes.push_back(shape);
	}
}

void CCanvas::DeleteShape(std::shared_ptr<IShape> const & shape)
{
	auto it = find(m_shapes.begin(), m_shapes.end(), shape);
	if (it == m_shapes.end())
	{
		return;
	}

	m_shapes.erase(it);
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

	return m_shapes[index];
}
