#pragma once
#include "IEditableShape.h"

class CHistory;

class CEditableShape : public IEditableShape
{
public:
	CEditableShape(std::shared_ptr<IShape> const & shape, CHistory & history);
	~CEditableShape();
	void SetRect(CRect const & rect) override;
	void Offset(CPoint const & delta, OffsetType type) override;
	CRect GetRect() const override;
	ShapeType GetType() const override;
	void Commit() override;

	boost::signals2::connection DoOnShapeChange(ShapeChangeSignal::slot_type const & handler) override;

private:
	CRect m_rect;
	std::shared_ptr<IShape> m_shape;
	CHistory & m_history;
	ShapeChangeSignal m_shapeChange;
	boost::signals2::connection m_connection;
};
