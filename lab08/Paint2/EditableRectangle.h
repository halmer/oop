#pragma once
#include "EditableShape.h"

class CEditableRectangle : public CEditableShape
{
public:
	CEditableRectangle(CRect const & rect);
	EditableShapeType GetType() const override;
};
