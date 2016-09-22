#include "stdafx.h"
#include "CanvasPresenter.h"
#include "IEditableCanvas.h"
#include "IDocument.h"

CCanvasPresenter::CCanvasPresenter(::IDocument & doc)
	: m_doc(doc)
	, m_canvas(doc.GetCanvas())
{
}

void CCanvasPresenter::OnCreateRectangle()
{
	m_canvas.AddShape(ShapeType::Rectangle, CRect(10, 10, 10, 10));
}

void CCanvasPresenter::OnCreateTriangle()
{
	m_canvas.AddShape(ShapeType::Triangle, CRect(10, 10, 10, 10));
}

void CCanvasPresenter::OnCreateEllipse()
{
	m_canvas.AddShape(ShapeType::Ellipse, CRect(10, 10, 10, 10));
}

void CCanvasPresenter::OnUndo()
{
	m_doc.Undo();
}

void CCanvasPresenter::OnRedo()
{
	m_doc.Redo();
}
