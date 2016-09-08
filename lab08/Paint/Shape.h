#pragma once

using DrawCommand = std::function<BOOL(CDC*)>;

enum class OffsetType
{
	AllSide,
	TopLeft,
	TopRight,
	BottomLeft,
	BottomRight,
};

enum class ShapeType
{
	Rectangle,
	Triangle,
	Ellipse,
};

class CShape
{
public:
	CShape(CRect const & rect);
	virtual ~CShape();

	void Offset(CPoint const & delta, OffsetType type);
	CRect GetFrameRect() const;
	CPoint GetBottomRight() const;

	virtual DrawCommand DrawShape() const = 0;
	virtual bool IsPointInShape(CPoint const & point) const = 0;
	virtual ShapeType GetType() const = 0;
	virtual std::shared_ptr<CShape> Clone() const = 0;

protected:
	CRect m_rect;
};
