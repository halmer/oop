#pragma once
#include "IEditableShape.h"

class IEditableCanvas
{
public:
	virtual void AddShape(ShapeType type, CRect const & rect) = 0;
	virtual void SelectShape(std::shared_ptr<IEditableShape> const & shape) = 0;
	virtual void DeleteSelection() = 0;
	virtual std::shared_ptr<IEditableShape> GetSelection() = 0;
	virtual size_t GetShapeCount() const = 0;
	virtual std::shared_ptr<IEditableShape> GetShapeAtIndex(size_t index) const = 0;

	using InsertShapeSignal = boost::signals2::signal<void(std::shared_ptr<IEditableShape> const & shape, boost::optional<size_t> position)>;
	virtual void DoOnInsertShape(InsertShapeSignal::slot_type const & handler) = 0;

	using DeleteShapeSignal = boost::signals2::signal<void(std::shared_ptr<IEditableShape> const & shape)>;
	virtual void DoOnDeleteShape(DeleteShapeSignal::slot_type const & handler) = 0;

	using ChangeShapeSignal = boost::signals2::signal<void(std::shared_ptr<IEditableShape> const & shape)>;
	virtual void DoOnChangeShape(ChangeShapeSignal::slot_type const & handler) = 0;

	virtual ~IEditableCanvas() = default;
};
