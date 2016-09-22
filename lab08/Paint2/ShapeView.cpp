#include "stdafx.h"
#include "ShapeView.h"

using namespace std;

CShapeView::CShapeView(CRect const & rect, ShapeType type)
	: m_rect(rect)
	, m_type(type)
{
}

void CShapeView::SetRect(CRect const & rect)
{
	m_rect = rect;
}

static map<ShapeType, function<void(CDC*, CRect)>> const drawingShapes{
	{ ShapeType::Rectangle,
		bind(static_cast<BOOL(CDC::*)(LPCRECT)>(&CDC::Rectangle), placeholders::_1, placeholders::_2)
	},
	{ ShapeType::Ellipse,
		bind(static_cast<BOOL(CDC::*)(LPCRECT)>(&CDC::Ellipse), placeholders::_1, placeholders::_2)
	},
	{ ShapeType::Triangle,
		[](CDC * pDC, CRect rect) {
			vector<CPoint> points;
			points.reserve(3);
			points.emplace_back(rect.left, rect.bottom);
			points.emplace_back((rect.left + rect.right) / 2, rect.top);
			points.emplace_back(rect.right, rect.bottom);
			pDC->Polygon(points.data(), points.size());
		}
	}
};

void CShapeView::DrawShape(CDC * pDC)
{
	auto it = drawingShapes.find(m_type);
	if (it != drawingShapes.end())
	{
		it->second(pDC, m_rect);
	}
}

static map<ShapeType, function<bool(CRect const &, CPoint const &)>> const pointInShape{
	{ ShapeType::Rectangle,
		[](CRect const & rect, CPoint const & point) {
			CRgn rgn;
			rgn.CreateRectRgnIndirect(&rect);
			return rgn.PtInRegion(point) != 0;
		}
	},
	{ ShapeType::Ellipse,
		[](CRect const & rect, CPoint const & point) {
			CRgn rgn;
			rgn.CreateEllipticRgnIndirect(&rect);
			return rgn.PtInRegion(point) != 0;
		}
	},
	{ ShapeType::Triangle,
		[](CRect const & rect, CPoint const & point) {
			vector<CPoint> points;
			points.reserve(3);
			points.emplace_back(rect.left, rect.bottom);
			points.emplace_back((rect.left + rect.right) / 2, rect.top);
			points.emplace_back(rect.right, rect.bottom);
			CRgn rgn;
			rgn.CreatePolygonRgn(points.data(), points.size(), WINDING);
			return rgn.PtInRegion(point) != 0;
		}
	}
};

bool CShapeView::IsPointInShape(CPoint const & point) const
{
	auto it = pointInShape.find(m_type);
	if (it != pointInShape.end())
	{
		if (it->second(m_rect, point))
		{
			return true;
		}
	}
	
	return false;
}
