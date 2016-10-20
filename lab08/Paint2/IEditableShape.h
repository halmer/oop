#pragma once
#include "IShape.h"

enum class OffsetType
{
	AllSide,
	TopLeft,
	TopRight,
	BottomLeft,
	BottomRight,
};

class IEditableShape
{
public:
	virtual void SetRect(CRect const & rect) = 0;
	virtual void Offset(CPoint const & delta, OffsetType type) = 0;
	virtual CRect GetRect() const = 0;
	virtual ShapeType GetType() const = 0;
	virtual HANDLE GetBitmap() const = 0;
	virtual void Commit() = 0;

	using ShapeChangeSignal = boost::signals2::signal<void(IEditableShape const * shape)>;
	virtual boost::signals2::connection DoOnShapeChange(ShapeChangeSignal::slot_type const & handler) = 0;

	virtual ~IEditableShape() = default;
};
