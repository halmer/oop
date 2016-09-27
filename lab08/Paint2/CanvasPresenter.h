#pragma once
#include "ICommandSourceDelegate.h"

class IDocument;
class IEditableCanvas;
class ICanvasView;
class IEditableShape;
class IShapeView;
class IPaint2View;

class CCanvasPresenter : public ICommandSourceDelegate
{
public:
	CCanvasPresenter(::IDocument & doc, ICanvasView & canvasView);
	void InitView(IPaint2View * view);

	void OnCreateRectangle() override;
	void OnCreateTriangle() override;
	void OnCreateEllipse() override;
	void OnUndo() override;
	void OnRedo() override;
	void OnLButtonDown(UINT nFlags, CPoint point) override;
	void OnLButtonUp(UINT nFlags, CPoint point) override;
	void OnMouseMove(UINT nFlags, CPoint point) override;
	void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) override;
	void OnUpdateUndo(CCmdUI * pCmdUI) override;
	void OnUpdateRedo(CCmdUI * pCmdUI) override;

private:
	void InsertShape(std::shared_ptr<IEditableShape> const & shape, boost::optional<size_t> pos);
	void DeleteShape(std::shared_ptr<IEditableShape> const & shape);
	void ChangeShape(std::shared_ptr<IEditableShape> const & shape);
	CRect GetDefaultRect() const;

	struct Data
	{
		std::shared_ptr<IEditableShape> editableShape;
		std::shared_ptr<IShapeView> shapeView;
	};

	::IDocument & m_doc;
	IEditableCanvas & m_canvas;
	ICanvasView & m_canvasView;
	IPaint2View * m_view;
	std::vector<Data> m_shapes;
	std::shared_ptr<IShapeView> m_selectedShape;
	boost::optional<CPoint> m_lastPoint;
};
