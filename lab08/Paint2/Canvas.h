#pragma once
#include "ICanvas.h"

class CCanvas : public ICanvas
{
public:
	void InsertShape(std::shared_ptr<IShape> const & shape, boost::optional<size_t> pos) override;
	void DeleteShape(std::shared_ptr<IShape> const & shape) override;
	size_t GetShapeCount() const override;
	std::shared_ptr<IShape> GetShapeAtIndex(size_t index) const override;

	void DoOnInsertShape(InsertShapeSignal::slot_type const & handler) override;
	void DoOnDeleteShape(DeleteShapeSignal::slot_type const & handler) override;

private:
	std::vector<std::shared_ptr<IShape>> m_shapes;
	InsertShapeSignal m_insertShape;
	DeleteShapeSignal m_deleteShape;
};
