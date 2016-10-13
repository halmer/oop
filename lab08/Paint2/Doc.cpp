#include "stdafx.h"
#include "Doc.h"
#include "ICanvas.h"

CDoc::CDoc(std::unique_ptr<ICanvas> && canvas)
	: m_canvas(move(canvas))
	, m_editableCanvas(*m_canvas, m_history)
{
}

ICanvas const & CDoc::GetCanvas()
{
	return *m_canvas;
}

IEditableCanvas & CDoc::GetEditableCanvas()
{
	return m_editableCanvas;
}

void CDoc::Undo()
{
	m_history.Undo();
}

void CDoc::Redo()
{
	m_history.Redo();
}

bool CDoc::CanUndo() const
{
	return m_history.CanUndo();
}

bool CDoc::CanRedo() const
{
	return m_history.CanRedo();
}
