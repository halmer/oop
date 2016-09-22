#pragma once
#include "IDocument.h"
#include "ICanvas.h"
#include "EditableCanvas.h"
#include "../Paint/History.h"

class CDoc : public ::IDocument
{
public:
	CDoc(ICanvas & canvas);
	IEditableCanvas & GetCanvas() override;
	void Undo() override;
	void Redo() override;
	bool CanUndo() const override;
	bool CanRedo() const override;

private:
	CHistory m_history;
	ICanvas & m_canvas;
	CEditableCanvas m_editableCanvas;
};
