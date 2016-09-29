#pragma once
#include "ICanvasView.h"
#include "SelectionFrame.h"

class CShapeView;

class CCanvasView : public ICanvasView
{
public:
	CCanvasView();

	std::shared_ptr<IShapeView> InsertShape(ShapeViewType type, CRect const & rect, boost::optional<size_t> position) override;
	void DeleteShape(std::shared_ptr<IShapeView> const & shape) override;
	void DeleteSelection() override;
	void SelectShape(std::shared_ptr<IShapeView> const & shape) override;
	std::shared_ptr<ISelectionFrame> GetSelection() override;
	size_t GetShapeCount() const override;
	std::shared_ptr<IShapeView> GetShapeAtIndex(size_t index) const override;
	CRect GetSize() const override;

	void Draw(CDC & dc);
	void HandleMouseDown(CPoint const & point);
	void HandleMouseUp(CPoint const & point);
	void HandleMouseMove(UINT nFlags, CPoint const & point);
	void HandleOnKeyDown(UINT nChar);

private:
	std::vector<std::shared_ptr<CShapeView>> m_shapes;
	std::shared_ptr<IShapeView> m_selectedShape;
	std::shared_ptr<CShapeView> m_capturedShape;
	CPen m_pen;
	CBrush m_brush;
	boost::optional<CPoint> m_mousePressPoint;
	CSelectionFrame m_frame;
	ControlPointType m_selectedControlPoint;
};
