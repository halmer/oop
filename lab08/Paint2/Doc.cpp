#include "stdafx.h"
#include "Doc.h"
#include "ICanvas.h"

CDoc::CDoc(std::unique_ptr<ICanvas> && canvas)
	: m_canvas(move(canvas))
	, m_editableCanvas(*m_canvas, m_history)
{
}

void CDoc::Save(std::function<void(ICanvas const & canvas)> const & saver)
{
	saver(*m_canvas);
}

void CDoc::Load(std::function<void(ICanvas & canvas)> const & loader)
{
	loader(*m_canvas);
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
