#pragma once
#include "IShape.h"

class CShape : public IShape
{
public:
	CShape(CRect const & rect);
	void SetRect(CRect const & rect) override;
	CRect GetRect() const override;

private:
	CRect m_rect;
};
