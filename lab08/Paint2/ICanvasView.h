#pragma once

class IShapeView;
class ISelectionView;

enum class ShapeViewType
{
	Rectangle,
	Triangle,
	Ellipse,
};

class ICanvasView
{
public:
	virtual std::shared_ptr<IShapeView> InsertShape(ShapeViewType type, CRect const & frame, size_t position) = 0;
	virtual void DeleteShape(std::shared_ptr<IShapeView> const & shape) = 0;
	virtual void SelectShape(std::shared_ptr<IShapeView> const & shape) = 0;
	virtual std::shared_ptr<ISelectionView> GetSelection() = 0;

	virtual ~ICanvasView() = default;
};
