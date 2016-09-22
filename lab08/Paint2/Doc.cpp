#include "stdafx.h"
#include "Doc.h"
#include "EditableCanvas.h"

CDoc::CDoc(ICanvas & canvas)
	: m_canvas(canvas)
	, m_editableCanvas(m_canvas, m_history)
{
}

IEditableCanvas & CDoc::GetCanvas()
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
