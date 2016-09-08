#pragma once
#include "Shape.h"

class CModel
{
public:
	using AddShapeSignal = boost::signals2::signal<void(std::shared_ptr<CShape> const & shape, boost::optional<size_t> position)>;
	using DeleteShapeSignal = boost::signals2::signal<void(boost::optional<size_t> position)>;
	using DeleteAllShapesSignal = boost::signals2::signal<void()>;
	using OffsetShapeSignal = boost::signals2::signal<void(std::shared_ptr<CShape> const & shape, size_t position)>;

	void AddShape(std::shared_ptr<CShape> const & shape, boost::optional<size_t> position);
	void DeleteShape(boost::optional<size_t> position);
	void DeleteAllShapes();
	void OffsetShape(std::shared_ptr<CShape> const & shape, CPoint const & delta, OffsetType type);

	void DoOnAddShape(AddShapeSignal::slot_type const & slot);
	void DoOnDeleteShape(DeleteShapeSignal::slot_type const & slot);
	void DoOnDeleteAllShapes(DeleteAllShapesSignal::slot_type const & slot);
	void DoOnOffsetShape(OffsetShapeSignal::slot_type const & slot);

private:
	std::vector<std::shared_ptr<CShape>> m_shapes;

	AddShapeSignal m_addShape;
	DeleteShapeSignal m_deleteShape;
	DeleteAllShapesSignal m_deleteAllShapes;
	OffsetShapeSignal m_offsetShape;
};
