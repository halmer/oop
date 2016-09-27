#pragma once
#include "ICanvasView.h"

class CShapeView;

class CCanvasView : public ICanvasView
{
public:
	CCanvasView();

	std::shared_ptr<IShapeView> InsertShape(ShapeViewType type, CRect const & rect, boost::optional<size_t> position) override;
	void DeleteShape(std::shared_ptr<IShapeView> const & shape) override;
	void SelectShape(std::shared_ptr<IShapeView> const & shape) override;
	std::shared_ptr<ISelectionView> GetSelection() override;
	size_t GetShapeCount() const override;
	std::shared_ptr<IShapeView> GetShapeAtIndex(size_t index) const override;

	void Draw(CDC & dc);
	void HandleMouseDown(const CPoint & point);
	void HandleMouseUp(const CPoint & point);
	void HandleMouseMove(UINT nFlags, const CPoint & point);

private:
	std::vector<std::shared_ptr<CShapeView>> m_shapes;
	std::shared_ptr<IShapeView> m_selectedShape;
	std::shared_ptr<CShapeView> m_capturedShape;
	CPen m_pen;
	CBrush m_brush;
	boost::optional<CPoint> m_mousePressPoint;
};
