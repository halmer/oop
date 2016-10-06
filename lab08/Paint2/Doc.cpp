#include "stdafx.h"
#include "Doc.h"
#include "EditableCanvas.h"

CDoc::CDoc(std::unique_ptr<ICanvas> && canvas)
	: m_canvas(move(canvas))
	, m_editableCanvas(*m_canvas, m_history)
{
}

void CDoc::Save(const std::function<void(const ICanvas & canvas)> & saver)
{
	saver(*m_canvas);
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
