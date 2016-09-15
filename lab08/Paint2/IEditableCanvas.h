#pragma once
#include "IShape.h"

class ISelection;

class IEditableCanvas
{
public:
	virtual void AddShape(ShapeType type, CRect const & rect) = 0;
	virtual void SelectShape(std::shared_ptr<IShape> const & shape) = 0;
	virtual void DeleteSelection() = 0;
	virtual std::shared_ptr<ISelection> GetSelection() = 0;
	virtual size_t GetShapeCount() const = 0;
	virtual std::shared_ptr<IShape> GetShapeAtIndex(size_t index) const = 0;

	virtual ~IEditableCanvas() = default;
};
