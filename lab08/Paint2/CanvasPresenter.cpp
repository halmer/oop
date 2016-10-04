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

	auto & frame = m_canvasView.GetSelectionFrame();
	frame.DoOnControlPointDrag(std::bind(&CCanvasPresenter::OffsetShape, this, std::placeholders::_1, std::placeholders::_2));
	frame.DoOnDragEnd([this]() {
		auto shape = m_canvas.GetSelection();
		shape->Commit();
	});
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
	m_canvasView.SelectShape(nullptr);
}

void CCanvasPresenter::OnRedo()
{
	m_doc.Redo();
	m_canvasView.SelectShape(nullptr);
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

	shapeView->DoOnResizeShape([=](IShapeView & /*shapeView*/, const CRect & targetRect) {
		shape->SetRect(targetRect);
		context->moved = true;
	});

	shapeView->DoOnDeleteShape([=](IShapeView & /*shape*/) {
		m_canvas.SelectShape(shape);
		m_canvas.DeleteSelection();
	});

	std::weak_ptr<IShapeView> shapeViewWeak(shapeView);
	shape->DoOnShapeChange([=](IEditableShape const * shape) {
		if (auto shapeView = shapeViewWeak.lock())
		{
			shapeView->SetRect(shape->GetRect());
		}
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

void CCanvasPresenter::OffsetShape(ControlPointType type, CPoint const & delta)
{
	auto shape = m_canvas.GetSelection();

	switch (type)
	{
	case ControlPointType::NorthWest:
		shape->Offset(delta, OffsetType::TopLeft);
		break;
	case ControlPointType::NorthEast:
		shape->Offset(delta, OffsetType::TopRight);
		break;
	case ControlPointType::SouthWest:
		shape->Offset(delta, OffsetType::BottomLeft);
		break;
	case ControlPointType::SouthEast:
		shape->Offset(delta, OffsetType::BottomRight);
		break;
	case ControlPointType::NotSelected:
		break;
	}
}

CRect CCanvasPresenter::GetDefaultRect() const
{
	auto point = m_view->GetPointInViewCenter();
	point.Offset(-WIDTH / 2, -HEIGHT / 2);

	return { point, CSize(WIDTH, HEIGHT) };
}
