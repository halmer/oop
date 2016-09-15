#include "stdafx.h"
#include "EditableRectangle.h"

CEditableRectangle::CEditableRectangle(CRect const & rect)
	: CEditableShape(rect)
{
}

EditableShapeType CEditableRectangle::GetType() const
{
	return EditableShapeType::Rectangle;
}
