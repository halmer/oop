#pragma once

#define REDRAW_UPDATESCROLL 0
#define REDRAW_UPDATESCROLL_RESETSELECT 1
#define REDRAW 2

class IPaintView
{
public:
	using CreateShapeSignal = boost::signals2::signal<void(CRect const & rect)>;
	using ShapeSelectionSignal = boost::signals2::signal<void(std::shared_ptr<CShape> const & shape)>;
	using OffsetSelectedShapeSignal = boost::signals2::signal<void(OffsetType type, CPoint const & point)>;
	using LButtonUpSignal = boost::signals2::signal<void(OffsetType type)>;
	using UndoRedoSignal = boost::signals2::signal<void()>;
	using KeyDeleteDownSignal = boost::signals2::signal<void()>;

	virtual void DoOnButtonRectangle(CreateShapeSignal::slot_type const & slot) = 0;
	virtual void DoOnButtonTriangle(CreateShapeSignal::slot_type const & slot) = 0;
	virtual void DoOnButtonEllipse(CreateShapeSignal::slot_type const & slot) = 0;
	virtual void DoOnShapeSelection(ShapeSelectionSignal::slot_type const & slot) = 0;
	virtual void DoOnMouseMoveWithLButtonDown(OffsetSelectedShapeSignal::slot_type const & slot) = 0;
	virtual void DoOnLButtonUp(LButtonUpSignal::slot_type const & slot) = 0;
	virtual void DoOnButtonUndo(UndoRedoSignal::slot_type const & slot) = 0;
	virtual void DoOnButtonRedo(UndoRedoSignal::slot_type const & slot) = 0;
	virtual void DoOnKeyDeleteDown(KeyDeleteDownSignal::slot_type const & slot) = 0;

	virtual void SetMovingShape(std::shared_ptr<CShape> const & shape) = 0;

	virtual ~IPaintView() = default;
};
