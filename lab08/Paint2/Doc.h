#pragma once
#include "IDocument.h"
#include "ICanvas.h"
#include "EditableCanvas.h"
#include "../Paint/History.h"

class CDoc : public ::IDocument
{
public:
	CDoc(std::unique_ptr<ICanvas> && canvas);

	void Save(const std::function<void(const ICanvas & canvas)> & saver) override;
	
	IEditableCanvas & GetEditableCanvas() override;
	void Undo() override;
	void Redo() override;
	bool CanUndo() const override;
	bool CanRedo() const override;

private:
	std::unique_ptr<ICanvas> m_canvas;
	CHistory m_history;
	CEditableCanvas m_editableCanvas;
};
