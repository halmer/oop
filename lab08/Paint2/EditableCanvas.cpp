#include "stdafx.h"
#include "EditableCanvas.h"
#include "ModelRectangle.h"
#include "ModelTriangle.h"
#include "ModelEllipse.h"
#include "Selection.h"

CEditableCanvas::CEditableCanvas(ICanvas & canvas, CHistory & history)
	: m_canvas(canvas)
	, m_history(history)
	, m_indexSelectedShape(0)
{
}

void CEditableCanvas::AddShape(ShapeType type, CRect const & rect)
{
	std::shared_ptr<IShape> shape;
	if (type == ShapeType::Rectangle)
	{
		shape = std::make_shared<CModelRectangle>(rect);
	}
	else if (type == ShapeType::Triangle)
	{
		shape = std::make_shared<CModelTriangle>(rect);
	}
	else if (type == ShapeType::Ellipse)
	{
		shape = std::make_shared<CModelEllipse>(rect);
	}
	else
	{
		return;
	}

	m_history.AddCommandAndExecute({
		std::bind(&ICanvas::InsertShape, &m_canvas, shape, boost::none),
		std::bind(&ICanvas::DeleteShape, &m_canvas, shape)
	});
}

void CEditableCanvas::SelectShape(std::shared_ptr<IShape> const & shape)
{
	for (size_t i = 0; i < m_canvas.GetShapeCount(); ++i)
	{
		if (m_canvas.GetShapeAtIndex(i) == shape)
		{
			m_indexSelectedShape = i;
			m_selectedShape = shape;
			m_selection = nullptr;

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
		std::bind(&ICanvas::DeleteShape, &m_canvas, m_selectedShape),
		std::bind(&ICanvas::InsertShape, &m_canvas, m_selectedShape, m_indexSelectedShape)
	});

	m_selectedShape = nullptr;
	m_selection = nullptr;
}

std::shared_ptr<ISelection> CEditableCanvas::GetSelection()
{
	m_selection = std::make_shared<CSelection>(m_selectedShape);
	m_selection->DoOnShapeChange([this](){ m_selectedShape->SetRect(m_selection->GetRect()); });
	return m_selection;
}

size_t CEditableCanvas::GetShapeCount() const
{
	return m_canvas.GetShapeCount();
}

std::shared_ptr<IShape> CEditableCanvas::GetShapeAtIndex(size_t index) const
{
	if (m_selection && m_indexSelectedShape == index)
	{
		return m_selection->GetShape();
	}

	return m_canvas.GetShapeAtIndex(index);
}
