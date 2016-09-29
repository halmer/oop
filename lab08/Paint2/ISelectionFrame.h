#pragma once

enum class ControlPointType
{
	NorthEast,
	NorthWest,
	SouthEast,
	SouthWest,
	NotSelected,
};

class ISelectionFrame
{
public:
	virtual void SetFrame(CRect const & rect) = 0;
	virtual void ResizeFrame(ControlPointType type, CPoint const & delta) = 0;
	virtual CRect GetFrame() const = 0;
	virtual ControlPointType GetSelectedControlPoint(CPoint const & point) const = 0;
	virtual void DrawFrame(CDC * pDC) = 0;
	
	virtual ~ISelectionFrame () = default;
};
