#pragma once

enum class ShapeType
{
	Rectangle,
	Triangle,
	Ellipse,
};

class IShape
{
public:
	virtual void SetRect(CRect const & rect) = 0;
	virtual CRect GetRect() const = 0;
	virtual ShapeType GetType() const = 0;

	virtual ~IShape() = default;
};
