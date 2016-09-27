#include "stdafx.h"
#include "CanvasPresenter.h"
#include "IDocument.h"
#include "IEditableCanvas.h"
#include "IShapeView.h"
#include "ICanvasView.h"
#include "IPaint2View.h"

const int WIDTH = 50;
const int HEIGHT = 50;

CCanvasPresenter::CCanvasPresenter(::IDocument & doc, ICanvasView & canvasView)
	: m_doc(doc)
	, m_canvas(doc.GetCanvas())
	, m_canvasView(canvasView)
{
	m_canvas.DoOnInsertShape(std::bind(&CCanvasPresenter::InsertShape, this, std::placeholders::_1, std::placeholders::_2));
	m_canvas.DoOnDeleteShape(std::bind(&CCanvasPresenter::DeleteShape, this, std::placeholders::_1));
	m_canvas.DoOnChangeShape(std::bind(&CCanvasPresenter::ChangeShape, this, std::placeholders::_1));
}

void CCanvasPresenter::InitView(IPaint2View * view)
{
	m_view = view;
}

void CCanvasPresenter::OnCreateRectangle()
{
	m_canvas.AddShape(ShapeType::Rectangle, GetDefaultRect());
}

void CCanvasPresenter::OnCreateTriangle()
{
	m_canvas.AddShape(ShapeType::Triangle, GetDefaultRect());
}

void CCanvasPresenter::OnCreateEllipse()
{
	m_canvas.AddShape(ShapeType::Ellipse, GetDefaultRect());
}

void CCanvasPresenter::OnUndo()
{
	m_doc.Undo();
}

void CCanvasPresenter::OnRedo()
{
	m_doc.Redo();
}

/*
void CCanvasPresenter::OnLButtonDown(UINT / *nFlags* /, CPoint point)
{
	for (auto const & item : boost::adaptors::reverse(m_shapes))
	{
		if (item.shapeView->IsPointInShape(point))
		{
			m_selectedShape = item.shapeView;
			m_canvasView.SelectShape(m_selectedShape);
			m_canvas.SelectShape(item.editableShape);
			return;
		}
	}

	m_selectedShape = nullptr;
	m_canvasView.SelectShape(nullptr);
	m_canvas.SelectShape(nullptr);
}
*/

/*
void CCanvasPresenter::OnLButtonUp(UINT / *nFlags* /, CPoint / *point* /)
{
	if (m_selectedShape)
	{
		m_canvas.GetSelection()->Commit();
		m_lastPoint = boost::none;
	}
}
*/

/*
void CCanvasPresenter::OnMouseMove(UINT nFlags, CPoint point)
{
	if (nFlags & MK_LBUTTON && m_selectedShape)
	{
		if (!m_lastPoint)
		{
			m_lastPoint = point;
		}
		
		auto selection = m_canvas.GetSelection();
		selection->Offset(point - *m_lastPoint, OffsetType::AllSide);
		m_selectedShape->SetRect(selection->GetRect());

		m_lastPoint = point;
	}
}

*/
void CCanvasPresenter::OnKeyDown(UINT nChar, UINT /*nRepCnt*/, UINT /*nFlags*/)
{
	if ((nChar == VK_DELETE) && m_selectedShape)
	{
		m_canvas.DeleteSelection();
		m_canvasView.DeleteShape(m_selectedShape);
		m_selectedShape = nullptr;
	}
}

void CCanvasPresenter::OnUpdateUndo(CCmdUI * pCmdUI)
{
	pCmdUI->Enable(m_doc.CanUndo());
}

void CCanvasPresenter::OnUpdateRedo(CCmdUI * pCmdUI)
{
	pCmdUI->Enable(m_doc.CanRedo());
}

void CCanvasPresenter::InsertShape(std::shared_ptr<IEditableShape> const & shape, boost::optional<size_t> pos)
{
	std::shared_ptr<IShapeView> shapeView;
	switch (shape->GetType())
	{
	case ShapeType::Rectangle:
		shapeView = m_canvasView.InsertShape(ShapeViewType::Rectangle, shape->GetRect(), pos);
		break;
	case ShapeType::Triangle:
		shapeView = m_canvasView.InsertShape(ShapeViewType::Triangle, shape->GetRect(), pos);
		break;
	case ShapeType::Ellipse:
		shapeView = m_canvasView.InsertShape(ShapeViewType::Ellipse, shape->GetRect(), pos);
		break;
	}

	struct Context
	{
		bool moved = false;
	};

	auto context = std::make_shared<Context>();

	shapeView->DoOnMousePress([=](IShapeView & /*shape*/) {
		m_selectedShape = shapeView;
		m_canvas.SelectShape(shape);
	});

	shapeView->DoOnMouseRelease([=](IShapeView & /*shape*/, bool /*releaseInside*/) {
		if (context->moved)
		{
			shape->Commit();
		}
		context->moved = false;
	});

	shapeView->DoOnMouseDrag([=](IShapeView & /*shapeView*/, const CRect & targetRect) {
		shape->SetRect(targetRect);
		context->moved = true;
	});

	shape->DoOnShapeChange([=](IEditableShape const * shape) {
		shapeView->SetRect(shape->GetRect());
	});


	if (pos)
	{
		if (*pos > m_shapes.size())
		{
			return;
		}

		m_shapes.insert(m_shapes.begin() + *pos, { shape, shapeView });
	}
	else
	{
		m_shapes.push_back({ shape, shapeView });
	}
}

void CCanvasPresenter::DeleteShape(std::shared_ptr<IEditableShape> const & shape)
{
	auto it = find_if(m_shapes.begin(), m_shapes.end(),
		[&shape](Data const & data) { return data.editableShape == shape; });
	if (it == m_shapes.end())
	{
		return;
	}

	m_canvasView.DeleteShape(it->shapeView);
	m_shapes.erase(it);
}

void CCanvasPresenter::ChangeShape(std::shared_ptr<IEditableShape> const & shape)
{
	auto it = find_if(m_shapes.begin(), m_shapes.end(),
		[&shape](Data const & data) { return data.editableShape == shape; });
	if (it == m_shapes.end())
	{
		return;
	}

	//it->shapeView->SetRect(shape->GetRect());

	m_selectedShape = nullptr;
	m_canvasView.SelectShape(nullptr);
	m_canvas.SelectShape(nullptr);
}

CRect CCanvasPresenter::GetDefaultRect() const
{
	auto point = m_view->GetPointInViewCenter();
	point.Offset(-WIDTH / 2, -HEIGHT / 2);

	return { point, CSize(WIDTH, HEIGHT) };
}
