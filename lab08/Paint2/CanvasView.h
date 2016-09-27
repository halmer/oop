#pragma once
#include "ICanvasView.h"

class CCanvasView : public ICanvasView
{
public:
	std::shared_ptr<IShapeView> InsertShape(ShapeViewType type, CRect const & rect, boost::optional<size_t> position) override;
	void DeleteShape(std::shared_ptr<IShapeView> const & shape) override;
	void SelectShape(std::shared_ptr<IShapeView> const & shape) override;
	std::shared_ptr<ISelectionView> GetSelection() override;
	size_t GetShapeCount() const override;
	std::shared_ptr<IShapeView> GetShapeAtIndex(size_t index) const override;

	CCanvasView();
	void Draw(CDC & dc);

private:
	std::vector<std::shared_ptr<IShapeView>> m_shapes;
	std::shared_ptr<IShapeView> m_selectedShape;
	CPen m_pen;
	CBrush m_brush;
};
