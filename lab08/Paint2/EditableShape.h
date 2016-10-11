#pragma once
#include "IEditableShape.h"
#include "signals.h"

class IHistory;

class CEditableShape : public IEditableShape
{
public:
	CEditableShape(std::shared_ptr<IShape> const & shape, IHistory & history);
	void SetRect(CRect const & rect) override;
	void Offset(CPoint const & delta, OffsetType type) override;
	CRect GetRect() const override;
	ShapeType GetType() const override;
	void Commit() override;

	boost::signals2::connection DoOnShapeChange(ShapeChangeSignal::slot_type const & handler) override;

private:
	CRect m_rect;
	std::shared_ptr<IShape> m_shape;
	IHistory & m_history;
	ShapeChangeSignal m_shapeChange;
	ScopedConnection m_connection;
};
