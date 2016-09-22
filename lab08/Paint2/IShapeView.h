#pragma once

class IShapeView
{
public:
	virtual void SetRect(CRect const & rect) = 0;
	virtual void DrawShape(CDC * pDC) = 0;
	virtual bool IsPointInShape(CPoint const & point) const = 0;
	
	virtual ~IShapeView() = default;
};
