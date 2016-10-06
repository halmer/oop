#pragma once

class IEditableCanvas;

class IDocument
{
public:
	virtual void Save(const std::function<void(const ICanvas & canvas)> & saver) = 0;

	virtual void NewDocument() = 0;
	virtual IEditableCanvas & GetCanvas() = 0;
	virtual void Undo() = 0;
	virtual void Redo() = 0;
	virtual bool CanUndo() const = 0;
	virtual bool CanRedo() const = 0;

	virtual ~IDocument() = default;
};
