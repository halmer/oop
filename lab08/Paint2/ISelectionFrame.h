#pragma once
#include"signals.h"

enum class ControlPointType
{
	NorthWest,
	NorthEast,
	SouthWest,
	SouthEast,
	NotSelected,
};

class ISelectionFrame
{
public:
	virtual void SetFrame(CRect const & rect) = 0;
	virtual CRect GetFrame() const = 0;
	virtual ControlPointType GetSelectedControlPoint(CPoint const & point) const = 0;
	virtual void DrawFrame(CDC * pDC) = 0;
	virtual bool IsInvertedCursor() const = 0;

	using DragSignal = Signal<void(ControlPointType type, CPoint const & delta)>;
	virtual Connection DoOnControlPointDrag(DragSignal::slot_type const & handler) = 0;

	using DragEndSignal = Signal<void()>;
	virtual Connection DoOnDragEnd(DragEndSignal::slot_type const & handler) = 0;
	
	virtual ~ISelectionFrame () = default;
};
