#include "stdafx.h"
#include "CanvasPresenter.h"
#include "IDocument.h"
#include "IEditableCanvas.h"
#include "ICanvasView.h"
#include "IShapeView.h"
#include "IPaint2View.h"
#include <afxdlgs.h>

const int WIDTH = 50;
const int HEIGHT = 50;

CCanvasPresenter::CCanvasPresenter(std::shared_ptr<::IDocument> const & doc, ICanvasViewOwner & viewOwner)
	: m_doc(doc)
	, m_editableCanvas(doc->GetEditableCanvas())
	, m_canvasView(viewOwner.GetCanvasView())
	, m_viewOwner(viewOwner)
{
	m_editableCanvas.DoOnInsertShape(std::bind(&CCanvasPresenter::InsertShape, this, std::placeholders::_1, std::placeholders::_2));
	m_editableCanvas.DoOnDeleteShape(std::bind(&CCanvasPresenter::DeleteShape, this, std::placeholders::_1));

	auto & frame = m_canvasView->GetSelectionFrame();
	frame.DoOnControlPointDrag(std::bind(&CCanvasPresenter::OffsetShape, this, std::placeholders::_1, std::placeholders::_2));
	frame.DoOnDragEnd([this]() {
		auto shape = m_editableCanvas.GetSelection();
		shape->Commit();
	});

	for (size_t i = 0; i < m_editableCanvas.GetShapeCount(); ++i)
	{
		InsertShape(m_editableCanvas.GetShapeAtIndex(i), i);
	}
}

void CCanvasPresenter::OnCreateRectangle()
{
	m_editableCanvas.AddShape(ShapeType::Rectangle, GetDefaultRect());
}

void CCanvasPresenter::OnCreateTriangle()
{
	m_editableCanvas.AddShape(ShapeType::Triangle, GetDefaultRect());
}

void CCanvasPresenter::OnCreateEllipse()
{
	m_editableCanvas.AddShape(ShapeType::Ellipse, GetDefaultRect());
}

void CCanvasPresenter::OnInsertImage()
{
	CFileDialog fileDialog(TRUE, NULL, _T("*.bmp"));
	if (fileDialog.DoModal() == IDOK)
	{
		HANDLE hBitmap = LoadImage(NULL, fileDialog.GetPathName(), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);

		CBitmap bitmap;
		if (bitmap.Attach(hBitmap))
		{
			BITMAP bm;
			bitmap.GetBitmap(&bm);

			CRect defRect = GetDefaultRect();
			CRect imageRect(CPoint(defRect.left, defRect.top), CSize(bm.bmWidth, bm.bmHeight));

			m_editableCanvas.AddShape(ShapeType::Image, imageRect, hBitmap);

			bitmap.Detach();
		}
	}
}

void CCanvasPresenter::OnUndo()
{
	m_doc->Undo();
	m_canvasView->SelectShape(nullptr);

	m_viewOwner.Update(UpdateType::RedrawUpdateScroll);
}

void CCanvasPresenter::OnRedo()
{
	m_doc->Redo();
	m_canvasView->SelectShape(nullptr);

	m_viewOwner.Update(UpdateType::RedrawUpdateScroll);
}

void CCanvasPresenter::OnUpdateUndo(CCmdUI * pCmdUI)
{
	pCmdUI->Enable(m_doc->CanUndo());
}

void CCanvasPresenter::OnUpdateRedo(CCmdUI * pCmdUI)
{
	pCmdUI->Enable(m_doc->CanRedo());
}

void CCanvasPresenter::InsertShape(std::shared_ptr<IEditableShape> const & shape, boost::optional<size_t> pos)
{
	std::shared_ptr<IShapeView> shapeView;
	switch (shape->GetType())
	{
	case ShapeType::Rectangle:
		shapeView = m_canvasView->InsertShape(ShapeViewType::Rectangle, shape->GetRect(), pos);
		break;
	case ShapeType::Triangle:
		shapeView = m_canvasView->InsertShape(ShapeViewType::Triangle, shape->GetRect(), pos);
		break;
	case ShapeType::Ellipse:
		shapeView = m_canvasView->InsertShape(ShapeViewType::Ellipse, shape->GetRect(), pos);
		break;
	case ShapeType::Image:
		shapeView = m_canvasView->InsertShape(ShapeViewType::Image, shape->GetRect(), pos, shape->GetBitmap());
		break;
	}

	struct Context
	{
		bool moved = false;
	};

	auto context = std::make_shared<Context>();

	shapeView->DoOnMousePress([=](IShapeView & /*shape*/) {
		m_editableCanvas.SelectShape(shape);
	});

	shapeView->DoOnMouseRelease([=](IShapeView & /*shape*/, bool /*releaseInside*/) {
		if (context->moved)
		{
			shape->Commit();
		}
		context->moved = false;
		m_viewOwner.Update(UpdateType::RedrawUpdateScroll);
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
		m_editableCanvas.SelectShape(shape);
		m_editableCanvas.DeleteSelection();
	});

	std::weak_ptr<IShapeView> shapeViewWeak(shapeView);
	shape->DoOnShapeChange([=](IEditableShape const * shape) {
		if (auto shapeView = shapeViewWeak.lock())
		{
			shapeView->SetRect(shape->GetRect());
		}
		m_viewOwner.Update(UpdateType::Redraw);
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

	m_viewOwner.Update(UpdateType::RedrawUpdateScroll);
}

void CCanvasPresenter::DeleteShape(std::shared_ptr<IEditableShape> const & shape)
{
	auto it = find_if(m_shapes.begin(), m_shapes.end(),
		[&shape](Data const & data) { return data.editableShape == shape; });
	if (it == m_shapes.end())
	{
		return;
	}

	m_canvasView->DeleteShape(it->shapeView);
	m_shapes.erase(it);

	m_viewOwner.Update(UpdateType::RedrawUpdateScroll);
}

void CCanvasPresenter::OffsetShape(ControlPointType type, CPoint const & delta)
{
	auto shape = m_editableCanvas.GetSelection();

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
	auto point = m_viewOwner.GetPointInViewCenter();
	point.Offset(-WIDTH / 2, -HEIGHT / 2);

	return { point, CSize(WIDTH, HEIGHT) };
}
