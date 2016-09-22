#include "stdafx.h"
#include "EditableShape.h"
#include "EditableCanvas.h"
#include "Shape.h"

CEditableCanvas::CEditableCanvas(ICanvas & canvas, CHistory & history)
	: m_canvas(canvas)
	, m_history(history)
	, m_selectedShapeIndex(0)
{
	m_canvas.DoOnInsertShape(std::bind(&CEditableCanvas::InsertShape, this, std::placeholders::_1, std::placeholders::_2));
	m_canvas.DoOnDeleteShape(std::bind(&CEditableCanvas::DeleteShape, this, std::placeholders::_1));
}

void CEditableCanvas::AddShape(ShapeType type, CRect const & rect)
{
	std::shared_ptr<IShape> shape = std::make_shared<CShape>(rect, type);
	
	m_history.AddCommandAndExecute({
		std::bind(&ICanvas::InsertShape, &m_canvas, shape, boost::none),
		std::bind(&ICanvas::DeleteShape, &m_canvas, shape)
	});
}

void CEditableCanvas::SelectShape(std::shared_ptr<IEditableShape> const & shape)
{
	for (size_t i = 0; i < m_shapes.size(); ++i)
	{
		if (m_shapes[i].editableShape == shape)
		{
			m_selectedShapeIndex = i;
			m_selectedShape = shape;

			return;
		}
	}

	m_selectedShape = nullptr;
}

void CEditableCanvas::DeleteSelection()
{
	if (!m_selectedShape)
	{
		return;
	}

	m_history.AddCommandAndExecute({
		std::bind(&ICanvas::DeleteShape, &m_canvas, m_shapes[m_selectedShapeIndex].shape),
		std::bind(&ICanvas::InsertShape, &m_canvas, m_shapes[m_selectedShapeIndex].shape, m_selectedShapeIndex)
	});

	m_selectedShape = nullptr;
}

std::shared_ptr<IEditableShape> CEditableCanvas::GetSelection()
{	
	return m_shapes[m_selectedShapeIndex].editableShape;
}

size_t CEditableCanvas::GetShapeCount() const
{
	return m_shapes.size();
}

std::shared_ptr<IEditableShape> CEditableCanvas::GetShapeAtIndex(size_t index) const
{
	if (index >= m_shapes.size())
	{
		return nullptr;
	}

	return m_shapes[index].editableShape;
}

void CEditableCanvas::DoOnInsertShape(InsertShapeSignal::slot_type const & handler)
{
	m_insertShape.connect(handler);
}

void CEditableCanvas::DoOnDeleteShape(DeleteShapeSignal::slot_type const & handler)
{
	m_deleteShape.connect(handler);
}

void CEditableCanvas::DoOnChangeShape(ChangeShapeSignal::slot_type const & handler)
{
	m_changeShape.connect(handler);
}

void CEditableCanvas::InsertShape(std::shared_ptr<IShape> const & shape, boost::optional<size_t> pos)
{
	std::shared_ptr<IEditableShape> editableShape = std::make_shared<CEditableShape>(shape);
	editableShape->DoOnShapeChange(std::bind(&CEditableCanvas::ChangeEditableShape, this, std::placeholders::_1));

	if (pos)
	{
		if (*pos > m_shapes.size())
		{
			return;
		}

		m_shapes.insert(m_shapes.begin() + *pos, { shape, editableShape });
	}
	else
	{
		m_shapes.push_back({ shape, editableShape });
	}

	m_insertShape(editableShape, pos);
}

void CEditableCanvas::DeleteShape(std::shared_ptr<IShape> const & shape)
{
	auto it = find_if(m_shapes.begin(), m_shapes.end(),
		[&shape](Data const & data) { return data.shape == shape; });
	if (it == m_shapes.end())
	{
		return;
	}

	m_deleteShape(it->editableShape);
	
	m_shapes.erase(it);
}

void CEditableCanvas::ChangeEditableShape(IEditableShape const * shape)
{
	auto it = find_if(m_shapes.begin(), m_shapes.end(),
		[&shape](Data const & data) { return data.editableShape.get() == shape; });
	if (it == m_shapes.end())
	{
		return;
	}

	m_history.AddCommandAndExecute({
		std::bind(&IShape::SetRect, it->shape, it->editableShape->GetRect()),
		std::bind(&IShape::SetRect, it->shape, it->shape->GetRect())
	});

	m_changeShape(it->editableShape);
}

#pragma warning(disable:4503)
