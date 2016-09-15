#pragma once

class IShape;

enum class SelectControlPointType
{
	NorthEast,
	NorthWest,
	SouthEast,
	SouthWest,
};

class ISelection
{
public:
	virtual std::shared_ptr<IShape> GetShape() = 0;
	virtual void MoveControlPoint(SelectControlPointType type, CPoint const & coord) = 0;
	virtual void SetRect(CRect const & rect) = 0;
	virtual CRect GetRect() const = 0;
	virtual void Commit() const = 0;

	using ShapeChangeSignal = boost::signals2::signal<void()>;
	virtual void DoOnShapeChange(ShapeChangeSignal::slot_type const & handler) = 0;

	virtual ~ISelection() = default;
};
