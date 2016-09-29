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
	virtual void SetRect(CRect const & rect) = 0;
	virtual void DrawShape(CDC * pDC) = 0;
	
	using MousePressSignal = Signal<void(IShapeView & shapeView)>;
	virtual Connection DoOnMousePress(MousePressSignal::slot_type const & handler) = 0;
	
	using MouseReleaseSignal = Signal<void(IShapeView & shapeView, bool releaseInside)>;
	virtual Connection DoOnMouseRelease(MouseReleaseSignal::slot_type const & handler) = 0;
	
	using MouseDragSignal = Signal<void(IShapeView & shapeView, CRect const & targetRect)>;
	virtual Connection DoOnMouseDrag(MouseDragSignal::slot_type const & handler) = 0;

	using ResizeShapeSignal = Signal<void(IShapeView & shapeView, CRect const & targetRect)>;
	virtual Connection DoOnResizeShape(ResizeShapeSignal::slot_type const & handler) = 0;

	using DeleteShapeSignal = Signal<void(IShapeView & shapeView)>;
	virtual Connection DoOnDeleteShape(DeleteShapeSignal::slot_type const & handler) = 0;

	virtual ~IShapeView() = default;
};
