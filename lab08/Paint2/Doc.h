#pragma once
#include "IDocument.h"
#include "History.h"
#include "EditableCanvas.h"

class ICanvas;

class CDoc : public ::IDocument
{
public:
	CDoc(std::unique_ptr<ICanvas> && canvas);
	void Save(std::function<void(ICanvas const & canvas)> const & saver) override;
	void Load(std::function<void(ICanvas & canvas)> const & loader) override;
	IEditableCanvas & GetEditableCanvas() override;
	void Undo() override;
	void Redo() override;
	bool CanUndo() const override;
	bool CanRedo() const override;

private:
	CHistory m_history;
	std::unique_ptr<ICanvas> m_canvas;
	CEditableCanvas m_editableCanvas;
};
