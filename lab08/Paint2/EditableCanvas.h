#pragma once
#include "IEditableCanvas.h"
#include "ICanvas.h"
#include "../Paint/History.h"
#include "ISelection.h"

class CEditableCanvas : public IEditableCanvas
{
public:
	CEditableCanvas(ICanvas & canvas, CHistory & history);
	void AddShape(ShapeType type, CRect const & rect) override;
	void SelectShape(std::shared_ptr<IShape> const & shape) override;
	void DeleteSelection() override;
	std::shared_ptr<ISelection> GetSelection() override;
	size_t GetShapeCount() const override;
	std::shared_ptr<IShape> GetShapeAtIndex(size_t index) const override;

private:
	ICanvas & m_canvas;
	CHistory & m_history;
	std::shared_ptr<IShape> m_selectedShape;
	size_t m_indexSelectedShape;
	std::shared_ptr<ISelection> m_selection;
};
