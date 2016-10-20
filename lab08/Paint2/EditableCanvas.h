#pragma once
#include "IEditableCanvas.h"

class ICanvas;
class IHistory;

class CEditableCanvas : public IEditableCanvas
{
public:
	CEditableCanvas(ICanvas & canvas, IHistory & history);
	void AddShape(ShapeType type, CRect const & rect, HANDLE bitmap = nullptr) override;
	void SelectShape(std::shared_ptr<IEditableShape> const & shape) override;
	void DeleteSelection() override;
	std::shared_ptr<IEditableShape> GetSelection() override;
	size_t GetShapeCount() const override;
	std::shared_ptr<IEditableShape> GetShapeAtIndex(size_t index) const override;

	void DoOnInsertShape(InsertShapeSignal::slot_type const & handler) override;
	void DoOnDeleteShape(DeleteShapeSignal::slot_type const & handler) override;
	void DoOnChangeShape(ChangeShapeSignal::slot_type const & handler) override;

private:
	void InsertShape(std::shared_ptr<IShape> const & shape, boost::optional<size_t> pos);
	void DeleteShape(std::shared_ptr<IShape> const & shape);
	void ChangeEditableShape(IEditableShape const * shape);

	struct Data
	{
		std::shared_ptr<IShape> shape;
		std::shared_ptr<IEditableShape> editableShape;
	};
	
	ICanvas & m_canvas;
	IHistory & m_history;
	std::vector<Data> m_shapes;
	std::shared_ptr<IEditableShape> m_selectedShape;
	InsertShapeSignal m_insertShape;
	DeleteShapeSignal m_deleteShape;
	ChangeShapeSignal m_changeShape;
};
