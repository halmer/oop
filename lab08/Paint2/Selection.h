#pragma once
#include "ISelection.h"

class CSelection : public ISelection
{
public:
	CSelection(std::shared_ptr<IShape> const & shape);
	std::shared_ptr<IShape> GetShape() override;
	void MoveControlPoint(SelectControlPointType type, CPoint const & coord) override;
	void SetRect(CRect const & rect) override;
	CRect GetRect() const override;
	void Commit() const override;

	void DoOnShapeChange(ShapeChangeSignal::slot_type const & handler) override;

public:
	std::shared_ptr<IShape> m_shape;
	ShapeChangeSignal m_shapeChange;
};
