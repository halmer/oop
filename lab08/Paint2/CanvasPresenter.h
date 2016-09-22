#pragma once
#include "ICommandSourceDelegate.h"

class IDocument;
class IEditableCanvas;

class CCanvasPresenter : public ICommandSourceDelegate
{
public:
	CCanvasPresenter(::IDocument & doc);
	void OnCreateRectangle() override;
	void OnCreateTriangle() override;
	void OnCreateEllipse() override;
	void OnUndo() override;
	void OnRedo() override;

private:
	::IDocument & m_doc;
	IEditableCanvas & m_canvas;
};
