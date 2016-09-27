#pragma once
#include "IShapeView.h"

class ISelectionView;

class ICanvasView
{
public:
	virtual std::shared_ptr<IShapeView> InsertShape(ShapeViewType type, CRect const & rect, boost::optional<size_t> position) = 0;
	virtual void DeleteShape(std::shared_ptr<IShapeView> const & shape) = 0;
	virtual void SelectShape(std::shared_ptr<IShapeView> const & shape) = 0;
	virtual std::shared_ptr<ISelectionView> GetSelection() = 0;
	virtual size_t GetShapeCount() const = 0;
	virtual std::shared_ptr<IShapeView> GetShapeAtIndex(size_t index) const = 0;

	virtual ~ICanvasView() = default;
};
