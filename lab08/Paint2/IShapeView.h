#pragma once
#include "signals.h"

enum class ShapeViewType
{
	Rectangle,
	Triangle,
	Ellipse,
};

class IShapeView
{
public:
	using MousePressSignal = Signal<void(IShapeView & shapeView)>;
	using MouseReleaseSignal = Signal<void(IShapeView & shapeView, bool releaseInside)>;
	using MouseDragSignal = Signal<void(IShapeView & shapeView, const CRect & targetRect)>;

	virtual Connection DoOnMousePress(const MousePressSignal::slot_type & handler) = 0;
	virtual Connection DoOnMouseRelease(const MouseReleaseSignal::slot_type & handler) = 0;
	virtual Connection DoOnMouseDrag(const MouseDragSignal::slot_type & handler) = 0;

	virtual void SetRect(CRect const & rect) = 0;
	virtual void DrawShape(CDC * pDC) = 0;
	
	virtual ~IShapeView() = default;
};
