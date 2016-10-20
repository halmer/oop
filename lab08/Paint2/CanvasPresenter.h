#pragma once
#include "ICommandSourceDelegate.h"
#include "ISelectionFrame.h"

class IDocument;
class IEditableCanvas;
class ICanvasView;
class IEditableShape;
class IShapeView;
class ICanvasViewOwner;

class CCanvasPresenter : public ICommandSourceDelegate
{
public:
	CCanvasPresenter(std::shared_ptr<::IDocument> const & doc, ICanvasViewOwner & viewOwner);
private:
	void OnCreateRectangle() override;
	void OnCreateTriangle() override;
	void OnCreateEllipse() override;
	void OnInsertImage() override;
	void OnUndo() override;
	void OnRedo() override;
	void OnUpdateUndo(CCmdUI * pCmdUI) override;
	void OnUpdateRedo(CCmdUI * pCmdUI) override;

private:
	void InsertShape(std::shared_ptr<IEditableShape> const & shape, boost::optional<size_t> pos);
	void DeleteShape(std::shared_ptr<IEditableShape> const & shape);
	void OffsetShape(ControlPointType type, CPoint const & delta);
	CRect GetDefaultRect() const;

	struct Data
	{
		std::shared_ptr<IEditableShape> editableShape;
		std::shared_ptr<IShapeView> shapeView;
	};

	std::shared_ptr<::IDocument> m_doc;
	IEditableCanvas & m_editableCanvas;
	std::shared_ptr<ICanvasView> m_canvasView;
	ICanvasViewOwner & m_viewOwner;
	std::vector<Data> m_shapes;
};
