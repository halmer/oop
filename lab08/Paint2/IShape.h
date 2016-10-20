#pragma once

enum class ShapeType
{
	Rectangle,
	Triangle,
	Ellipse,
	Image,
};

class IShape
{
public:
	virtual void SetRect(CRect const & rect) = 0;
	virtual CRect GetRect() const = 0;
	virtual ShapeType GetType() const = 0;
	virtual HANDLE GetBitmap() const = 0;

	using ShapeChangeSignal = boost::signals2::signal<void(IShape const * shape)>;
	virtual boost::signals2::connection DoOnShapeChange(ShapeChangeSignal::slot_type const & handler) = 0;

	virtual ~IShape() = default;
};
