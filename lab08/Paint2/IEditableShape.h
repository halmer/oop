#pragma once

enum class EditableShapeType
{
	Rectangle,
	Triangle,
	Ellipse,
};

class IEditableShape
{
public:
	virtual void SetRect(CRect const & rect) = 0;
	virtual CRect GetRect() const = 0;
	virtual EditableShapeType GetType() const = 0;
	virtual void Commit() const = 0;

	using ShapeChangeSignal = boost::signals2::signal<void()>;
	virtual void DoOnShapeChange(ShapeChangeSignal::slot_type const & handler) = 0;

	virtual ~IEditableShape() = default;
};
