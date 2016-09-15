#include "stdafx.h"
#include "ModelShape.h"

CModelShape::CModelShape(CRect const & rect)
	: m_rect(rect)
{
}

void CModelShape::SetRect(CRect const & rect)
{
	m_rect = rect;
}

CRect CModelShape::GetRect() const
{
	return m_rect;
}
