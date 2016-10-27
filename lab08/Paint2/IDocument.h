#pragma once

class IEditableCanvas;
class ICanvas;

class IDocument
{
public:
	virtual ICanvas const & GetCanvas() = 0;
	virtual IEditableCanvas & GetEditableCanvas() = 0;
	virtual void Undo() = 0;
	virtual void Redo() = 0;
	virtual bool CanUndo() const = 0;
	virtual bool CanRedo() const = 0;
	virtual void SaveState() = 0;
	virtual bool IsModified() const = 0;

	virtual ~IDocument() = default;
};
