#include "stdafx.h"
#include "Model.h"

using namespace std;

void CModel::AddShape(shared_ptr<CShape> const & shape, boost::optional<size_t> position)
{
	if (position)
	{
		if (*position > m_shapes.size())
		{
			return;
		}

		m_shapes.insert(m_shapes.begin() + *position, shape);
	}
	else
	{
		m_shapes.push_back(shape);
	}

	m_addShape(shape, position);
}

void CModel::DeleteShape(boost::optional<size_t> position)
{
	if (position)
	{
		if (*position >= m_shapes.size())
		{
			return;
		}

		m_shapes.erase(m_shapes.begin() + *position);
	}
	else
	{
		m_shapes.pop_back();
	}

	m_deleteShape(position);
}

void CModel::DeleteAllShapes()
{
	m_shapes.clear();
	m_deleteAllShapes();
}

void CModel::OffsetShape(shared_ptr<CShape> const & shape, CPoint const & delta, OffsetType type)
{
	auto it = find(m_shapes.begin(), m_shapes.end(), shape);
	if (it == m_shapes.end())
	{
		return;
	}

	size_t position = distance(m_shapes.begin(), it);
	m_shapes[position]->Offset(delta, type);

	m_offsetShape(m_shapes[position], position);
}

void CModel::DoOnAddShape(AddShapeSignal::slot_type const & slot)
{
	m_addShape.connect(slot);
}

void CModel::DoOnDeleteShape(DeleteShapeSignal::slot_type const & slot)
{
	m_deleteShape.connect(slot);
}

void CModel::DoOnDeleteAllShapes(DeleteAllShapesSignal::slot_type const & slot)
{
	m_deleteAllShapes.connect(slot);
}

void CModel::DoOnOffsetShape(OffsetShapeSignal::slot_type const & slot)
{
	m_offsetShape.connect(slot);
}
