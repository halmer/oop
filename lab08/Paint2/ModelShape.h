#pragma once
#include "IShape.h"

class CModelShape : public IShape
{
public:
	CModelShape(CRect const & rect);
	void SetRect(CRect const & rect) override;
	CRect GetRect() const override;

private:
	CRect m_rect;
};
