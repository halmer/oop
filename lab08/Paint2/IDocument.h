#pragma once

class IEditableCanvas;
class ICanvas;

class IDocument
{
public:
	virtual void Save(std::function<void(ICanvas const & canvas)> const & saver) = 0;
	virtual void Load(std::function<void(ICanvas & canvas)> const & loader) = 0;
	virtual IEditableCanvas & GetEditableCanvas() = 0;
	virtual void Undo() = 0;
	virtual void Redo() = 0;
	virtual bool CanUndo() const = 0;
	virtual bool CanRedo() const = 0;

	virtual ~IDocument() = default;
};
