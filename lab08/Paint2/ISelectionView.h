#pragma once

class IShapeView;

enum class ControlPointType
{
	NorthEast,
	NorthWest,
	SouthEast,
	SouthWest,
};

class ISelectionView
{
public:
	virtual std::shared_ptr<IShapeView> GetShape() = 0;

	using DragSignal = boost::signals2::signal<void(ControlPointType type, CPoint const & coord)>;
	virtual void DoOnControlPointDrag(DragSignal::slot_type const & handler) = 0;

	using DragEndSignal = boost::signals2::signal<void()>;
	virtual void DoOnDragEnd(DragEndSignal::slot_type const & handler) = 0;
	
	virtual ~ISelectionView () = default;
};
